//
// 2008. 9. 22.
//
//    http://www.opencv.co.kr
//    http://www.t9t9.com
//

#include <cv.h>
#include <highgui.h>
#include <cvcam.h>
#include <stdio.h>

void callback(IplImage* image);
void testViewFromCamera_capture();
void testViewFromCamera_capture2();
void OnFrame(IplImage* image);
void Test()	;

void cvShow(char *win_name, IplImage *img, int width=0, int height=0)
{
	if(width==0){
		cvNamedWindow( win_name, 1);
	} else{
		cvNamedWindow( win_name, 0);
		cvResizeWindow( win_name, width, height);
	}
	cvShowImage( win_name, img );
}

void main()
{
	testViewFromCamera_capture2();

	if( 0 )
	{
		int *nCam = NULL;
		//int nSelected = cvcamSelectCamera(&nCam); // 카메라 선택 메뉴 띄우기
		int nSelected = cvcamGetCamerasCount(); // 자동선택 버전

		cvcamSetProperty(0, CVCAM_PROP_ENABLE, CVCAMTRUE);
		cvcamSetProperty(0, CVCAM_PROP_RENDER, CVCAMTRUE); 

		//cvcamGetProperty(*nCam, CVCAM_VIDEOFORMAT, NULL); // 해상도, 프레임 수 선택
		//cvcamGetProperty(*nCam, CVCAM_CAMERAPROPS, NULL); // 기타 셋업, 밝기, 화이트 밸런스

		int w = 640;
		int h = 480;

		cvcamSetProperty(0, CVCAM_RNDWIDTH, &w);		// 카메라 입력 사이즈 너비
		cvcamSetProperty(0, CVCAM_RNDHEIGHT, &h);		// 카메라 입력 사이즈 높이

		cvcamSetProperty(0, CVCAM_PROP_CALLBACK, callback);	// frame 마다 호출될 함수

		cvcamInit( );
		cvcamStart( );

		cvWaitKey(0);

		cvcamStop( );
		cvcamExit( );

		cvDestroyAllWindows();
	}
}

void callback(IplImage* img)
{
}

// 간단한 움직임 감지(motion detection) : 이전 프레임과 현재 프레임의 차를 구하여 처리
void testViewFromCamera_capture()
{
	// 이전 프레임
	IplImage *previous_image;

	// 현재 프레임
	IplImage *current_image;

	// 차 영상
	IplImage *diff_image = NULL;

	//
	int count = 0;
	CvScalar avg;
	double threshold = 20.0;

	// 카메라 캡쳐 초기화
	// 0 번째 연결된 카메라로부터 연결 
	CvCapture *capture = cvCaptureFromCAM(0);

	char *captureWidow = "camera";
	cvNamedWindow( captureWidow, CV_WINDOW_AUTOSIZE );
	//cvResizeWindow( captureWidow, 640, 480 );

	char *diff_captureWidow = "diff_camera";
	cvNamedWindow( diff_captureWidow, CV_WINDOW_AUTOSIZE );

	while(1)
	{
		// 카메라로부터 입력된 프레임을 잡는다.
		// 만약에 실패할시 에러 메시지를 보여준다.
		if( !cvGrabFrame( capture ) ) 
		{
			printf("프레임을 가져올 수 없습니다! \n");
			break;
		}

		// 프레임 회수를 초기화 한다. (초당 30프레임 기준)
		if( count % 30 == 0 ) count = 0;

		// 프레임 회수를 증가한다.
		count++;

		// 가져온 프레임으로부터 영상 데이터를 얻는다.
		current_image = cvRetrieveFrame( capture );

		// 초당 30프레임 기준의 현재 프레임을 이전의 프레임에 복사한다.
		if( count == 1 )
		{
			// 영상 데이타를 창에서 보여준다.
			cvShowImage( captureWidow, current_image );

			// 이전의 프레임으로 복사한다.
			previous_image = cvCreateImage( cvGetSize(current_image), 
				IPL_DEPTH_8U,
				current_image->nChannels);

			cvCopy( current_image, previous_image );
		}
		// 이전 프레임과 현재 프레임을 비교한다. 
		else
		{
			diff_image = cvCreateImage( cvGetSize(current_image), 
				IPL_DEPTH_8U,
				current_image->nChannels);

			// 차연산을 수행한다.
			cvAbsDiff( current_image, previous_image, diff_image );

			// 차연산 수행한 결과의 영상 데이타를 창에서 보여준다.
			cvShowImage( diff_captureWidow, diff_image );

			// 평균값을 구한다.
			avg = cvAvg( diff_image, NULL );

			// 만약에 주어진 값 이상이면 움직임이 있다고 판단하여 파일로 저장한다.
			printf("평균값 : %f\n", avg.val[0]);

			if( avg.val[0] > threshold )
			{
				//...( current_image를 cvSaveImage() 함수로 저장 )....
			}

			// 할당한 메모리 해제
			cvReleaseImage( &diff_image );
		}

		// 무한루프안에 cvWaitKey() 함수 호출 않을시
		// 동작하지 않으므로 반드시 있어야 함.
		// ESC 키를 만나면 종료한다.
		if( cvWaitKey(10) == 27 )
			break;
	}

	// 할당한 메모리 해제
	if( previous_image != NULL ) 
	{
		cvReleaseImage( &previous_image );
	}

	// 카메라 연결 종료
	cvReleaseCapture( &capture );
	cvDestroyWindow( captureWidow );
	cvDestroyWindow( diff_captureWidow );
}

