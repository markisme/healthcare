#pragma once

#include "resource.h"       // 주 기호입니다.
#include <vector>

class CMainFrame;

class CTestApp : public CWinApp
{
public:
	CTestApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	virtual BOOL OnIdle( LONG lCount );

	void Init();
	void Uninit();
	void ProcPacket();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	void SendAddUserData( int value, float temp );

private:
	CMainFrame* _mainFrm;
	HANDLE hComm;

	//
	BOOL _isStart;
	int _lastMin;

	// 주기 검출 데이터
	int _maxValue;
	int _minValue;
	int _tValue;
	int _overflowCnt;
	float _hz;
	std::vector<float> _hzList;

	// 테스트용
	int _lastSec;
	int _dataCnt;
};

extern CTestApp theApp;