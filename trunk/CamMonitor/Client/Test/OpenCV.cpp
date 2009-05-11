#include "stdafx.h"
#include "OpenCV.h"
#include <ctime>

OpenCV::OpenCV() :
m_iplImage( NULL )
{
	m_iSMin = 50;
	m_iVMin = 10;
	m_iVMax = 256;
	m_bBeingTracked = 0;
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

		CurrentFrame( current_image );
		cvShowImage( diff_captureWidow, current_image );
		continue;

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

//
int hdims = 16;
float hranges_arr[] = {0,180};
float* hranges = hranges_arr;

//
void OpenCV::CurrentFrame(IplImage* frame)
{
	if(!frame) return;

	// m_iplImage이 초기화되지 않았을 때  
	if( m_iplImage == NULL )
	{
		// m_iplImage 초기화 
		m_iplImage = cvCreateImage( cvGetSize(frame), 8, 3 );
		m_iplImage->origin = frame->origin;

		// 여러가지 영상 데이터 초기화 
		m_iplHsv = cvCreateImage( cvGetSize(frame), 8, 3 );
		m_iplHue = cvCreateImage( cvGetSize(frame), 8, 1 );
		m_iplMask = cvCreateImage( cvGetSize(frame), 8, 1 );
		m_iplBackproject = cvCreateImage( cvGetSize(frame), 8, 1 );

		// 히스토그램 초기화 
		m_cvHist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );
	}

	// 카메라에서 넘겨온 frame을 m_iplImage로 복사한 후, HSV 컬러 공간으로 변환한다.
	cvCopy( frame, m_iplImage, 0 );
	cvCvtColor( m_iplImage, m_iplHsv, CV_BGR2HSV );

	// 추적이 시작되었다면
	if( m_bBeingTracked )
	{
		// 주어진 3개( Value 채널의 max, min, Saturation 채널의 min )를 갖고
		// 다시 HSV 컬러 공간내 범위를 줄인다.
		cvInRangeS( m_iplHsv, cvScalar(0, m_iSMin, MIN(m_iVMin, m_iVMax),0),
			cvScalar(180,256,MAX(m_iVMin,m_iVMax),0), m_iplMask );

		// HSV 컬러 공간을 분할하여, Hue 채널만 가져온다.
		cvSplit( m_iplHsv, m_iplHue, 0, 0, 0 );

		if( m_bBeingTracked < 0 )
		{
			float max_val = 0.f;

			// 객체 설정한 영역 정보를 갖고, 관심 영역으로 간주하여 추출한다.
			cvSetImageROI( m_iplHue, m_cvRtSelected );
			cvSetImageROI( m_iplMask, m_cvRtSelected );

			// 관심 영역의 히스토그램을 계산한다.
			cvCalcHist( &m_iplHue, m_cvHist, 0, m_iplMask );
			cvGetMinMaxHistValue( m_cvHist, 0, &max_val, 0, 0 );

			// 관심 영역의 히스토그램을 재계산한다.
			cvConvertScale( m_cvHist->bins, m_cvHist->bins, 
				max_val ? 255. / max_val : 0., 0 );

			// 관심 영역을 다시 되돌려서 원래의 크기로 반환한다.
			cvResetImageROI( m_iplHue );
			cvResetImageROI( m_iplMask );

			// 추적 대상의 영역 정보를 넘긴다.
			m_cvRtTracked = m_cvRtSelected;

			// Hue 채널 내 영역을 계속 재설정하는 논리적 오류를 방지한다.
			m_bBeingTracked = 1;
		}

		// Hue 채널의 이미지와 히스토그램 값을 넣고 backproject한 영상을 반환한다.
		cvCalcBackProject( &m_iplHue, m_iplBackproject, m_cvHist );

		// backproject한 영상과 AND 연산해서 갱신한다.
		cvAnd( m_iplBackproject, m_iplMask, m_iplBackproject, 0 );

		// CAMSHIFT 알고리즘을 이용해 추적한다.
		cvCamShift( m_iplBackproject, m_cvRtTracked,
			cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
			&m_cvTrackComp, &m_cvTrackBox );

		// 새롭게 추적한 범위를 다시 할당한다.
		m_cvRtTracked = m_cvTrackComp.rect;

		if( m_iplImage->origin )
			m_cvTrackBox.angle = -m_cvTrackBox.angle;

		// 추적한 부분을 타원형으로 그려준다.
		cvEllipseBox( frame, m_cvTrackBox, CV_RGB(255,0,0), 3, CV_AA, 0 );
	}

	// 할당한 메모리 해제한다.
	if( m_iplImage == NULL )
	{
		cvReleaseHist( &m_cvHist );
		cvReleaseImage( &m_iplHsv );
		cvReleaseImage( &m_iplHue );
		cvReleaseImage( &m_iplMask );
		cvReleaseImage( &m_iplBackproject );
	}
}