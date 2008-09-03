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

	// ��Ʈ��ũ �ʱ�ȭ
	Network::GetInstance().Init( clientPort, ip, serverPort );

	LoginDlg dlg;
	dlg.DoModal();

	// ȣ��Ʈ�� ��� ���� ����Ʈ ��û
	if( Network::GetInstance()._isHost )
	{
		Network::GetInstance().ReqGetUserInfoSend();
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
	if( Network::GetInstance().ProcPacket() )
	{
		_mainFrm->GetMainDlg().RefreshControls();
	}

	if( Network::GetInstance()._isHost == false )
	{
		// ������ ��� (���Ű� �۽�)
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

		// �����͸� ���� �޾ƾ� �ϴ� ��� ���ź��� ����
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
					int y = 195-(atoi(str.c_str())/1.f)+200;
					if ( y > 180 ) y = Temp_y;
					dataList.push_back( PacketData(0,y) );
					str.clear();
					if( y < 180) Temp_y = y;
				}
			}
		}

		// ������ ����
		Network::GetInstance().ReqClientDataSend();

		// �ֱ� ������ ����(1�п� �ѹ���)
		SendAddUserData( 80, 36.5 );
	}

	_mainFrm->GetMainDlg().GetView()->Refresh();
	return TRUE;
}

void CTestApp::SendAddUserData( int value, float temp )
{
	CTime t = CTime::GetCurrentTime();

	UserData userData; char buf[1024];
	userData._year = itoa( t.GetYear(), buf, 10 );

	if( t.GetMonth() < 10 )
	{
		userData._month = "0";
		userData._month += itoa( t.GetMonth(), buf, 10 );
	}
	else
	{
		userData._month = itoa( t.GetMonth(), buf, 10 );
	}

	if( t.GetDay() < 10 )
	{
		userData._day = "0";
		userData._day += itoa( t.GetDay(), buf, 10 );
	}
	else
	{
		userData._day = itoa( t.GetDay(), buf, 10 );
	}

	if( t.GetHour() < 10 )
	{
		userData._hour = "0";
		userData._hour += itoa( t.GetHour(), buf, 10 );
	}
	else
	{
		userData._hour = itoa( t.GetHour(), buf, 10 );
	}

	if( t.GetMinute() < 10 )
	{
		userData._min = "0";
		userData._min += itoa( t.GetMinute(), buf, 10 );
	}
	else
	{
		userData._min = itoa( t.GetMinute(), buf, 10 );
	}

	userData._value = itoa( value, buf, 10 );
	userData._temp = gcvt( temp, 10, buf );

	Network::GetInstance().ReqAddUserDataSend( 1, userData );
}