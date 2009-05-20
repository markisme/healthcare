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
_state( NONE_STATE )
{
}

CTestApp theApp;

BOOL CTestApp::InitInstance()
{
	CWinApp::InitInstance();

	// 네트워크 초기화
	Network::GetInstance().Init();
	Sleep(100);

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

	// 웹캠 관련
	if( Config::GetInstance()._isWebcamMode )
	{
		_openCV = new OpenCV;
		_openCV->Init();
	}

	// 모니터 모드 시작
	_state = MONITOR_STATE;

	// 스크린세이버 동작
	ScreenSaver::GetInstance().StartScreenSaver();
	Sleep(1000);

	return TRUE;
}

int CTestApp::ExitInstance()
{
	// 비 정상 프로그램 종료
	if( _state != EXIT_STATE )
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
		//
		if( Network::GetInstance()._isConnecting == 0 )
		{
#ifndef TEST
			// 스크린 세이버 죽이기
			ScreenSaver::GetInstance().KillScreenSaver();
#endif
			// 보안모드 작동
			OperatorMonitor( "네트워크 연결 끊김!\n보안 모드 작동!" );
			return TRUE;
		}

		if( Network::GetInstance()._isSuccessAuth == 1 )
		{
			// 보안 모드 해제
			ReleaseMonitor();
			return TRUE;
		}
		else if( Network::GetInstance()._isSuccessAuth == 0 )
		{
			AfxMessageBox("비밀번호가 틀렸습니다.");
			_rePassCount++;
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

	if( Config::GetInstance()._isUSBMouseMode )
	{
		USBMouseUpdate();
	}

	if( Config::GetInstance()._isScreenSaverMode )
	{
		ScreenSaverUpdate();
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

void CTestApp::USBMouseUpdate()
{
	CMainFrame * mainWnd = (CMainFrame*)m_pMainWnd;
	if( mainWnd->IsDeviceChange() == TRUE )
	{
#ifndef TEST
		// 스크린 세이버 죽이기
		ScreenSaver::GetInstance().KillScreenSaver();
#endif
		// 보안모드 작동
		OperatorMonitor( "USB 장치 해제!\n보안 모드 작동!" );
	}
}

void CTestApp::ScreenSaverUpdate()
{
	if( ScreenSaver::GetInstance().IsScreenSaverRunning() == FALSE )
	{
		// 보안모드 작동
		OperatorMonitor( "스크린 세이버 모드 동작!\n보안 모드 작동!" );
	}
}

void CTestApp::OperatorMonitor( std::string text )
{
	if( _state == ALERT_STATE || _state == EXIT_STATE )
	{
		return;
	}

	// 서버와 접속 해제
	Network::GetInstance().Disconnect();

#ifndef TEST
	_soundMixer->SetMute( FALSE );
	_soundMixer->SetVolumn( 600 );

	std::string wav = "Test.wav";
	PlaySound(wav.c_str(),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
#endif
	_state = ALERT_STATE;
	AfxMessageBox( text.c_str() );
}

void CTestApp::ReleaseMonitor()
{
	_state = EXIT_STATE;
	AfxMessageBox("보안 모드 해제");
	PostQuitMessage(0);
}