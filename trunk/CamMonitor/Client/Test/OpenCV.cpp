#include "stdafx.h"
#include "OpenCV.h"
#include <ctime>

OpenCV::OpenCV() : 
_saveImage( NULL ),
_checkMarkerCnt( 0 ),
_alert( FALSE )
{
}

OpenCV::~OpenCV()
{
}

void OpenCV::Init()
{
	_regionList.clear();

	// 마커 설정을 위한 초기화
	ARTInit();

	// 카메라 캡쳐 초기화
	// 0 번째 연결된 카메라로부터 연결 
	_capture = cvCaptureFromCAM(0);

	// 카메라로부터 입력된 프레임을 잡는다.
	// 만약에 실패할시 에러 메시지를 보여준다.
	if( !cvGrabFrame( _capture ) ) 
	{
		AfxMessageBox("카메라가 연결되어 있지 않습니다.");
		return;
	}

#ifdef TEST
	cvNamedWindow( "diff_camera", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "save_camera", CV_WINDOW_AUTOSIZE );
#endif

	// 가져온 프레임으로부터 영상 데이터를 얻는다.
	IplImage * current_image = cvRetrieveFrame( _capture );

	IplImage* cp = cvCreateImage( cvGetSize(current_image), IPL_DEPTH_8U, current_image->nChannels);
	cvFlip( current_image, cp, 0 );

	// 파트 설정
	InitPart( cp );

	// 성공음
	MessageBeep( MB_ICONASTERISK );

	// 이미지 저장
	_saveImage = cvCreateImage( cvGetSize(current_image), IPL_DEPTH_8U, current_image->nChannels);
	cvCopy( current_image, _saveImage );

#ifdef TEST
	_saveImage->origin = current_image->origin;
	cvShowImage( "save_camera", _saveImage );
#endif
	cvReleaseImage( &cp );
}

void OpenCV::Uninit()
{
	_regionList.clear();

	// 할당한 메모리 해제
	if( _saveImage != NULL ) 
	{
		cvReleaseImage( &_saveImage );
	}

#ifdef TEST
	// 윈도우 해제
	cvDestroyWindow( "diff_camera" );
	cvDestroyWindow( "save_camera" );
#endif

	// 카메라 연결 종료
	cvReleaseCapture( &_capture );
}

