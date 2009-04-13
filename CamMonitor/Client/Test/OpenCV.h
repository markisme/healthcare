#include <cv.h>
#include <highgui.h>
#include <cvcam.h>
#include <stdio.h>
#include "mmsystem.h"
#include <vector>

struct ResionRect
{
	ResionRect( int lefttopX, int lefttopY, int rightbottomX, int rightbottomY ) : 
		_lefttopX(lefttopX), _lefttopY(lefttopY), _rightbottomX(rightbottomX), _rightbottomY(rightbottomY) { _checkCount = 0; };
	~ResionRect() {};

	void CheckResion( int x, int y)
	{
		if( _lefttopX <= x && x < _rightbottomX && 
			_lefttopY <= y && y < _rightbottomY )
		{
			_checkCount++;
		}
	}
	
	int _checkCount;
	int _lefttopX;
	int _lefttopY;
	int _rightbottomX;
	int _rightbottomY;
};

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

private:
	bool _startMonitor;
	bool _alert;
};