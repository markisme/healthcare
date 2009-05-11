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

	ARTInit();

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

		// ��Ŀ ��Ī �׽�Ʈ
		{
			IsMarker2( current_image );
			cvShowImage( diff_captureWidow, current_image );
			continue;
		}

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

bool OpenCV::IsMarker( IplImage* current_image )
{
	double min, max;
	CvPoint left_top;

	//
	IplImage * marker = cvLoadImage("marker.jpg", 1);	// ��Ŀ�� �д´�.
	IplImage* coeff = cvCreateImage( cvSize( current_image->width - marker->width+1, 
		current_image->height - marker->height+1 ), IPL_DEPTH_32F, 1 );	// �������� ���� �̹���(C)
	coeff->origin = current_image->origin;

	//
	cvMatchTemplate(current_image, marker, coeff, CV_TM_CCOEFF_NORMED);	// �������� ���Ͽ� C �� �׸���.
	cvMinMaxLoc(coeff, &min, &max, NULL, &left_top);	// �������� �ִ밪�� ���� ��ġ ã�� 

#ifdef TEST
	cvShowImage( "save_camera", coeff );

	char buf[1024];
	sprintf( buf, "min : %f, max : %f \n", min, max );
	OutputDebugString( buf );
#endif

	cvRectangle(current_image, left_top, cvPoint(left_top.x + marker->width, left_top.y + marker->height), CV_RGB(255,0,0));	// ã�� ��ü�� ������� �׸���.
	
	if( max > 0.7f )
		return true;

	return false;
}

void OpenCV::ARTInit()
{
	ARParam  wparam;
	char * vconf = "WDM_camera_flipV.xml";
	char * patt_name = "patt.hiro";
	int xsize, ysize;
	char * cparam_name    = "camera_para.dat";
	ARParam cparam;

	/* open the video path */
	if( arVideoOpen( vconf ) < 0 ) exit(0);
	/* find the size of the window */
	if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
	printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

	/* set the initial camera parameters */
	if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
		printf("Camera parameter load error !!\n");
		exit(0);
	}
	arParamChangeSize( &wparam, xsize, ysize, &cparam );
	arInitCparam( &cparam );
	printf("*** Camera Parameter ***\n");
	arParamDisp( &cparam );

    if( (arLoadPatt(patt_name)) < 0 ) {
        printf("pattern load error !!\n");
        exit(0);
    }

	/* open the graphics window */
	//argInit( &cparam, 1.0, 0, 0, 0, 0 );
}

bool OpenCV::IsMarker2( IplImage* current_image )
{
    ARMarkerInfo    *marker_info;
    int             marker_num;
	int             thresh = 100;

	//ARUint8 *dataPtr = (unsigned char*)current_image->imageData;

	int xsize = current_image->width;
	int ysize = current_image->height;

	//ARUint8 * dataPtr = new ARUint8[xsize*ysize];

	//for( int i = 0; i < ysize; i++ )
	//{
	//	int offset = current_image->widthStep * i;
	//	
	//	for( int j = 0; j < xsize; j++ )
	//	{
	//		dataPtr[4*i*xsize+4*j] = current_image->imageData[ offset + j * current_image->nChannels ];
	//		dataPtr[4*i*xsize+4*j+1] = current_image->imageData[ offset + j * current_image->nChannels+1 ];
	//		dataPtr[4*i*xsize+4*j+2] = current_image->imageData[ offset + j * current_image->nChannels+2 ];
	//	}
	//}

	ARUint8 * modifiedStorage = new ARUint8[xsize*ysize*4];

	//ARUint8 temp;
	for (int i=0,j=0;i<xsize*ysize*4;i+=4,j+=3) {
		// ARToolkit wants AGBR, we have RGBA
		modifiedStorage[i] = (char)255;
		modifiedStorage[i+1] = current_image->imageData[j+2];
		modifiedStorage[i+2] = current_image->imageData[j+1];
		modifiedStorage[i+3] = current_image->imageData[j];
	}

	/* detect the markers in the video frame */
    if( arDetectMarker(modifiedStorage, thresh, &marker_info, &marker_num) < 0 ) {
        exit(0);
    }

	char buf[1024];
	sprintf( buf, "markernum : %d , id : %d\n", marker_num, marker_info->id );
	OutputDebugString( buf );
}
