// Test.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "TestView.h"
#include "Network.h"
#include "LoginDlg.h"

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
END_MESSAGE_MAP()

CTestApp::CTestApp()
{
}

CTestApp theApp;

BOOL CTestApp::InitInstance()
{
	CWinApp::InitInstance();

	bool isHost = false;
	std::string id;
	std::string pw;
	int clientPort = 100;
	std::string ip = "192.168.0.3";
	int serverPort = 10000;

	LoginDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		id = dlg.GetID();
		pw = dlg.GetPW();
		clientPort = atoi( dlg.GetCPort().c_str() );
		ip = dlg.GetIP();
		serverPort = atoi( dlg.GetPort().c_str() );
	}

	// 호스트와 클라이언트 선정 (차후 서버로 기능 이전)
	if( id == "naid" )
	{
		isHost = true;
	}

	// 네트워크 초기화
	Network::GetInstance().Init( isHost, clientPort, ip, serverPort );
	
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

	if( Network::GetInstance()._isHost )
	{
		pMainFrame->MoveWindow( mainRect.left, mainRect.top, 720, 410 );
	}
	else
	{
		pMainFrame->MoveWindow( mainRect.left, mainRect.top, 370, 410 );
	}

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