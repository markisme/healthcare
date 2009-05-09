#include <cv.h>
#include <highgui.h>
#include <cvcam.h>
#include <stdio.h>
#include "mmsystem.h"
#include <vector>

struct RegionRect
{
	RegionRect( int partNo, int lefttopX, int lefttopY, int rightbottomX, int rightbottomY ) : _partNo( partNo ),
		_lefttopX(lefttopX), _lefttopY(lefttopY), _rightbottomX(rightbottomX), _rightbottomY(rightbottomY),
		_left(false), _right(false), _top(false), _bottom(false) { _checkCount = 0; };
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

	// 주변 파트 정보 표시
	void SetLeft( bool b ) { _left = b; }
	void SetRight( bool b ) { _right = b; }
	void SetTop( bool b ) { _top = b; }
	void SetBottom( bool b ) { _bottom = b; }

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

	// 주변 파트 정보
	int _left;
	int _right;
	int _top;
	int _bottom;
};

typedef std::vector<RegionRect> RegionList;

enum CamState
{
	NONE_MOVE,
	OBJECT_MOVE,
	CAM_MOVE
};

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
	CamState CompareImage( IplImage* current_image, IplImage* previous_image );
	void InitPart( IplImage* current_image );
	void ComparePart( IplImage* gray );

private:
	bool _startMonitor;
	bool _alert;
	RegionList _regionList;
};