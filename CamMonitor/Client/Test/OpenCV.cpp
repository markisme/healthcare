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
		Sleep(10);

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
	IplImage* gray=cvCreateImage(cvGetSize(current_image), IPL_DEPTH_8U,1);
	IplImage* oldgray=cvCreateImage(cvGetSize(previous_image), IPL_DEPTH_8U, 1);

	cvCvtColor(current_image, gray, CV_BGR2GRAY);
	cvCvtColor(previous_image, oldgray, CV_BGR2GRAY);

	int xpart = gray->width / 4;
	int ypart = gray->height / 4;

	std::vector<ResionRect> resionList;
	ResionRect resion01(xpart*0,ypart*0,xpart*1,ypart*1);
	ResionRect resion02(xpart*1,ypart*0,xpart*2,ypart*1);
	ResionRect resion03(xpart*2,ypart*0,xpart*3,ypart*1);
	ResionRect resion04(xpart*3,ypart*0,xpart*4,ypart*1);
	ResionRect resion05(xpart*0,ypart*1,xpart*1,ypart*2);
	ResionRect resion06(xpart*1,ypart*1,xpart*2,ypart*2);
	ResionRect resion07(xpart*2,ypart*1,xpart*3,ypart*2);
	ResionRect resion08(xpart*3,ypart*1,xpart*4,ypart*2);
	ResionRect resion09(xpart*0,ypart*2,xpart*1,ypart*3);
	ResionRect resion10(xpart*1,ypart*2,xpart*2,ypart*3);
	ResionRect resion11(xpart*2,ypart*2,xpart*3,ypart*3);
	ResionRect resion12(xpart*3,ypart*2,xpart*4,ypart*3);
	ResionRect resion13(xpart*0,ypart*3,xpart*1,ypart*4);
	ResionRect resion14(xpart*1,ypart*3,xpart*2,ypart*4);
	ResionRect resion15(xpart*2,ypart*3,xpart*3,ypart*4);
	ResionRect resion16(xpart*3,ypart*3,xpart*4,ypart*4);
	
	resionList.push_back( resion01 );
	resionList.push_back( resion02 );
	resionList.push_back( resion03 );
	resionList.push_back( resion04 );
	resionList.push_back( resion05 );
	resionList.push_back( resion06 );
	resionList.push_back( resion07 );
	resionList.push_back( resion08 );
	resionList.push_back( resion09 );
	resionList.push_back( resion10 );
	resionList.push_back( resion11 );
	resionList.push_back( resion12 );
	resionList.push_back( resion13 );
	resionList.push_back( resion14 );
	resionList.push_back( resion15 );
	resionList.push_back( resion16 );

	float CriticalValue = 15.0f;
	for(int x=0; x<gray->width; x+=6) {  
		for(int y=0; y<gray->height; y+=6) {   
			uchar c1 = ((uchar*)(gray->imageData + 
				gray->widthStep*y))[x];   
			uchar c2 = ((uchar*)(oldgray->imageData +
				oldgray->widthStep*y))[x];

			if(fabs(float((float)c1-(float)c2))>CriticalValue) 
			{

				int size = resionList.size();
				for( int num = 0; num < size; num++ )
				{
					ResionRect & rect = resionList[ num ];
					rect.CheckResion( x, y );
				}
			}  
		}
	}

	// 할당한 메모리 해제
	cvReleaseImage( &gray );
	cvReleaseImage( &oldgray );

	// 판단
	int totalCount = 0;
	int size = resionList.size();
	for( int num = 0; num < size; num++ )
	{
		ResionRect & rect = resionList[ num ];
		int count = rect._checkCount;
		if( rect._checkCount > 20 )
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
		ResionRect & rect = resionList[ num ];
		if( rect._checkCount < 10 )
		{
			totalCount++;
		}
	}

	if( totalCount >= 4 )
	{
		return NONE_MOVE;
	}

	return OBJECT_MOVE;
}
