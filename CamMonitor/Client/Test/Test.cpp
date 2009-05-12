// Test.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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

	// 서버 설정
	int clientPort = 200;
	//std::string ip = "211.189.20.246";
	std::string ip = "211.189.19.160";
	int serverPort = 10000;

	// 네트워크 초기화
	Network::GetInstance().Init( clientPort, ip, serverPort );

	// 스크린 세이버 설정 초기화
	ScreenSaver::GetInstance().Init();

	ConfigDlg dlg;
	if( dlg.DoModal() != IDOK )
	{
		PostQuitMessage(0);
		return FALSE;
	}

	// 사운드 관련
	_soundMixer = new SoundMixer;
	_soundMixer->Init();

	// 주 MDI 프레임 창을 만듭니다.
	m_pMainWnd = new CMainFrame;
	if (!m_pMainWnd || !((CMainFrame*)m_pMainWnd)->LoadFrame(IDR_MAINFRAME))
	{
		delete m_pMainWnd;
		return FALSE;
	}

	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	CRect mainRect;
	m_pMainWnd->GetWindowRect( mainRect );
	m_pMainWnd->MoveWindow( 0, 0, 0, 0 );
	m_pMainWnd->ShowWindow( FALSE );

	//Config::GetInstance()._isWebcamMode
	//Config::GetInstance()._isACPowerMode
	//Config::GetInstance()._isUSBMouseMode

	// 웹캠 관련
	if( Config::GetInstance()._isWebcamMode )
	{
		_openCV = new OpenCV;
		_openCV->Init();
	}

	// 모니터 모드 시작
	_isMonitorMode = true;

	// 스크린세이버 동작
	m_pMainWnd->ShowWindow( TRUE );
	HWND hwnd = (HWND)m_pMainWnd->GetActiveWindow();
	ScreenSaver::GetInstance().StartScreenSaver(hwnd);
	m_pMainWnd->ShowWindow( FALSE );

	return TRUE;
}

int CTestApp::ExitInstance()
{
	// 모니터 모드 중 프로그램 종료
	if( _isMonitorMode )
	{
		// 보안모드 작동
		OperatorMonitor( "강제 프로그램 종료!\n보안 모드 작동!" );
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
		// 보안모드 작동
		OperatorMonitor( "비밀번호 3회 오류!\n보안 모드 작동!" );
	}

	// 패킷 처리
	if( Network::GetInstance().ProcPacket() == TRUE )
	{
		if( Network::GetInstance()._isSuccessAuth == 1 )
		{
			// 보안 모드 해제
			ReleaseMonitor();
		}
		else if( Network::GetInstance()._isSuccessAuth == 0 )
		{
			AfxMessageBox("비밀번호가 틀렸습니다.");
			_rePassCount++;
		}

		//
		if( Network::GetInstance()._isConnecting == 0 )
		{
#ifndef TEST
			// 스크린 세이버 죽이기
			ScreenSaver::GetInstance().KillScreenSaver();
#endif
			// 보안모드 작동
			OperatorMonitor( "네트워크 연결 끊김!\n보안 모드 작동!" );
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
		// 스크린 세이버 죽이기
		ScreenSaver::GetInstance().KillScreenSaver();
#endif
		if( _openCV->GetAlert() )
		{
			// 보안모드 작동
			OperatorMonitor( "웹캠 동작!\n보안 모드 작동!" );
		}
		else
		{
			// 보안 모드 해제
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
		// 스크린 세이버 죽이기
		ScreenSaver::GetInstance().KillScreenSaver();
#endif
		// 보안모드 작동
		OperatorMonitor( "베터리 모드 동작!\n보안 모드 작동!" );
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
	AfxMessageBox("보안 모드 해제");
	_isMonitorMode = false;
	PostQuitMessage(0);
}