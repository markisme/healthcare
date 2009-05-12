// Test.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "Network.h"
#include "OpenCV.h"
#include "SoundMixer.h"
#include "ScreenSaver.h"
#include "ConfigDlg.h"
#include "Config.h"

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

	ConfigDlg dlg;
	if( dlg.DoModal() != IDOK )
	{
		PostQuitMessage(0);
		return FALSE;
	}

	// ���� ����
	_soundMixer = new SoundMixer;
	_soundMixer->Init();

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

	//Config::GetInstance()._isWebcamMode
	//Config::GetInstance()._isACPowerMode
	//Config::GetInstance()._isUSBMouseMode

	// ��ķ ����
	if( Config::GetInstance()._isWebcamMode )
	{
		_openCV = new OpenCV;
		_openCV->Init();
	}

	// ����� ��� ����
	_isMonitorMode = true;

	// ��ũ�����̹� ����
	m_pMainWnd->ShowWindow( TRUE );
	HWND hwnd = (HWND)m_pMainWnd->GetActiveWindow();
	ScreenSaver::GetInstance().StartScreenSaver(hwnd);
	m_pMainWnd->ShowWindow( FALSE );

	return TRUE;
}

int CTestApp::ExitInstance()
{
	// ����� ��� �� ���α׷� ����
	if( _isMonitorMode )
	{
		// ���ȸ�� �۵�
		OperatorMonitor( "���� ���α׷� ����!\n���� ��� �۵�!" );
		Sleep(9999999);
	}

	if( Config::GetInstance()._isWebcamMode )
	{
		_openCV->Uninit();
		delete( _openCV );
	}

	_soundMixer->Uninit();
	delete( _soundMixer );

	//
	ScreenSaver::GetInstance().Uninit();

	//
	Network::GetInstance().Uninit();

	//
	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return TRUE;
}

BOOL CTestApp::OnIdle( LONG lCount )
{
	//
	if( _rePassCount >= 3 )
	{
		// ���ȸ�� �۵�
		OperatorMonitor( "��й�ȣ 3ȸ ����!\n���� ��� �۵�!" );
	}

	// ��Ŷ ó��
	if( Network::GetInstance().ProcPacket() == TRUE )
	{
		if( Network::GetInstance()._isSuccessAuth == 1 )
		{
			// ���� ��� ����
			ReleaseMonitor();
		}
		else if( Network::GetInstance()._isSuccessAuth == 0 )
		{
			AfxMessageBox("��й�ȣ�� Ʋ�Ƚ��ϴ�.");
			_rePassCount++;
		}

		//
		if( Network::GetInstance()._isConnecting == 0 )
		{
#ifndef TEST
			// ��ũ�� ���̹� ���̱�
			ScreenSaver::GetInstance().KillScreenSaver();
#endif
			// ���ȸ�� �۵�
			OperatorMonitor( "��Ʈ��ũ ���� ����!\n���� ��� �۵�!" );
		}
	}

	if( Config::GetInstance()._isWebcamMode )
	{
		OpenCVUpdate();
	}

	if( Config::GetInstance()._isACPowerMode )
	{
		ACPowerUpdate();
	}

	return TRUE;
}

void CTestApp::OpenCVUpdate()
{
	if( _openCV->UpdateMonitor() == FALSE )
	{
#ifndef TEST
		// ��ũ�� ���̹� ���̱�
		ScreenSaver::GetInstance().KillScreenSaver();
#endif
		if( _openCV->GetAlert() )
		{
			// ���ȸ�� �۵�
			OperatorMonitor( "��ķ ����!\n���� ��� �۵�!" );
		}
		else
		{
			// ���� ��� ����
			ReleaseMonitor();
		}
	}
}

void CTestApp::ACPowerUpdate()
{
	//
	SYSTEM_BATTERY_STATE sys_bat_state;
	CallNtPowerInformation(SystemBatteryState, NULL, 0,	&sys_bat_state, sizeof(sys_bat_state));

	//
	if( sys_bat_state.AcOnLine == false )
	{
#ifndef TEST
		// ��ũ�� ���̹� ���̱�
		ScreenSaver::GetInstance().KillScreenSaver();
#endif
		// ���ȸ�� �۵�
		OperatorMonitor( "���͸� ��� ����!\n���� ��� �۵�!" );
	}
}

void CTestApp::OperatorMonitor( std::string text )
{
#ifndef TEST
	_soundMixer->SetMute( FALSE );
	_soundMixer->SetVolumn( 6000 );

	std::string wav = "Test.wav";
	PlaySound(wav.c_str(),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
#endif
	AfxMessageBox( text.c_str() );
}

void CTestApp::ReleaseMonitor()
{
	AfxMessageBox("���� ��� ����");
	_isMonitorMode = false;
	PostQuitMessage(0);
}