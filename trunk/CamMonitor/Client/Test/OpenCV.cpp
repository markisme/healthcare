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

// ��ü ȭ�� ���� ��ȭ ����
void OpenCV::StartMonitor()
{
	_startMonitor = true;

	// ���� ������
	IplImage *previous_image = NULL;

	// ���� ������
	IplImage *current_image = NULL;

	// ���� ������
	IplImage *save_image = NULL;

	// ���� �ð�
	time_t curTime;
	time_t alertTime;

	// ������ ī��Ʈ
	int count = 0;

	// ī�޶� ĸ�� �ʱ�ȭ
	// 0 ��° ����� ī�޶�κ��� ���� 
	CvCapture *capture = cvCaptureFromCAM(0);

	while(_startMonitor)
	{
		Sleep(10);

		// ī�޶�κ��� �Էµ� �������� ��´�.
		// ���࿡ �����ҽ� ���� �޽����� �����ش�.
		if( !cvGrabFrame( capture ) ) 
		{
			AfxMessageBox("ī�޶� ����Ǿ� ���� �ʽ��ϴ�.");
			break;
		}

		// ������ ȸ���� �ʱ�ȭ �Ѵ�.
		if( count % 2 == 0 ) count = 0;

		// ������ ȸ���� �����Ѵ�.
		count++;

		// ������ ���������κ��� ���� �����͸� ��´�.
		current_image = cvRetrieveFrame( capture );

		if( _alert == true && save_image != NULL )
		{
			CamState camState = CompareImage( current_image, save_image );

			//
			time( &curTime );
			time_t diffTime = curTime - alertTime;

			// ���� 4���� ��� ���������� ī�޶� ���������� �Ǵ�
			if( camState == CAM_MOVE && diffTime >= 3.f )
			{
				OutputDebugString( "���� �溸\n" );
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

		// �ʴ� 30������ ������ ���� �������� ������ �����ӿ� �����Ѵ�.
		if( count == 1 && save_image == NULL )
		{
			// ������ ���������� �����Ѵ�.
			previous_image = cvCreateImage( cvGetSize(current_image), IPL_DEPTH_8U,	current_image->nChannels);

			cvCopy( current_image, previous_image );
		}
		// ���� �����Ӱ� ���� �������� ���Ѵ�. 
		else
		{
			// ķ ������Ʈ�� ������
			CamState camState = CompareImage( current_image, previous_image );	

			// ���� 4���� �������� �ʾҴٸ� ȭ�� ����
			if( camState == NONE_MOVE && save_image == NULL )
			{
				// ������
				MessageBeep(MB_ICONASTERISK);

				// �̹��� ����
				save_image = cvCreateImage( cvGetSize(previous_image), IPL_DEPTH_8U, previous_image->nChannels);
				cvCopy( previous_image, save_image );
			}

			// ���� 4���� ��� ���������� ī�޶� ���������� �Ǵ�
			if( camState == CAM_MOVE && _alert != true )
			{
				// �����
				PlaySound("notify.wav",NULL,SND_APPLICATION | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				
				// �ð� ���
				time( &alertTime );
				_alert = true;
			}
		}
	}

	// �Ҵ��� �޸� ����
	if( previous_image != NULL ) 
	{
		cvReleaseImage( &previous_image );
	}

	// ī�޶� ���� ����
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

	// �Ҵ��� �޸� ����
	cvReleaseImage( &gray );
	cvReleaseImage( &oldgray );

	// �Ǵ�
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
