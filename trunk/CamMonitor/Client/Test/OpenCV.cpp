#include "stdafx.h"
#include "OpenCV.h"
#include <ctime>

OpenCV::OpenCV()
{
}

OpenCV::~OpenCV()
{
}

void OpenCV::Init()
{
	_alert = false;
	_regionList.clear();
}

void OpenCV::Uninit()
{
	_alert = false;
	_regionList.clear();
}

// 전체 화면 영역 변화 감지
void OpenCV::StartMonitor()
{
	// 현재 프레임
	IplImage *current_image = NULL;

	// 저장 프레임
	IplImage *save_image = NULL;

	// 현재 시간
	time_t curTime;
	time_t alertTime;

	// 카메라 캡쳐 초기화
	// 0 번째 연결된 카메라로부터 연결 
	CvCapture *capture = cvCaptureFromCAM(0);

#ifdef TEST
	char *diff_captureWidow = "diff_camera";
	cvNamedWindow( diff_captureWidow, CV_WINDOW_AUTOSIZE );

	char *save_captureWidow = "save_camera";
	cvNamedWindow( save_captureWidow, CV_WINDOW_AUTOSIZE );
#endif

	// 초기화 설정
	{
		cvWaitKey(10);

		// 카메라로부터 입력된 프레임을 잡는다.
		// 만약에 실패할시 에러 메시지를 보여준다.
		if( !cvGrabFrame( capture ) ) 
		{
			AfxMessageBox("카메라가 연결되어 있지 않습니다.");
			return;
		}

		// 가져온 프레임으로부터 영상 데이터를 얻는다.
		current_image = cvRetrieveFrame( capture );

		// 파트 설정
		InitPart( current_image );

		// 성공음
		MessageBeep(MB_ICONASTERISK);

		// 이미지 저장
		save_image = cvCreateImage( cvGetSize(current_image), IPL_DEPTH_8U, current_image->nChannels);
		cvCopy( current_image, save_image );
		save_image->origin = current_image->origin;

#ifdef TEST
		cvShowImage( save_captureWidow, save_image );
#endif
	}

	while( true )
	{
		Sleep(100);
		cvWaitKey(10);

		// 카메라로부터 입력된 프레임을 잡는다.
		// 만약에 실패할시 에러 메시지를 보여준다.
		if( !cvGrabFrame( capture ) ) 
		{
			AfxMessageBox("카메라가 연결되어 있지 않습니다.");
			break;
		}

		// 가져온 프레임으로부터 영상 데이터를 얻는다.
		current_image = cvRetrieveFrame( capture );

		// 마커 매칭 테스트
		{
			IsMarker( current_image );
			cvShowImage( diff_captureWidow, current_image );
			continue;
		}

#ifdef TEST
		cvShowImage( diff_captureWidow, current_image );
#endif

		// 캠 스테이트를 가져옴
		bool isMoveCam = IsMoveCam( current_image, save_image );

		// 경보 설정
		if( !_alert )
		{
			// 캠이 움직이면 경보 설정
			if( isMoveCam )
			{
				// 경고음
				PlaySound("notify.wav",NULL,SND_APPLICATION | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

				// 시간 기록
				time( &alertTime );
				_alert = true;
			}
		}
		else
		{
			//
			time( &curTime );
			time_t diffTime = curTime - alertTime;

			// 3초간 캠 움직임 판단
			if( isMoveCam && diffTime >= 3.f )
			{
				OutputDebugString( "도난 경보\n" );
				_alert = true;
				break;
			}
			else if( !isMoveCam )
			{
				PlaySound(NULL, NULL, SND_PURGE);
				_alert = false;
			}
		}
	}

	// 할당한 메모리 해제
	if( save_image != NULL ) 
	{
		cvReleaseImage( &save_image );
	}

	// 카메라 연결 종료
	cvReleaseCapture( &capture );

#ifdef TEST
	// 윈도우 해제
	cvDestroyWindow( diff_captureWidow );
	cvDestroyWindow( save_captureWidow );
#endif
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

	char buf[1024];
	sprintf( buf, "%d \n", _regionList.size() );
	OutputDebugString( buf );
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
			uchar c1 = ((uchar*)(gray->imageData + 
				gray->widthStep*y))[x];   
			uchar c2 = ((uchar*)(oldgray->imageData +
				oldgray->widthStep*y))[x];

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
		int diff = 20; // 같은 픽셀 140-diff / 다른 픽셀 diff
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

bool OpenCV::IsMarker( IplImage* current_image )
{
	double min, max;
	CvPoint left_top;

	//
	IplImage * marker = cvLoadImage("marker.jpg", 1);	// 마커를 읽는다.
	IplImage* coeff = cvCreateImage( cvSize( current_image->width - marker->width+1, 
		current_image->height - marker->height+1 ), IPL_DEPTH_32F, 1 );	// 상관계수를 구할 이미지(C)
	coeff->origin = current_image->origin;

	//
	cvMatchTemplate(current_image, marker, coeff, CV_TM_CCOEFF_NORMED);	// 상관계수를 구하여 C 에 그린다.
	cvMinMaxLoc(coeff, &min, &max, NULL, &left_top);	// 상관계수가 최대값을 값는 위치 찾기 

#ifdef TEST
	cvShowImage( "save_camera", coeff );

	char buf[1024];
	sprintf( buf, "min : %f, max : %f \n", min, max );
	OutputDebugString( buf );
#endif

	cvRectangle(current_image, left_top, cvPoint(left_top.x + marker->width, left_top.y + marker->height), CV_RGB(255,0,0));	// 찾은 물체에 사격형을 그린다.
	
	if( max > 0.7f )
		return true;

	return false;
}
