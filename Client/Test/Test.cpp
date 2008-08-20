// Test.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "TestView.h"
#include "Network.h"
#include "LoginDlg.h"
#include <windows.h>

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

	{
		hComm = CreateFile(L"COM2", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if(hComm == INVALID_HANDLE_VALUE)
			return -1;

		//* 포트 설정 얻고 수정 하여 반영하기
		DCB dcb;
		
		// 설정 얻기
		if( !GetCommState(hComm, &dcb) )
			return -1;
		
		// 설정 변경
		dcb.BaudRate = CBR_9600;
		dcb.fParity = FALSE;
		dcb.fNull = FALSE;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		
		// 설정 반영
		if( !SetCommState(hComm, &dcb) )
			return -1;

		//*   타임아웃 설정
		COMMTIMEOUTS timeouts;
		timeouts.ReadIntervalTimeout = 0;
		timeouts.ReadTotalTimeoutMultiplier = 0;
		timeouts.ReadTotalTimeoutConstant = 0;
		timeouts.WriteTotalTimeoutMultiplier = 0;
		timeouts.WriteTotalTimeoutConstant = 0;
		if( !SetCommTimeouts(hComm, &timeouts) )
			return -1;
	}

	return TRUE;
}

int CTestApp::ExitInstance()
{
	Network::GetInstance().Uninit();

	//* 포트 닫기
	CloseHandle(hComm);

	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return TRUE;
}

BOOL CTestApp::OnIdle( LONG lCount )
{
	Network::GetInstance().ProcPacket();

	//*  데이터 통신 (수신과 송신)
	unsigned char buf[350];   // 통신에 사용될 버퍼
	DWORD byteRead, byteWritten;
	int retval;

	DataList & dataList = Network::GetInstance().GetDataList();
	dataList.clear();

	// 데이터를 먼저 받아야 하는경우 수신부터 구현
	retval = ReadFile(hComm, buf, 350, &byteRead, NULL);
	if( retval )
	{
		for( int num = 0; num < 350; num++ )
		{
			unsigned char ch = buf[ num ];
			int y = (int)ch;

			dataList.push_back( PacketData(0,y) );
		}
	}

	_mainFrm->GetMainDlg().GetView()->Refresh();
	return TRUE;
}