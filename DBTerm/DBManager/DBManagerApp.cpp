// Test.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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

	// �� MDI ������ â�� ����ϴ�.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}

	//
	_mainFrm = pMainFrame;
	m_pMainWnd = pMainFrame;

	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
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