// 전체 화면 영역 변화 감지
void testViewFromCamera_capture2()
{
	// 이전 프레임
	IplImage *previous_image;

	// 현재 프레임
	IplImage *current_image;

	// 차 영상
	IplImage *diff_image = NULL;

	//
	HWND hWnd = (HWND)cvGetWindowHandle( "diff_captureWidow" );
	HDC hDC = GetDC(hWnd);

	//
	int count = 0;
	CvScalar avg;
	double threshold = 20.0;

	// 카메라 캡쳐 초기화
	// 0 번째 연결된 카메라로부터 연결 
	CvCapture *capture = cvCaptureFromCAM(0);

	char *captureWidow = "camera";
	cvNamedWindow( captureWidow, CV_WINDOW_AUTOSIZE );
	//cvResizeWindow( captureWidow, 640, 480 );

	char *diff_captureWidow = "diff_camera";
	cvNamedWindow( diff_captureWidow, CV_WINDOW_AUTOSIZE );

	while(1)
	{
		// 카메라로부터 입력된 프레임을 잡는다.
		// 만약에 실패할시 에러 메시지를 보여준다.
		if( !cvGrabFrame( capture ) ) 
		{
			printf("프레임을 가져올 수 없습니다! \n");
			break;
		}

		// 프레임 회수를 초기화 한다. (초당 30프레임 기준)
		if( count % 30 == 0 ) count = 0;

		// 프레임 회수를 증가한다.
		count++;

		// 가져온 프레임으로부터 영상 데이터를 얻는다.
		current_image = cvRetrieveFrame( capture );

		// 초당 30프레임 기준의 현재 프레임을 이전의 프레임에 복사한다.
		if( count == 1 )
		{
			// 영상 데이타를 창에서 보여준다.
			cvShowImage( captureWidow, current_image );

			// 이전의 프레임으로 복사한다.
			previous_image = cvCreateImage( cvGetSize(current_image), 
				IPL_DEPTH_8U,
				1/*current_image->nChannels*/);

			cvCopy( current_image, previous_image );
		}
		// 이전 프레임과 현재 프레임을 비교한다. 
		else
		{
			diff_image = cvCreateImage( cvGetSize(current_image), 
				IPL_DEPTH_8U,
				1/*current_image->nChannels*/);

			IplImage* gray=cvCreateImage(cvGetSize(current_image), IPL_DEPTH_8U, 1);
			IplImage* oldgray=cvCreateImage(cvGetSize(previous_image), IPL_DEPTH_8U, 1);

			cvCvtColor(current_image, gray, CV_BGR2GRAY);
			cvCvtColor(previous_image, oldgray, CV_BGR2GRAY);


			float CriticalValue = 15.0f;
			for(int x=0; x<gray->width; x+=6) {  
				for(int y=0; y<gray->height; y+=6) {   
					uchar c1 = ((uchar*)(gray->imageData + 
						gray->widthStep*y))[x];   
					uchar c2 = ((uchar*)(oldgray->imageData +
						oldgray->widthStep*y))[x];

					if(fabs(float((float)c1-(float)c2))>CriticalValue) 
					{
						int X = 330+x;    
						int Y = diff_image->height-1-y;    
						Rectangle(hDC, X-3, Y-3, X+3, Y+3);   
					}  
				}
			}

			// 차연산을 수행한다.
			//cvAbsDiff( current_image, previous_image, diff_image );

			// 차연산 수행한 결과의 영상 데이타를 창에서 보여준다.
			cvShowImage( diff_captureWidow, diff_image );

			// 평균값을 구한다.
			//avg = cvAvg( diff_image, NULL );

			// 만약에 주어진 값 이상이면 움직임이 있다고 판단하여 파일로 저장한다.
			//printf("평균값 : %f\n", avg.val[0]);

			//if( avg.val[0] > threshold )
			{
				//...( current_image를 cvSaveImage() 함수로 저장 )....
			}

			// 할당한 메모리 해제
			cvReleaseImage( &diff_image );
		}

		// 무한루프안에 cvWaitKey() 함수 호출 않을시
		// 동작하지 않으므로 반드시 있어야 함.
		// ESC 키를 만나면 종료한다.
		if( cvWaitKey(10) == 27 )
			break;
	}

	// 할당한 메모리 해제
	if( previous_image != NULL ) 
	{
		cvReleaseImage( &previous_image );
	}

	// 카메라 연결 종료
	cvReleaseCapture( &capture );
	cvDestroyWindow( captureWidow );
	cvDestroyWindow( diff_captureWidow );
}


