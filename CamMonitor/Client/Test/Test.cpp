// Test.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "Network.h"
#include "LoginDlg.h"
#include "OpenCV.h"
#include "SoundMixer.h"
#include "ScreenSaver.h"

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
END_MESSAGE_MAP()

CTestApp::CTestApp() :
_rePassCount( 0 ),
_isMonitorMode( false )
{
}

CTestApp theApp;

BOOL CTestApp::InitInstance()
{
	CWinApp::InitInstance();

	// ���� ����
	int clientPort = 200;
	//std::string ip = "211.189.20.246";
	std::string ip = "211.189.19.160";
	int serverPort = 10000;

	// ��Ʈ��ũ �ʱ�ȭ
	Network::GetInstance().Init( clientPort, ip, serverPort );

	// ��ũ�� ���̹� ���� �ʱ�ȭ
	ScreenSaver::GetInstance().Init();

	// ��ķ ����
	_openCV = new OpenCV;
	_openCV->Init();

	// ���� ����
	_soundMixer = new SoundMixer;
	_soundMixer->Init();

	_soundMixer->SetMute( FALSE );
	_soundMixer->SetVolumn( 6000 );

	// ���α׷� ���� �α���
	_dlg = new LoginDlg;
	//if( _dlg->DoModal() != IDOK )
	//{
	//	AfxMessageBox("���α׷��� ���� �մϴ�.");
	//	return -1;
	//}

	//if(  Network::GetInstance()._isSuccessAuth == false )
	//{
	//	AfxMessageBox("��й�ȣ�� Ʋ�Ƚ��ϴ�.");
	//	return -1;
	//}

	// �� MDI ������ â�� ����ϴ�.
	m_pMainWnd = new CMainFrame;
	if (!m_pMainWnd || !((CMainFrame*)m_pMainWnd)->LoadFrame(IDR_MAINFRAME))
	{
		delete m_pMainWnd;
		return FALSE;
	}

	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	CRect mainRect;
	m_pMainWnd->GetWindowRect( mainRect );
	m_pMainWnd->MoveWindow( 0, 0, 0, 0 );
	m_pMainWnd->ShowWindow( FALSE );

	// ķ ���� ��� �۵�
	CWinThread * pThread1 = AfxBeginThread(MonitorWebCamThreadFunction, this);

	// �Ŀ� ���� ��� �۵�
	//CWinThread * pThread2 = AfxBeginThread(MonitorPowerThreadFunction, this);

	// ��Ʈ��ũ ���� ��� �۵�
	//CWinThread * pThread3 = AfxBeginThread(MonitorNetworkThreadFunction, this);

	// ����� ��� ����
	_isMonitorMode = true;

	return TRUE;
}

int CTestApp::ExitInstance()
{
	// ����� ��� �� ���α׷� ����
	if( _isMonitorMode )
	{
		// ���ȸ�� �۵�
		PlayAlertSound();

		//
		AfxMessageBox("���� ���α׷� ����!\n���� ��� �۵�!");

		//
		Sleep(9999999);
	}

	_openCV->Uninit();
	delete( _openCV );

	_soundMixer->Uninit();
	delete( _soundMixer );

	//
	ScreenSaver::GetInstance().Uninit();

	//
	Network::GetInstance().Uninit();

	//
	delete _dlg;
	_dlg = NULL;

	//
	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return TRUE;
}

BOOL CTestApp::OnIdle( LONG lCount )
{
	if( _rePassCount == 0 )
	{
#ifndef TEST
		// ��ũ�����̹� ����
		m_pMainWnd->ShowWindow( TRUE );
		HWND hwnd = GetActiveWindow();
		ScreenSaver::GetInstance().StartScreenSaver( hwnd );
		m_pMainWnd->ShowWindow( FALSE );
#endif
	}

	//
	if( _dlg->DoModal() == IDOK )
	{
		if( Network::GetInstance()._isSuccessAuth == true )
		{
			// ���α׷� ����
			AfxMessageBox("���� ��� ����");
			_isMonitorMode = false;
			PostQuitMessage(0);
		}
		else
		{
			AfxMessageBox("��й�ȣ�� Ʋ�Ƚ��ϴ�.");
			_rePassCount++;
		}
	}

	//
	if( _rePassCount >= 3 )
	{
		// ���ȸ�� �۵�
		PlayAlertSound();

		//
		AfxMessageBox("��й�ȣ 3ȸ ����!\n���� ��� �۵�!");
	}

	//
	Network::GetInstance().ProcPacket();

	return TRUE;
}

UINT CTestApp::MonitorWebCamThreadFunction(LPVOID pParam)
{
	CTestApp *pthis = (CTestApp*)pParam;     
	pthis->MonitorWebCamThreadDo(); // ���ø� ���� ������ ����
	return 0;
}

void CTestApp::MonitorWebCamThreadDo()
{
	// ���� ��� ����
	_openCV->StartMonitor();

	// ��ũ�� ���̹� ���̱�
#ifndef TEST
	ScreenSaver::GetInstance().KillScreenSaver();
#endif

	// ���ȸ�� �۵�
	PlayAlertSound();
	
	//
	AfxMessageBox("��ķ ����!\n���� ��� �۵�!");
}

UINT CTestApp::MonitorPowerThreadFunction(LPVOID pParam)
{
	CTestApp *pthis = (CTestApp*)pParam;     
	pthis->MonitorPowerThreadDo(); // ���ø� ���� ������ ����
	return 0;
}

void CTestApp::MonitorPowerThreadDo()
{
	// ���� ��� ����
	while( true )
	{
		//
		Sleep(1000);
		
		//
		SYSTEM_BATTERY_STATE sys_bat_state;
		CallNtPowerInformation(SystemBatteryState, NULL, 0,	&sys_bat_state, sizeof(sys_bat_state));

		//
		if( sys_bat_state.AcOnLine == false )
		{
			break;
		}
	}

	// ���ȸ�� �۵�
	PlayAlertSound();

	//
	AfxMessageBox("���͸� ��� ����!\n���� ��� �۵�!");
}

UINT CTestApp::MonitorNetworkThreadFunction(LPVOID pParam)
{
	CTestApp *pthis = (CTestApp*)pParam;     
	pthis->MonitorNetworkThreadDo(); // ���ø� ���� ������ ����
	return 0;
}

void CTestApp::MonitorNetworkThreadDo()
{
	// ���� ��� ����
	while( true )
	{
		//
		Sleep(1000);
		
		//
		if( Network::GetInstance()._isConnecting == 0 )
		{
			break;
		}
	}

	// ���ȸ�� �۵�
	PlayAlertSound();

	//
	AfxMessageBox("��Ʈ��ũ ���� ����!\n���� ��� �۵�!");
}


void CTestApp::PlayAlertSound()
{
#ifndef TEST
	std::string wav = "Test.wav";
	PlaySound(wav.c_str(),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
#endif
}
