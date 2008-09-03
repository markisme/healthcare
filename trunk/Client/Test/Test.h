#pragma once

#include "resource.h"       // 주 기호입니다.

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

	BOOL _isStart;
};

extern CTestApp theApp;