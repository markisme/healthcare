#pragma once

#include "resource.h"       // �� ��ȣ�Դϴ�.
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

	// �ֱ� ���� ������
	int _maxValue;
	int _minValue;
	int _tValue;
	int _overflowCnt;
	float _hz;
	std::vector<float> _hzList;

	// �׽�Ʈ��
	int _lastSec;
	int _dataCnt;
};

extern CTestApp theApp;