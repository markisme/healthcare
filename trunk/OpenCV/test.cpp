////
////  OpenCV 를 이용하여 카메라로 영상을 입력 받아 이진화(Binarization)하고
////  테두리(contour)을 찾는 프로그램 입니다.
////
////  자세한 설명이나 기타 문의사항은 아래 사이트를 방문.
////
////  - 2006. 1. 3. Dong-Chul Kim
////    http://www.opencv.co.kr
////    http://www.t9t9.com
////
//
//#include <cv.h>
//#include <highgui.h>
//
//#define Width 640
//#define Height 480
//
//struct stMap{
//
//	unsigned char r;
//	unsigned char g;
//	unsigned char b;
//
//} Map[Height][Width];
//
//
//void Img2Aray(IplImage* img)
//{
//
//	for(int i=0; i<img->height; i++){
//		for(int j=0; j<img->width; j++){
//
//			Map[i][j].r = img->imageData[i*img->widthStep + j*img->nChannels + 2];
//			Map[i][j].g = img->imageData[i*img->widthStep + j*img->nChannels + 1];
//			Map[i][j].b = img->imageData[i*img->widthStep + j*img->nChannels + 0];
//
//		}
//	}
//}
//
//int AutoThreshold(IplImage* img)
//{
//	unsigned char t = 128;
//	unsigned char min = 255, max = 0;
//	unsigned char p;
//
//	for(int i=0; i<img->height; i+=2){
//		for(int j=0; j<img->width; j+=2){
//			p = img->imageData[i*img->widthStep + j ];
//
//			min = ( min > p ) ? p : min;
//			max = ( max < p ) ? p : max;
//
//		}
//	}
//
//	t = (min + max) / 2;
//
//	return t;
//}
//
//IplImage* contour(IplImage* img)
//{
//	int di[8] = {-1,-1,-1, 0, 0, 1, 1, 1},
//		dj[8] = {-1, 0, 1,-1, 1,-1, 0, 1};
//
//	int mask[3][3]={{ -1, -1, -1 },
//	{ -1,  8, -1 },
//	{ -1, -1, -1 }};
//
//	int p;
//	Img2Aray(img);
//
//	for(int i=1; i<img->height-1; i++){
//		for(int j=1; j<img->width-1; j++){
//			p = Map[i][j].r;
//			p *= mask[1][1];
//
//			for(int k=0; k<8; k++){
//				p = p + ( (unsigned char)Map[i+di[k]][j+dj[k]].r * mask[1+di[k]][1+dj[k]] );
//			}
//
//			if(p > 255)  p = 255;
//			else if(p < 0) p = 0;
//
//			img->imageData[i*img->widthStep + j ] = (unsigned char)p;
//		}
//	}
//
//	return img;
//
//}
//
//void main()
//{
//	int i, threshold = 128; // 임계값(Threshold) 설정
//
//	IplImage* img = 0;
//	IplImage* output = 0;
//	IplImage* gray = 0;
//
//	CvCapture* capture = cvCaptureFromCAM(0); // 카메라 연결
//
//	cvNamedWindow( "T9-camera", 1 ); // 원본 영상을 띄울 윈도우
//	cvNamedWindow( "T9-output", 1 ); // 결과 영상을 띄울 윈도우
//
//	while(1) {
//		cvGrabFrame( capture );
//		img = cvRetrieveFrame( capture );
//		cvShowImage( "T9-camera", img );
//
//		if(!output){
//			gray = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1); // 흑백 이미지 생성
//			output = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1); // 출력할 흑백 이미지 생성
//		}
//
//		cvCvtColor(img, gray, CV_RGB2GRAY); // 컬러를 흑백으로 변환
//
//		if( cvWaitKey(10) >= 0 )
//			break;
//
//		threshold = AutoThreshold(gray); // 자동 이진화
//
//		cvThreshold(gray, output, threshold, 255, CV_THRESH_BINARY); // 영상의 각 픽셀(x,y) 값이 threshold 값의 초과는 255 로, 그 이하는 0 으로 변환
//
//		output = contour(output);
//		output->origin = img->origin; // 방향이 뒤집어 진것을 바로 잡아줌
//
//		cvShowImage( "T9-output", output );
//	}
//
//	cvReleaseImage( &gray );
//	cvReleaseImage( &output );
//	cvReleaseCapture( &capture );
//	cvDestroyWindow( "T9-output" );
//	cvDestroyWindow( "T9-camera" );
//}
