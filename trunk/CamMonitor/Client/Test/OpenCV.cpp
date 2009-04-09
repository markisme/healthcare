#include "stdafx.h"
#include "OpenCV.h"

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
			CamState camState = CompareImage( current_image, previous_image );

			// 지역 4개가 모두 움직이지 않았다면 화면 저장
			if( camState == NONE_MOVE )
			{
				save_image = cvCreateImage( cvGetSize(previous_image), IPL_DEPTH_8U, previous_image->nChannels);
				cvCopy( previous_image, save_image );
			}

			// 지역 4개가 모두 움직였으면 카메라 움직임으로 판단
			if( camState == CAM_MOVE )
			{
				OutputDebugString( "움직임확인\n" );
				_alert = true;
				continue;
			}

			if( _alert == true )
			{
				Sleep( 1000 );

				if( save_image == NULL )
				{
					save_image = cvCreateImage( cvGetSize(previous_image), IPL_DEPTH_8U, previous_image->nChannels);
					cvCopy( previous_image, save_image );
				}

				CamState camState = CompareImage( current_image, save_image );

				// 지역 4개가 모두 움직였으면 카메라 움직임으로 판단
				if( camState == CAM_MOVE )
				{
					OutputDebugString( "도난 경보\n" );
					_alert = true;
					break;
				}
				else
				{
					_alert = false;
				}
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

	int resion1 = 0;
	int resion2 = 0;
	int resion3 = 0;
	int resion4 = 0;

	int xpart = gray->width / 4;
	int ypart = gray->height / 4;

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
	if( _resionLeftTop == 0 && _resionRightTop == 0 && 
		_resionLeftBottom == 0 && _resionRightBottom == 0 )
	{
		return NONE_MOVE;
	}
	else if( _resionLeftTop > 0 && _resionRightTop > 0 && 
			 _resionLeftBottom > 0 && _resionRightBottom > 0 )
	{
		return CAM_MOVE;
	}

	return OBJECT_MOVE;
}
