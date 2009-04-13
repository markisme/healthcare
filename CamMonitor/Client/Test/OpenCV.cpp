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
		Sleep(100);

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
			if( camState == CAM_MOVE )
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

	// �Ҵ��� �޸� ����
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
