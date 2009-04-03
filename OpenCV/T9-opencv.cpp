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
	cvResizeWindow( captureWidow, 640, 480 );

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
	int count = 0;
	CvScalar avg;
	double threshold = 20.0;

	// 카메라 캡쳐 초기화
	// 0 번째 연결된 카메라로부터 연결 
	CvCapture *capture = cvCaptureFromCAM(0);

	//char *captureWidow = "camera";
	//cvNamedWindow( captureWidow, CV_WINDOW_AUTOSIZE );

	//char *diff_captureWidow = "diff_camera";
	//cvNamedWindow( diff_captureWidow, CV_WINDOW_AUTOSIZE );

	//
	//HWND hWnd = (HWND)cvGetWindowHandle( "diff_camera" );
	//HDC hDC = GetDC(hWnd);

	while(1)
	{
		Sleep(100);
		// 카메라로부터 입력된 프레임을 잡는다.
		// 만약에 실패할시 에러 메시지를 보여준다.
		if( !cvGrabFrame( capture ) ) 
		{
			printf("프레임을 가져올 수 없습니다! \n");
			break;
		}

		// 프레임 회수를 초기화 한다. (초당 30프레임 기준)
		if( count % 2 == 0 ) count = 0;

		// 프레임 회수를 증가한다.
		count++;

		// 가져온 프레임으로부터 영상 데이터를 얻는다.
		current_image = cvRetrieveFrame( capture );

		// 초당 30프레임 기준의 현재 프레임을 이전의 프레임에 복사한다.
		if( count == 1 )
		{
			// 영상 데이타를 창에서 보여준다.
			// cvShowImage( captureWidow, current_image );

			// 이전의 프레임으로 복사한다.
			previous_image = cvCreateImage( cvGetSize(current_image), 
				IPL_DEPTH_8U,
				current_image->nChannels);

			cvCopy( current_image, previous_image );
		}
		// 이전 프레임과 현재 프레임을 비교한다. 
		else
		{
			diff_image = cvCreateImage( cvGetSize(current_image), IPL_DEPTH_8U, current_image->nChannels);

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
							resion1++;
						}
						else if( 3 * xpart <= x && x <= 4 * xpart && 0 <= y && y <= ypart )
						{
							resion2++;
						}
						else if( 0 <= x && x <= xpart && 3 * ypart <= y && y <= 4 * ypart )
						{
							resion3++;
						}
						else if( 3 * xpart <= x && x <= 4 * xpart && 3 * ypart <= y && y <= 4 * ypart )
						{
							resion4++;
						}
						//int X = x;    
						//int Y = diff_image->height-1-y;    
						//Rectangle(hDC, X-3, Y-3, X+3, Y+3);   
					}  
				}
			}

			// 차연산을 수행한다.
			//cvAbsDiff( current_image, previous_image, diff_image );

			// 차연산 수행한 결과의 영상 데이타를 창에서 보여준다.
			//cvShowImage( diff_captureWidow, diff_image );

			// 평균값을 구한다.
			//avg = cvAvg( diff_image, NULL );

			// 만약에 주어진 값 이상이면 움직임이 있다고 판단하여 파일로 저장한다.
			//printf("1지역 : %d // 2지역 : %d // 3지역 : %d // 4지역 : %d\n", resion1, resion2, resion3, resion4);
			if( resion1 > 10 && resion2 > 10 && resion3 > 10 && resion4 > 10 )
			{
				printf("위험\n");
			}

			//if( avg.val[0] > threshold )
			{
				//...( current_image를 cvSaveImage() 함수로 저장 )....
			}

			// 할당한 메모리 해제
			cvReleaseImage( &diff_image );

			cvReleaseImage( &gray );
			cvReleaseImage( &oldgray );
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
	//cvDestroyWindow( captureWidow );
	//cvDestroyWindow( diff_captureWidow );
}