// 전체 화면 영역 변화 감지
BOOL OpenCV::UpdateMonitor()
{
	//Sleep(200);
	cvWaitKey(10);

	// 카메라로부터 입력된 프레임을 잡는다.
	// 만약에 실패할시 에러 메시지를 보여준다.
	if( !cvGrabFrame( _capture ) ) 
	{
		AfxMessageBox("카메라가 연결되어 있지 않습니다.");
		return FALSE;
	}

	// 가져온 프레임으로부터 영상 데이터를 얻는다.
	IplImage * current_image = cvRetrieveFrame( _capture );

	// 해제를 위한 마커 매칭
	if( IsMarker( current_image ) )
	{
		_checkMarkerCnt++;
	}
	else
	{
		_checkMarkerCnt = 0;
	}

	// 마커 체크 해제 모드로
	if( _checkMarkerCnt > 20 )
	{
		return FALSE;
	}

#ifdef TEST
	cvShowImage( "diff_camera", current_image );
#endif

	// 캠 스테이트를 가져옴
	bool isMoveCam = IsMoveCam( current_image, _saveImage );

	// 경보 설정
	if( !_alert )
	{
		// 캠이 움직이면 경보 설정
		if( isMoveCam )
		{
			// 경고음
			PlaySound("notify.wav",NULL,SND_APPLICATION | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

			// 시간 기록
			time( &_alertTime );
			_alert = true;
		}
	}
	else
	{
		//
		time_t curTime;
		time( &curTime );
		time_t diffTime = curTime - _alertTime;

		// 3초간 캠 움직임 판단
		if( isMoveCam && diffTime >= 3.f )
		{
			OutputDebugString( "도난 경보\n" );
			_alert = true;
			return FALSE;
		}
		else if( !isMoveCam )
		{
			PlaySound(NULL, NULL, SND_PURGE);
			_alert = false;
		}
	}

	return TRUE;
}

bool OpenCV::IsMoveCam( IplImage* current_image, IplImage* previous_image )
{
	HWND hWnd = (HWND)cvGetWindowHandle( "diff_camera" );
	HDC hDC = GetDC(hWnd);

	IplImage* gray=cvCreateImage(cvGetSize(current_image), IPL_DEPTH_8U,1);
	IplImage* oldgray=cvCreateImage(cvGetSize(previous_image), IPL_DEPTH_8U, 1);

	cvCvtColor(current_image, gray, CV_BGR2GRAY);
	cvCvtColor(previous_image, oldgray, CV_BGR2GRAY);

	// 영역 그리기
#ifdef TEST
	int regionCnt = _regionList.size();
	for( int num = 0; num < regionCnt; num++ )
	{
		RegionRect & rect = _regionList[ num ];
		rect.DrawRegion( hDC );
	}
#endif

	// 체크 클리어
	int size = _regionList.size();
	for( int num = 0; num < size; num++ )
	{
		RegionRect & rect = _regionList[ num ];
		rect.ClearCheck();
	}

	// 변한 영역 체크
	float CriticalValue = 20.0f;
	for(int x=0; x<gray->width; x+=6) {  
		for(int y=0; y<gray->height; y+=6) {   
			uchar c1 = ((uchar*)(gray->imageData + gray->widthStep*y))[x];   
			uchar c2 = ((uchar*)(oldgray->imageData + oldgray->widthStep*y))[x];

			if(fabs(float((float)c1-(float)c2))>CriticalValue) 
			{
				int size = _regionList.size();
				for( int num = 0; num < size; num++ )
				{
					RegionRect & rect = _regionList[ num ];
					rect.CheckRegion( x, y );
				}
#ifdef TEST
				int X = x;    
				int Y = previous_image->height-1-y;    
				Rectangle(hDC, X-3, Y-3, X+3, Y+3);
#endif
			}  
		}
	}

	// 할당한 메모리 해제
	cvReleaseImage( &gray );
	cvReleaseImage( &oldgray );

	// 판단
	int totalCount = 0;
	for( int num = 0; num < size; num++ )
	{
		RegionRect & rect = _regionList[ num ];
		int count = rect._checkCount;
		if( rect._checkCount > 5 )
		{
			totalCount++;
		}
	}

	if( totalCount >= size )
	{
		return true;
	}

	return false;
}

void OpenCV::InitPart( IplImage* current_image )
{
	// 나중에 지우자 이부분
	_regionList.clear();

	// 설정
	int count = 8;
	int xpart = current_image->width / count;
	int ypart = current_image->height / count;

	// 박스 나누기
	for( int num = 0; num < count; num++ )
	{
		for( int cnt = 0; cnt < count; cnt++ )
		{
			RegionRect region((num*10)+cnt,xpart*cnt,ypart*num,xpart*(cnt+1),ypart*(num+1));
			_regionList.push_back( region );
		}
	}

	// 파트 비교후 삭제
	ComparePart( current_image );
}

void OpenCV::ComparePart( IplImage* current_image )
{
	IplImage* gray=cvCreateImage(cvGetSize(current_image), IPL_DEPTH_8U,1);
	cvCvtColor(current_image, gray, CV_BGR2GRAY);

	// 설정
	int count = 8;
	int xpart = gray->width / count;
	int ypart = gray->height / count;
	float CriticalValue = 20.0f;

	// 각 영역별로 포문 돌기
	RegionList::iterator it = _regionList.begin();
	for( ; it != _regionList.end(); it++ )
	{
		//
		RegionRect & rect = *it;

		//
		int left = 140;
		int right = 140;
		int top = 140;
		int bottom = 140;

		for(int x=rect._lefttopX; x<rect._rightbottomX; x+=6) 
		{  
			for(int y=rect._lefttopY; y<rect._rightbottomY; y+=6) 
			{
				// 4방의 파트 비교 다른 픽셀이 있을시 카운트 증가
				uchar c0 = ((uchar*)(gray->imageData + gray->widthStep*y))[x];

				// 좌측 파트 비교
				if( x - xpart >= 0 )
				{
					uchar c1 = ((uchar*)(gray->imageData + gray->widthStep*y))[x-xpart];
					if(fabs(float((float)c0-(float)c1))<CriticalValue) // 같으면 카운트 빼기
					{
						left--;
					}
				}

				// 우측 파트 비교
				if( x + xpart <= gray->width )
				{
					uchar c2 = ((uchar*)(gray->imageData + gray->widthStep*y))[x+xpart];
					if(fabs(float((float)c0-(float)c2))<CriticalValue) // 같으면 카운트 빼기
					{
						right--;
					}
				}

				// 상위 파트 비교
				if( y - ypart >= 0 )
				{
					uchar c3 = ((uchar*)(gray->imageData + gray->widthStep*(y-ypart)))[x];
					if(fabs(float((float)c0-(float)c3))<CriticalValue) // 같으면 카운트 빼기
					{
						top--;
					}
				}

				// 하위 파트 비교
				if( y + ypart <= gray->height )
				{
					uchar c4 = ((uchar*)(gray->imageData + gray->widthStep*(y+ypart)))[x];
					if(fabs(float((float)c0-(float)c4))<CriticalValue) // 같으면 카운트 빼기
					{
						bottom--;
					}
				}
			}
		}

		// 주변에 비슷한 영역이 있는지 판단
		int diff = 10; // 같은 픽셀 140-diff / 다른 픽셀 diff
		if( left < diff || right < diff || top < diff || bottom < diff )
		{
			// 현재 파트 삭제
			if( it != _regionList.begin() )
			{
				_regionList.erase( it-- );
			}
			else
			{
				_regionList.erase( it );
				it = _regionList.begin();
			}
		}
	}
}

void OpenCV::ARTInit()
{
	char * vconf = "Data\\WDM_camera_flipV.xml";
	char * cparam_name = "Data\\camera_para.dat";
	char * patt_name = "Data\\patt.hiro";
	int xsize, ysize;
	ARParam  wparam;
	ARParam cparam;

	/* open the video path */
	if( arVideoOpen( vconf ) < 0 ) exit(0);
	/* find the size of the window */
	if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
	/* set the initial camera parameters */
	if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
		printf("Camera parameter load error !!\n");
		exit(0);
	}
	arParamChangeSize( &wparam, xsize, ysize, &cparam );
	arInitCparam( &cparam );

	int id = arLoadPatt(patt_name);

    if( (arLoadPatt(patt_name)) < 0 ) {
        printf("pattern load error !!\n");
        exit(0);
    }
}	

