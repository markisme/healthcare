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

#ifdef TEST
	char *diff_captureWidow = "diff_camera";
	cvNamedWindow( diff_captureWidow, CV_WINDOW_AUTOSIZE );
#endif

	while(_startMonitor)
	{
		Sleep(50);
		cvWaitKey(10);

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

		// �����ؾ���.. �� ó������
		InitPart( current_image );

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
		if( count == 1 )
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

#ifdef TEST
			cvShowImage( diff_captureWidow, current_image );
#endif

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
	HWND hWnd = (HWND)cvGetWindowHandle( "diff_camera" );
	HDC hDC = GetDC(hWnd);

	IplImage* gray=cvCreateImage(cvGetSize(current_image), IPL_DEPTH_8U,1);
	IplImage* oldgray=cvCreateImage(cvGetSize(previous_image), IPL_DEPTH_8U, 1);

	cvCvtColor(current_image, gray, CV_BGR2GRAY);
	cvCvtColor(previous_image, oldgray, CV_BGR2GRAY);

	// ���� ���� üũ
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

	// �Ҵ��� �޸� ����
	cvReleaseImage( &gray );
	cvReleaseImage( &oldgray );

	// �Ǵ�
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
	// ���߿� ������ �̺κ�
	_regionList.clear();

	// ����
	int count = 8;
	int xpart = current_image->width / count;
	int ypart = current_image->height / count;

	// �ڽ� ������
	for( int num = 0; num < count; num++ )
	{
		for( int cnt = 0; cnt < count; cnt++ )
		{
			RegionRect region((num*10)+cnt,xpart*cnt,ypart*num,xpart*(cnt+1),ypart*(num+1));
			_regionList.push_back( region );
		}
	}

	// ��Ʈ ���� ����
	ComparePart( current_image );
}

void OpenCV::ComparePart( IplImage* current_image )
{
	IplImage* gray=cvCreateImage(cvGetSize(current_image), IPL_DEPTH_8U,1);
	cvCvtColor(current_image, gray, CV_BGR2GRAY);

	// ����
	int count = 8;
	int xpart = gray->width / count;
	int ypart = gray->height / count;
	float CriticalValue = 15.0f;

	// �� �������� ���� ����
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

				// ���� ��Ʈ ��
				if( x - xpart >= 0 )
				{
					uchar c1 = ((uchar*)(gray->imageData + gray->widthStep*y))[x-xpart];
					if(fabs(float((float)c0-(float)c1))>CriticalValue)
					{
						left--;
					}
				}

				// ���� ��Ʈ ��
				if( x + xpart <= gray->width )
				{
					uchar c2 = ((uchar*)(gray->imageData + gray->widthStep*y))[x+xpart];
					if(fabs(float((float)c0-(float)c2))>CriticalValue)
					{
						right--;
					}
				}

				// ���� ��Ʈ ��
				if( y - ypart >= 0 )
				{
					uchar c3 = ((uchar*)(gray->imageData + gray->widthStep*(y-ypart)))[x];
					if(fabs(float((float)c0-(float)c3))>CriticalValue)
					{
						top--;
					}
				}

				// ���� ��Ʈ ��
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

		// �ֺ��� ����� ������ �ִ��� �Ǵ�
		if( left < 10 || right < 10 || top < 10 || bottom < 10 )
		{
			// ���� ��Ʈ ����
			_regionList.erase( it );
		}
	}
}