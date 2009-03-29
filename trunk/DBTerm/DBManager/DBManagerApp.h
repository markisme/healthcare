#pragma once

#include "resource.h"       // 주 기호입니다.
#include <vector>

class CMainFrame;

class CDBManagerApp : public CWinApp
{
public:
	CDBManagerApp();

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

private:
	CMainFrame* _mainFrm;
};

extern CDBManagerApp theApp;