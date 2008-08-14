// Test.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "TestView.h"
#include "Network.h"

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
END_MESSAGE_MAP()

CTestApp::CTestApp()
{
}

CTestApp theApp;

BOOL CTestApp::InitInstance()
{
	CWinApp::InitInstance();
	
	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}

	//
	_mainFrm = pMainFrame;
	m_pMainWnd = pMainFrame;

	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	Network::GetInstance().Init();

	return TRUE;
}

int CTestApp::ExitInstance()
{
	Network::GetInstance().Uninit();

	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return TRUE;
}

BOOL CTestApp::OnIdle( LONG lCount )
{
	Network::GetInstance().ProcPacket();
	return TRUE;
}