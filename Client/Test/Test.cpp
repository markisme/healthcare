// Test.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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
	std::string ip = "211.189.19.160";
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
	if( id == "host" )
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

	if( isHost == false )
	{
		hComm = CreateFile(L"COM2", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if(hComm == INVALID_HANDLE_VALUE)
			return -1;

		//* ��Ʈ ���� ��� ���� �Ͽ� �ݿ��ϱ�
		DCB dcb;
		
		// ���� ���
		if( !GetCommState(hComm, &dcb) )
			return -1;
		
		// ���� ����
		dcb.BaudRate = CBR_9600;
		dcb.fParity = FALSE;
		dcb.fNull = FALSE;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		
		// ���� �ݿ�
		if( !SetCommState(hComm, &dcb) )
			return -1;

		//*   Ÿ�Ӿƿ� ����
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

	//* ��Ʈ �ݱ�
	CloseHandle(hComm);

	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return TRUE;
}

BOOL CTestApp::OnIdle( LONG lCount )
{
	Network::GetInstance().ProcPacket();

	if( Network::GetInstance()._isHost == false )
	{
		//*  ������ ��� (���Ű� �۽�)
		unsigned char buf[31];   // ��ſ� ���� ����
		DWORD byteRead, byteWritten;
		int retval;

		DataList & dataList = Network::GetInstance().GetDataList();

		int count = dataList.size();
		if( count >= 350 )
		{
			DataList::iterator it = dataList.begin();
			dataList.erase( it, it + 5 );
		}
//TM
		// �����͸� ���� �޾ƾ� �ϴ°�� ���ź��� ����
		retval = ReadFile(hComm, buf, 30, &byteRead, NULL);
		if( retval )
		{
			std::string str;
			int Temp_y=0;
			for( int num = 0; num < 30; num++ )
			{
				unsigned char ch = buf[ num ];
				str += ch;
				
				if( ch == ' ' )
				{
					//int y = atoi(str.c_str())/5.f;
					//int y = (1024-atoi(str.c_str()))/5.f;
					int y = 195-(atoi(str.c_str())/1.f)+200;
					if ( y > 180 ) y = Temp_y;
					dataList.push_back( PacketData(0,y) );
					str.clear();
					if( y < 180) Temp_y = y;
				}
			}
		}

		Network::GetInstance().Send();
	}

	_mainFrm->GetMainDlg().GetView()->Refresh();
	return TRUE;
}