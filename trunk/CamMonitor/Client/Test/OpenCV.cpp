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

	while(_startMonitor)
	{
		Sleep(100);

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
		if( count == 1 && save_image == NULL )
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
			if( camState == CAM_MOVE )
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
	int _resionLeftTop = 0;
	int _resionRightTop = 0;
	int _resionLeftBottom = 0;
	int _resionRightBottom = 0;

	IplImage* gray=cvCreateImage(cvGetSize(current_image), IPL_DEPTH_8U,1);
	IplImage* oldgray=cvCreateImage(cvGetSize(previous_image), IPL_DEPTH_8U, 1);

	cvCvtColor(current_image, gray, CV_BGR2GRAY);
	cvCvtColor(previous_image, oldgray, CV_BGR2GRAY);

	int xpart = gray->width / 8;
	int ypart = gray->height / 8;

	struct Pos
	{
		int _x;
		int _y;
	};

	std::vector<Pos> posList;
	Pos pos;
	pos._x = xpart * 1;
	pos._y = xpart * 1;
	posList.push_back( pos );

	float CriticalValue = 15.0f;
	for(int x=0; x<gray->width; x+=6) {  
		for(int y=0; y<gray->height; y+=6) {   
			uchar c1 = ((uchar*)(gray->imageData + 
				gray->widthStep*y))[x];   
			uchar c2 = ((uchar*)(oldgray->imageData +
				oldgray->widthStep*y))[x];

			if(fabs(float((float)c1-(float)c2))>CriticalValue) 
			{
				if( 0 <= x && x <= xpart && 0 <= y && y <= ypart )
				{
					_resionLeftTop++;
				}
				else if( 3 * xpart <= x && x <= 4 * xpart && 0 <= y && y <= ypart )
				{
					_resionRightTop++;
				}
				else if( 0 <= x && x <= xpart && 3 * ypart <= y && y <= 4 * ypart )
				{
					_resionLeftBottom++;
				}
				else if( 3 * xpart <= x && x <= 4 * xpart && 3 * ypart <= y && y <= 4 * ypart )
				{
					_resionRightBottom++;
				}
			}  
		}
	}

	// 할당한 메모리 해제
	cvReleaseImage( &gray );
	cvReleaseImage( &oldgray );

	//
	if( _resionLeftTop == 0 || _resionRightTop == 0 || 
		_resionLeftBottom == 0 || _resionRightBottom == 0 )
	{
		return NONE_MOVE;
	}
	else if( _resionLeftTop > 10 && _resionRightTop > 10 && 
			 _resionLeftBottom > 10 && _resionRightBottom > 10 )
	{
		return CAM_MOVE;
	}

	return OBJECT_MOVE;
}
