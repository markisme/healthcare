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

// ��ü ȭ�� ���� ��ȭ ����
void OpenCV::StartMonitor()
{
	_startMonitor = true;

	// ���� ������
	IplImage *previous_image;

	// ���� ������
	IplImage *current_image;

	// ���� ������
	IplImage *save_image;

	// ������ ī��Ʈ
	int count = 0;

	// ī�޶� ĸ�� �ʱ�ȭ
	// 0 ��° ����� ī�޶�κ��� ���� 
	CvCapture *capture = cvCaptureFromCAM(0);

	while(_startMonitor)
	{
		Sleep(100);

		if( _alert == true )
		{
			Sleep( 3000 );

			CamState camState = CompareImage( current_image, previous_image );
			
			// ���� 4���� ��� ���������� ī�޶� ���������� �Ǵ�
			if( camState == CAM_MOVE )
			{
				OutputDebugString( "���� �溸\n" );
				_alert = true;
				break;
			}
			else
			{
				_alert = false;
			}
		}
		
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

		// �ʴ� 30������ ������ ���� �������� ������ �����ӿ� �����Ѵ�.
		if( count == 1 )
		{
			// ������ ���������� �����Ѵ�.
			previous_image = cvCreateImage( cvGetSize(current_image), IPL_DEPTH_8U,	current_image->nChannels);

			cvCopy( current_image, previous_image );
		}
		// ���� �����Ӱ� ���� �������� ���Ѵ�. 
		else
		{
			CamState camState = CompareImage( current_image, previous_image );

			// ���� 4���� ��� �������� �ʾҴٸ� ȭ�� ����
			if( camState == NONE_MOVE )
			{
				save_image = cvCreateImage( cvGetSize(previous_image), IPL_DEPTH_8U, previous_image->nChannels);
				cvCopy( previous_image, save_image );
			}

			// ���� 4���� ��� ���������� ī�޶� ���������� �Ǵ�
			if( camState == CAM_MOVE )
			{
				OutputDebugString( "������Ȯ��\n" );
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

	// �Ҵ��� �޸� ����
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
