// Test.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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

	// ȣ��Ʈ�� Ŭ���̾�Ʈ ���� (���� ������ ��� ����)
	if( id == "naid" )
	{
		isHost = true;
	}

	// ��Ʈ��ũ �ʱ�ȭ
	Network::GetInstance().Init( isHost, clientPort, ip, serverPort );
	
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