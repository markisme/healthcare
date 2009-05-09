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
}

void OpenCV::Uninit()
{
	_alert = false;

	_regionList.clear();
}

// 전체 화면 영역 변화 감지
void OpenCV::StartMonitor()
{
	_startMonitor = true;

	// 이전 프레임
	IplImage *previous_image = NULL;

	// 현재 프레임
	IplImage *current_image = NULL;

	// 저장 프레임
	IplImage *save_image = NULL;

	// 현재 시간
	time_t curTime;
	time_t alertTime;

	// 프레임 카운트
	int count = 0;

	// 카메라 캡쳐 초기화
	// 0 번째 연결된 카메라로부터 연결 
	CvCapture *capture = cvCaptureFromCAM(0);

#ifdef TEST
	char *diff_captureWidow = "diff_camera";
	cvNamedWindow( diff_captureWidow, CV_WINDOW_AUTOSIZE );
#endif

	while(_startMonitor)
	{
		Sleep(50);
		cvWaitKey(10);

		// 카메라로부터 입력된 프레임을 잡는다.
		// 만약에 실패할시 에러 메시지를 보여준다.
		if( !cvGrabFrame( capture ) ) 
		{
			AfxMessageBox("카메라가 연결되어 있지 않습니다.");
			break;
		}

		// 프레임 회수를 초기화 한다.
		if( count % 2 == 0 ) count = 0;

		// 프레임 회수를 증가한다.
		count++;

		// 가져온 프레임으로부터 영상 데이터를 얻는다.
		current_image = cvRetrieveFrame( capture );

		// 수정해야함.. 맨 처음으로
		InitPart( current_image );

		if( _alert == true && save_image != NULL )
		{
			CamState camState = CompareImage( current_image, save_image );

			//
			time( &curTime );
			time_t diffTime = curTime - alertTime;

			// 지역 4개가 모두 움직였으면 카메라 움직임으로 판단
			if( camState == CAM_MOVE && diffTime >= 3.f )
			{
				OutputDebugString( "도난 경보\n" );
				_alert = true;
				break;
			}
			else if( camState != CAM_MOVE )
			{
				PlaySound(NULL, NULL, SND_PURGE);
				_alert = false;
				count = 0;
			}

			continue;
		}

		// 초당 30프레임 기준의 현재 프레임을 이전의 프레임에 복사한다.
		if( count == 1 )
		{
			// 이전의 프레임으로 복사한다.
			previous_image = cvCreateImage( cvGetSize(current_image), IPL_DEPTH_8U,	current_image->nChannels);

			cvCopy( current_image, previous_image );
		}
		// 이전 프레임과 현재 프레임을 비교한다. 
		else
		{
			// 캠 스테이트를 가져옴
			CamState camState = CompareImage( current_image, previous_image );	

#ifdef TEST
			cvShowImage( diff_captureWidow, current_image );
#endif

			// 지역 4개가 움직이지 않았다면 화면 저장
			if( camState == NONE_MOVE && save_image == NULL )
			{
				// 성공음
				MessageBeep(MB_ICONASTERISK);

				// 이미지 저장
				save_image = cvCreateImage( cvGetSize(previous_image), IPL_DEPTH_8U, previous_image->nChannels);
				cvCopy( previous_image, save_image );
			}

			// 지역 4개가 모두 움직였으면 카메라 움직임으로 판단
			if( camState == CAM_MOVE && _alert != true )
			{
				// 경고음
				PlaySound("notify.wav",NULL,SND_APPLICATION | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				
				// 시간 기록
				time( &alertTime );
				_alert = true;
			}
		}
	}

	// 할당한 메모리 해제
	if( previous_image != NULL ) 
	{
		cvReleaseImage( &previous_image );
	}

	// 카메라 연결 종료
	cvReleaseCapture( &capture );
}

void OpenCV::StopMonitor()
{
	_startMonitor = false;
}

CamState OpenCV::CompareImage( IplImage* current_image, IplImage* previous_image )
{
	HWND hWnd = (HWND)cvGetWindowHandle( "diff_camera" );
	HDC hDC = GetDC(hWnd);

	IplImage* gray=cvCreateImage(cvGetSize(current_image), IPL_DEPTH_8U,1);
	IplImage* oldgray=cvCreateImage(cvGetSize(previous_image), IPL_DEPTH_8U, 1);

	cvCvtColor(current_image, gray, CV_BGR2GRAY);
	cvCvtColor(previous_image, oldgray, CV_BGR2GRAY);

	// 변한 영역 체크
	float CriticalValue = 15.0f;
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
#ifdef TEST
					//rect.DrawResion( hDC );
#endif
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
	int size = _regionList.size();
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
		return CAM_MOVE;
	}

	totalCount = 0;
	for( int num = 0; num < size; num++ )
	{
		RegionRect & rect = _regionList[ num ];
		if( rect._checkCount < 2 )
		{
			totalCount++;
		}
	}

	if( totalCount >= 60 )
	{
		return NONE_MOVE;
	}

	return OBJECT_MOVE;
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
	float CriticalValue = 15.0f;

	// 각 영역별로 포문 돌기
	RegionList::iterator it = _regionList.begin();
	for( ; it != _regionList.end(); it++ )
	{
		//
		RegionRect & rect = *it;

		//
		int rectCnt = rect._rightbottomX * rect._rightbottomY;
		int left = rectCnt;
		int right = rectCnt;
		int top = rectCnt;
		int bottom = rectCnt;

		for(int x=rect._lefttopX; x<rect._rightbottomX; x+=6) 
		{  
			for(int y=rect._lefttopY; y<rect._rightbottomY; y+=6) 
			{
				//
				uchar c0 = ((uchar*)(gray->imageData + gray->widthStep*y))[x];

				// 좌측 파트 비교
				if( x - xpart >= 0 )
				{
					uchar c1 = ((uchar*)(gray->imageData + gray->widthStep*y))[x-xpart];
					if(fabs(float((float)c0-(float)c1))>CriticalValue)
					{
						left--;
					}
				}

				// 우측 파트 비교
				if( x + xpart <= gray->width )
				{
					uchar c2 = ((uchar*)(gray->imageData + gray->widthStep*y))[x+xpart];
					if(fabs(float((float)c0-(float)c2))>CriticalValue)
					{
						right--;
					}
				}

				// 상위 파트 비교
				if( y - ypart >= 0 )
				{
					uchar c3 = ((uchar*)(gray->imageData + gray->widthStep*(y-ypart)))[x];
					if(fabs(float((float)c0-(float)c3))>CriticalValue)
					{
						top--;
					}
				}

				// 하위 파트 비교
				if( y + ypart <= gray->height )
				{
					uchar c4 = ((uchar*)(gray->imageData + gray->widthStep*(y+ypart)))[x];
					if(fabs(float((float)c0-(float)c4))>CriticalValue)
					{
						bottom--;
					}
				}
			}
		}

		// 주변에 비슷한 영역이 있는지 판단
		if( left < 10 || right < 10 || top < 10 || bottom < 10 )
		{
			// 현재 파트 삭제
			_regionList.erase( it );
		}
	}
}