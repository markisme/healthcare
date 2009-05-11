#include <cv.h>
#include <highgui.h>
#include <cvcam.h>
#include <stdio.h>
#include "mmsystem.h"
#include <vector>

struct RegionRect
{
	RegionRect( int partNo, int lefttopX, int lefttopY, int rightbottomX, int rightbottomY ) : _partNo( partNo ),
		_lefttopX(lefttopX), _lefttopY(lefttopY), _rightbottomX(rightbottomX), _rightbottomY(rightbottomY)
		{ _checkCount = 0; };
	~RegionRect() {};

	// 자신의 영역인지 체크
	void CheckRegion( int x, int y)
	{
		if( _lefttopX <= x && x < _rightbottomX && 
			_lefttopY <= y && y < _rightbottomY )
		{
			_checkCount++;
		}
	}

	// 자신의 영역 체크 지우기
	void ClearCheck() { _checkCount = 0; };

	// 자신의 영역 그리기
	void DrawRegion( HDC hdc )
	{
		HBRUSH MyBrush=CreateHatchBrush(HS_DIAGCROSS,RGB(255,255,255));
		HBRUSH OldBrush=(HBRUSH)SelectObject(hdc,MyBrush);

		SetBkMode(hdc,TRANSPARENT);
		Rectangle(hdc,_lefttopX, _lefttopY,_rightbottomX,_rightbottomY);
	}

	// 기본 정보
	int _partNo;
	int _checkCount;

	// 좌표 값
	int _lefttopX;
	int _lefttopY;
	int _rightbottomX;
	int _rightbottomY;
};

typedef std::vector<RegionRect> RegionList;

class OpenCV
{
public:
	OpenCV();
	~OpenCV();

	void Init();
	void Uninit();

	void StartMonitor();
	void StopMonitor();
	bool GetAlert() { return _alert; }

private:
	bool IsMoveCam( IplImage* current_image, IplImage* previous_image );
	void InitPart( IplImage* current_image );
	void ComparePart( IplImage* gray );
	void CurrentFrame(IplImage* frame);

private:
	bool _alert;
	RegionList _regionList;

	//
	int   m_bBeingTracked; // Flag for traking object.

	CvPoint m_cvPtOrigin;
	CvRect  m_cvRtSelected; // Zone Selected.
	CvRect	m_cvRtTracked; // Zone Tracked.
	
	CvBox2D	m_cvTrackBox;	// 2D Track Box
	CvConnectedComp m_cvTrackComp;	//Track Connected Comp
	CvHistogram* m_cvHist;	// Histogram

	IplImage* m_iplImage;
	IplImage* m_iplHsv;
	IplImage* m_iplHue;
	IplImage* m_iplMask;
	IplImage* m_iplBackproject;
	IplImage* m_iplHist;

	int m_iSMin;
	int m_iVMin;
	int m_iVMax;
};