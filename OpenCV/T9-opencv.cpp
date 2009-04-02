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

/*

void callback(IplImage* image);

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
	int *nCam = NULL;
	//int nSelected = cvcamSelectCamera(&nCam); // 카메라 선택 메뉴 띄우기
	int nSelected = cvcamGetCamerasCount(); // 자동선택 버전

	cvcamSetProperty(0, CVCAM_PROP_ENABLE, CVCAMTRUE);
	cvcamSetProperty(0, CVCAM_PROP_RENDER, CVCAMTRUE); 

	//cvcamGetProperty(*nCam, CVCAM_VIDEOFORMAT, NULL); // 해상도, 프레임 수 선택
	//cvcamGetProperty(*nCam, CVCAM_CAMERAPROPS, NULL); // 기타 셋업, 밝기, 화이트 밸런스

	w = 640;
	h = 480;
	
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

void callback(IplImage* img)
{
}
*/

int main (int argc, char **argv)
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
	if (argc == 1 || (argc == 2 && strlen (argv[1]) == 1 && isdigit (argv[1][0])))    
		capture = cvCreateCameraCapture (argc == 2 ? argv[1][0] - '0' : 0);  

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
