#include <cv.h>
#include <highgui.h>
#include <cvcam.h>
#include <stdio.h>
#include "mmsystem.h"
#include <vector>

struct ResionCount
{
	ResionCount() : _resionLeftTop(0), _resionRightTop(0), _resionLeftBottom(0), _resionRightBottom(0) {};
	~ResionCount() {};
	int _resionLeftTop;
	int _resionRightTop;
	int _resionLeftBottom;
	int _resionRightBottom;
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