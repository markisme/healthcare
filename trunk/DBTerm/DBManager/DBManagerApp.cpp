// Test.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "DBManagerApp.h"
#include "MainFrm.h"
#include "DBManagerView.h"
#include "LoginDlg.h"
#include <windows.h>

BEGIN_MESSAGE_MAP(CDBManagerApp, CWinApp)
END_MESSAGE_MAP()

CDBManagerApp::CDBManagerApp()
{
}
CDBManagerApp theApp;

BOOL CDBManagerApp::InitInstance()
{
	CWinApp::InitInstance();

	LoginDlg dlg;
	if( dlg.DoModal() != IDOK )
	{
		return -1;
	}

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

	CRect mainRect;
	pMainFrame->GetWindowRect( mainRect );
	pMainFrame->MoveWindow( mainRect.left, mainRect.top, 800, 600 );

	return TRUE;
}

int CDBManagerApp::ExitInstance()
{
	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return TRUE;
}

BOOL CDBManagerApp::OnIdle( LONG lCount )
{
	return TRUE;
}