void Test()	
{
	int i, c, counter; 
	int INIT_TIME = 100; 
	int w = 0, h = 0;
	double B_PARAM = 1.0 / 50.0;
	double T_PARAM = 1.0 / 200.0;
	double Zeta = 10.0;
	CvCapture *capture = 0;
	IplImage *frame = 0;
	IplImage *av_img, *sgm_img;
	IplImage *lower_img, *upper_img, *tmp_img;
	IplImage *dst_img, *msk_img;
	CvFont font;
	char str[64];

	// (1) 명령 인자에 의해 지정된 번호의 카메라에 대한 캡처 구조를 
	//if (argc == 1 || (argc == 2 && strlen (argv[1]) == 1 && isdigit (argv[1][0])))    
	//	capture = cvCreateCameraCapture (argc == 2 ? argv[1][0] - '0' : 0);  

	// (2) 1 후레무캬뿌챠하고 캬뿌챠사이즈를 취득합니다.
	frame = cvQueryFrame (capture);  
	w = frame->width; 
	h = frame->height;

	// (3) 작업 공간을 생성하는
	av_img = cvCreateImage (cvSize (w, h), IPL_DEPTH_32F, 3);
	sgm_img = cvCreateImage (cvSize (w, h), IPL_DEPTH_32F, 3); 
	tmp_img = cvCreateImage (cvSize (w, h), IPL_DEPTH_32F, 3); 
	lower_img = cvCreateImage (cvSize (w, h), IPL_DEPTH_32F, 3); 
	upper_img = cvCreateImage (cvSize (w, h), IPL_DEPTH_32F, 3); 
	dst_img = cvCreateImage (cvSize (w, h), IPL_DEPTH_8U, 3); 
	msk_img = cvCreateImage (cvSize (w, h), IPL_DEPTH_8U, 1); 

	// (4) 배경의 밝기 평균 초기 값을 계산할
	printf ("Background statistics initialization start\n");
	cvSetZero (av_img);    

	for (i = 0; i < INIT_TIME; i++) 
	{ 
		frame = cvQueryFrame (capture);
		cvAcc (frame, av_img);    
	}

	cvConvertScale (av_img, av_img, 1.0 / INIT_TIME);

	// (5) 배경의 밝기 스윙의 초기 값을 계산하는 
	cvSetZero (sgm_img);

	for (i = 0; i <INIT_TIME; i++)
	{
		frame = cvQueryFrame (capture);
		cvConvert (frame, tmp_img);
		cvSub (tmp_img, av_img, tmp_img);
		cvPow (tmp_img, tmp_img, 2.0);
		cvConvertScale (tmp_img, tmp_img, 2.0);
		cvPow (tmp_img, tmp_img, 0.5);     
		cvAcc (tmp_img, sgm_img);
	} 

	cvConvertScale (sgm_img, sgm_img, 1.0 / INIT_TIME);  
	printf ("Background statistics initialization finish\n");  
	
	// (6) 디스플레이 창을 생성
	cvInitFont (&font, CV_FONT_HERSHEY_COMPLEX, 0.7, 0.7);  
	cvNamedWindow ("Input", CV_WINDOW_AUTOSIZE);  
	cvNamedWindow ("Substraction", CV_WINDOW_AUTOSIZE);  
	
	// (7) 검색 이미지에서 배경을 분리하는 루프
	counter = 0;

	while (1) {    
		frame = cvQueryFrame (capture);    
		cvConvert (frame, tmp_img);    
		// (8) 배경으로 될 수있다 화소의 밝기 값 범위 확인
		cvSub (av_img, sgm_img, lower_img);    
		cvSubS (lower_img, cvScalarAll (Zeta), lower_img);    
		cvAdd (av_img, sgm_img, upper_img);    
		cvAddS (upper_img, cvScalarAll (Zeta), upper_img);    
		cvInRange (tmp_img, lower_img, upper_img, msk_img);    
		// (9) 밝기 진폭을 다시 계산하는 cvSub (tmp_img, av_img, tmp_img);
		cvSub (tmp_img, av_img, tmp_img);    
		cvPow (tmp_img, tmp_img, 2.0);    
		cvConvertScale (tmp_img, tmp_img, 2.0);    
		cvPow (tmp_img, tmp_img, 0.5);    
		// (10) 배경으로 판단되는 영역의 배경 휘도 평균과 밝기 진폭을 업데이트할 
		cvRunningAvg (frame, av_img, B_PARAM, msk_img);    
		cvRunningAvg (tmp_img, sgm_img, B_PARAM, msk_img);    
		// (11) 물체 영역과 판단되는 분야에서 휘도 진폭 만 (배경 영역보다 느린 속도로) 업데이트
		cvNot (msk_img, msk_img);    
		cvRunningAvg (tmp_img, sgm_img, T_PARAM, msk_img);    
		// (12) 물체 영역만 출력 이미지를 복사 (배경 영역은 검정)
		cvSetZero (dst_img);    
		cvCopy (frame, dst_img, msk_img);    
		// (13) 작업 결과를 표시하는
		sprintf (str, "%03d[frame]", counter);    
		cvPutText (dst_img, str, cvPoint (10, 20), &font, CV_RGB (0, 255, 100));    
		cvShowImage ("Input", frame);    
		cvShowImage ("Substraction", dst_img);    
		counter++;    
		c = cvWaitKey (10);    
		if (c == '\x1b')      
			break;  
	}

	cvDestroyWindow ("Input");  
	cvDestroyWindow ("Substraction");  
	cvReleaseImage (&frame); 
	cvReleaseImage (&dst_img);  
	cvReleaseImage (&av_img);  
	cvReleaseImage (&sgm_img);  
	cvReleaseImage (&lower_img);  
	cvReleaseImage (&upper_img);  
	cvReleaseImage (&tmp_img);  
	cvReleaseImage (&msk_img);
}
