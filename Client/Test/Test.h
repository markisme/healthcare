#pragma once

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include <assert.h>
//#include <cstdio>
//#include <cstring>
//#include <stdlib.h>
//#include <windows.h> // Sleep

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
	void Update();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	CMainFrame* _mainFrm;
	RakPeerInterface * _client;
};

extern CTestApp theApp;