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

// ��ü ȭ�� ���� ��ȭ ����
void OpenCV::StartMonitor()
{
	// ���� ������
	IplImage *current_image = NULL;

	// ���� ������
	IplImage *save_image = NULL;

	// ���� �ð�
	time_t curTime;
	time_t alertTime;

	// ī�޶� ĸ�� �ʱ�ȭ
	// 0 ��° ����� ī�޶�κ��� ���� 
	CvCapture *capture = cvCaptureFromCAM(0);

#ifdef TEST
	char *diff_captureWidow = "diff_camera";
	cvNamedWindow( diff_captureWidow, CV_WINDOW_AUTOSIZE );

	char *save_captureWidow = "save_camera";
	cvNamedWindow( save_captureWidow, CV_WINDOW_AUTOSIZE );
#endif

	// �ʱ�ȭ ����
	{
		cvWaitKey(10);

		// ī�޶�κ��� �Էµ� �������� ��´�.
		// ���࿡ �����ҽ� ���� �޽����� �����ش�.
		if( !cvGrabFrame( capture ) ) 
		{
			AfxMessageBox("ī�޶� ����Ǿ� ���� �ʽ��ϴ�.");
			return;
		}

		// ������ ���������κ��� ���� �����͸� ��´�.
		current_image = cvRetrieveFrame( capture );

		// ��Ʈ ����
		InitPart( current_image );

		// ������
		MessageBeep(MB_ICONASTERISK);

		// �̹��� ����
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

		// ī�޶�κ��� �Էµ� �������� ��´�.
		// ���࿡ �����ҽ� ���� �޽����� �����ش�.
		if( !cvGrabFrame( capture ) ) 
		{
			AfxMessageBox("ī�޶� ����Ǿ� ���� �ʽ��ϴ�.");
			break;
		}

		// ������ ���������κ��� ���� �����͸� ��´�.
		current_image = cvRetrieveFrame( capture );

		CurrentFrame( current_image );
		cvShowImage( diff_captureWidow, current_image );
		continue;

#ifdef TEST
		cvShowImage( diff_captureWidow, current_image );
#endif

		// ķ ������Ʈ�� ������
		bool isMoveCam = IsMoveCam( current_image, save_image );

		// �溸 ����
		if( !_alert )
		{
			// ķ�� �����̸� �溸 ����
			if( isMoveCam )
			{
				// �����
				PlaySound("notify.wav",NULL,SND_APPLICATION | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

				// �ð� ���
				time( &alertTime );
				_alert = true;
			}
		}
		else
		{
			//
			time( &curTime );
			time_t diffTime = curTime - alertTime;

			// 3�ʰ� ķ ������ �Ǵ�
			if( isMoveCam && diffTime >= 3.f )
			{
				OutputDebugString( "���� �溸\n" );
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

	// �Ҵ��� �޸� ����
	if( save_image != NULL ) 
	{
		cvReleaseImage( &save_image );
	}

	// ī�޶� ���� ����
	cvReleaseCapture( &capture );

#ifdef TEST
	// ������ ����
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

	// ���� �׸���
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

	// üũ Ŭ����
	int size = _regionList.size();
	for( int num = 0; num < size; num++ )
	{
		RegionRect & rect = _regionList[ num ];
		rect.ClearCheck();
	}

	// ���� ���� üũ
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

	// �Ҵ��� �޸� ����
	cvReleaseImage( &gray );
	cvReleaseImage( &oldgray );

	// �Ǵ�
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
	float CriticalValue = 20.0f;

	// �� �������� ���� ����
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
				// 4���� ��Ʈ �� �ٸ� �ȼ��� ������ ī��Ʈ ����
				uchar c0 = ((uchar*)(gray->imageData + gray->widthStep*y))[x];

				// ���� ��Ʈ ��
				if( x - xpart >= 0 )
				{
					uchar c1 = ((uchar*)(gray->imageData + gray->widthStep*y))[x-xpart];
					if(fabs(float((float)c0-(float)c1))<CriticalValue) // ������ ī��Ʈ ����
					{
						left--;
					}
				}

				// ���� ��Ʈ ��
				if( x + xpart <= gray->width )
				{
					uchar c2 = ((uchar*)(gray->imageData + gray->widthStep*y))[x+xpart];
					if(fabs(float((float)c0-(float)c2))<CriticalValue) // ������ ī��Ʈ ����
					{
						right--;
					}
				}

				// ���� ��Ʈ ��
				if( y - ypart >= 0 )
				{
					uchar c3 = ((uchar*)(gray->imageData + gray->widthStep*(y-ypart)))[x];
					if(fabs(float((float)c0-(float)c3))<CriticalValue) // ������ ī��Ʈ ����
					{
						top--;
					}
				}

				// ���� ��Ʈ ��
				if( y + ypart <= gray->height )
				{
					uchar c4 = ((uchar*)(gray->imageData + gray->widthStep*(y+ypart)))[x];
					if(fabs(float((float)c0-(float)c4))<CriticalValue) // ������ ī��Ʈ ����
					{
						bottom--;
					}
				}
			}
		}

		// �ֺ��� ����� ������ �ִ��� �Ǵ�
		int diff = 20; // ���� �ȼ� 140-diff / �ٸ� �ȼ� diff
		if( left < diff || right < diff || top < diff || bottom < diff )
		{
			// ���� ��Ʈ ����
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

	// m_iplImage�� �ʱ�ȭ���� �ʾ��� ��  
	if( m_iplImage == NULL )
	{
		// m_iplImage �ʱ�ȭ 
		m_iplImage = cvCreateImage( cvGetSize(frame), 8, 3 );
		m_iplImage->origin = frame->origin;

		// �������� ���� ������ �ʱ�ȭ 
		m_iplHsv = cvCreateImage( cvGetSize(frame), 8, 3 );
		m_iplHue = cvCreateImage( cvGetSize(frame), 8, 1 );
		m_iplMask = cvCreateImage( cvGetSize(frame), 8, 1 );
		m_iplBackproject = cvCreateImage( cvGetSize(frame), 8, 1 );

		// ������׷� �ʱ�ȭ 
		m_cvHist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );
	}

	// ī�޶󿡼� �Ѱܿ� frame�� m_iplImage�� ������ ��, HSV �÷� �������� ��ȯ�Ѵ�.
	cvCopy( frame, m_iplImage, 0 );
	cvCvtColor( m_iplImage, m_iplHsv, CV_BGR2HSV );

	// ������ ���۵Ǿ��ٸ�
	if( m_bBeingTracked )
	{
		// �־��� 3��( Value ä���� max, min, Saturation ä���� min )�� ����
		// �ٽ� HSV �÷� ������ ������ ���δ�.
		cvInRangeS( m_iplHsv, cvScalar(0, m_iSMin, MIN(m_iVMin, m_iVMax),0),
			cvScalar(180,256,MAX(m_iVMin,m_iVMax),0), m_iplMask );

		// HSV �÷� ������ �����Ͽ�, Hue ä�θ� �����´�.
		cvSplit( m_iplHsv, m_iplHue, 0, 0, 0 );

		if( m_bBeingTracked < 0 )
		{
			float max_val = 0.f;

			// ��ü ������ ���� ������ ����, ���� �������� �����Ͽ� �����Ѵ�.
			cvSetImageROI( m_iplHue, m_cvRtSelected );
			cvSetImageROI( m_iplMask, m_cvRtSelected );

			// ���� ������ ������׷��� ����Ѵ�.
			cvCalcHist( &m_iplHue, m_cvHist, 0, m_iplMask );
			cvGetMinMaxHistValue( m_cvHist, 0, &max_val, 0, 0 );

			// ���� ������ ������׷��� �����Ѵ�.
			cvConvertScale( m_cvHist->bins, m_cvHist->bins, 
				max_val ? 255. / max_val : 0., 0 );

			// ���� ������ �ٽ� �ǵ����� ������ ũ��� ��ȯ�Ѵ�.
			cvResetImageROI( m_iplHue );
			cvResetImageROI( m_iplMask );

			// ���� ����� ���� ������ �ѱ��.
			m_cvRtTracked = m_cvRtSelected;

			// Hue ä�� �� ������ ��� �缳���ϴ� ���� ������ �����Ѵ�.
			m_bBeingTracked = 1;
		}

		// Hue ä���� �̹����� ������׷� ���� �ְ� backproject�� ������ ��ȯ�Ѵ�.
		cvCalcBackProject( &m_iplHue, m_iplBackproject, m_cvHist );

		// backproject�� ����� AND �����ؼ� �����Ѵ�.
		cvAnd( m_iplBackproject, m_iplMask, m_iplBackproject, 0 );

		// CAMSHIFT �˰����� �̿��� �����Ѵ�.
		cvCamShift( m_iplBackproject, m_cvRtTracked,
			cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
			&m_cvTrackComp, &m_cvTrackBox );

		// ���Ӱ� ������ ������ �ٽ� �Ҵ��Ѵ�.
		m_cvRtTracked = m_cvTrackComp.rect;

		if( m_iplImage->origin )
			m_cvTrackBox.angle = -m_cvTrackBox.angle;

		// ������ �κ��� Ÿ�������� �׷��ش�.
		cvEllipseBox( frame, m_cvTrackBox, CV_RGB(255,0,0), 3, CV_AA, 0 );
	}

	// �Ҵ��� �޸� �����Ѵ�.
	if( m_iplImage == NULL )
	{
		cvReleaseHist( &m_cvHist );
		cvReleaseImage( &m_iplHsv );
		cvReleaseImage( &m_iplHue );
		cvReleaseImage( &m_iplMask );
		cvReleaseImage( &m_iplBackproject );
	}
}