bool OpenCV::IsMarker( IplImage* current_image )
{
    ARMarkerInfo    *marker_info;
    int             marker_num;
	int             thresh = 100;

	int xsize = current_image->width;
	int ysize = current_image->height;

	ARUint8 * dataPtr = new ARUint8[xsize*ysize*4];
	for (int i=0,j=0;i<xsize*ysize*4;i+=4,j+=3) {
		// ARToolkit wants AGBR, we have RGBA
		dataPtr[i] = (char)255;
		dataPtr[i+1] = current_image->imageData[j+2];
		dataPtr[i+2] = current_image->imageData[j+1];
		dataPtr[i+3] = current_image->imageData[j];
	}

	/* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 ) {
        exit(0);
    }

	CvPoint pos;
	pos.x = marker_info->pos[0]; 
	pos.y = marker_info->pos[1];
	cvRectangle(current_image, pos, cvPoint(pos.x + 10, pos.y + 10), CV_RGB(255,0,0));	// 찾은 물체에 사격형을 그린다.

//#ifdef TEST
	char buf[1024];
	sprintf( buf, "%d, %d\n", marker_num, marker_info[0].id );
	OutputDebugString( buf );
//#endif

	delete( dataPtr );
	dataPtr = NULL;

	if( marker_num > 0 )
	{
		return true;
	}

	return false;
}
