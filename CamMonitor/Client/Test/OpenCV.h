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

	// �ڽ��� �������� üũ
	void CheckRegion( int x, int y)
	{
		if( _lefttopX <= x && x < _rightbottomX && 
			_lefttopY <= y && y < _rightbottomY )
		{
			_checkCount++;
		}
	}

	void ClearCheck() { _checkCount = 0; };

	// �ڽ��� ���� �׸���
	void DrawRegion( HDC hdc )
	{
		HBRUSH MyBrush=CreateHatchBrush(HS_DIAGCROSS,RGB(255,255,255));
		HBRUSH OldBrush=(HBRUSH)SelectObject(hdc,MyBrush);

		SetBkMode(hdc,TRANSPARENT);
		Rectangle(hdc,_lefttopX, _lefttopY,_rightbottomX,_rightbottomY);
	}

	// �⺻ ����
	int _partNo;
	int _checkCount;

	// ��ǥ ��
	int _lefttopX;
	int _lefttopY;
	int _rightbottomX;
	int _rightbottomY;
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
	bool _init;
	RegionList _regionList;
};