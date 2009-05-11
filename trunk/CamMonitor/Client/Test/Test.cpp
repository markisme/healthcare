// Test.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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

	// 서버 설정
	int clientPort = 200;
	//std::string ip = "211.189.20.246";
	std::string ip = "211.189.19.160";
	int serverPort = 10000;

	// 네트워크 초기화
	Network::GetInstance().Init( clientPort, ip, serverPort );

	// 스크린 세이버 설정 초기화
	ScreenSaver::GetInstance().Init();

	// 웹캠 관련
	_openCV = new OpenCV;
	_openCV->Init();

	// 사운드 관련
	_soundMixer = new SoundMixer;
	_soundMixer->Init();

	_soundMixer->SetMute( FALSE );
	_soundMixer->SetVolumn( 6000 );

	// 프로그램 동작 로그인
	_dlg = new LoginDlg;
	//if( _dlg->DoModal() != IDOK )
	//{
	//	AfxMessageBox("프로그램을 종료 합니다.");
	//	return -1;
	//}

	//if(  Network::GetInstance()._isSuccessAuth == false )
	//{
	//	AfxMessageBox("비밀번호가 틀렸습니다.");
	//	return -1;
	//}

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

	// 캠 제어 모듈 작동
	CWinThread * pThread1 = AfxBeginThread(MonitorWebCamThreadFunction, this);

	// 파워 제어 모듈 작동
	//CWinThread * pThread2 = AfxBeginThread(MonitorPowerThreadFunction, this);

	// 네트워크 제어 모듈 작동
	//CWinThread * pThread3 = AfxBeginThread(MonitorNetworkThreadFunction, this);

	// 모니터 모드 시작
	_isMonitorMode = true;

	return TRUE;
}

int CTestApp::ExitInstance()
{
	// 모니터 모드 중 프로그램 종료
	if( _isMonitorMode )
	{
		// 보안모드 작동
		PlayAlertSound();

		//
		AfxMessageBox("강제 프로그램 종료!\n보안 모드 작동!");

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
		// 스크린세이버 동작
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
			// 프로그램 종료
			AfxMessageBox("보안 모드 해제");
			_isMonitorMode = false;
			PostQuitMessage(0);
		}
		else
		{
			AfxMessageBox("비밀번호가 틀렸습니다.");
			_rePassCount++;
		}
	}

	//
	if( _rePassCount >= 3 )
	{
		// 보안모드 작동
		PlayAlertSound();

		//
		AfxMessageBox("비밀번호 3회 오류!\n보안 모드 작동!");
	}

	//
	Network::GetInstance().ProcPacket();

	return TRUE;
}

UINT CTestApp::MonitorWebCamThreadFunction(LPVOID pParam)
{
	CTestApp *pthis = (CTestApp*)pParam;     
	pthis->MonitorWebCamThreadDo(); // 감시를 위한 쓰레드 시작
	return 0;
}

void CTestApp::MonitorWebCamThreadDo()
{
	// 감시 모드 동작
	_openCV->StartMonitor();

	// 스크린 세이버 죽이기
#ifndef TEST
	ScreenSaver::GetInstance().KillScreenSaver();
#endif

	// 보안모드 작동
	PlayAlertSound();
	
	//
	AfxMessageBox("웹캠 동작!\n보안 모드 작동!");
}

UINT CTestApp::MonitorPowerThreadFunction(LPVOID pParam)
{
	CTestApp *pthis = (CTestApp*)pParam;     
	pthis->MonitorPowerThreadDo(); // 감시를 위한 쓰레드 시작
	return 0;
}

void CTestApp::MonitorPowerThreadDo()
{
	// 감시 모드 동작
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

	// 보안모드 작동
	PlayAlertSound();

	//
	AfxMessageBox("베터리 모드 동작!\n보안 모드 작동!");
}

UINT CTestApp::MonitorNetworkThreadFunction(LPVOID pParam)
{
	CTestApp *pthis = (CTestApp*)pParam;     
	pthis->MonitorNetworkThreadDo(); // 감시를 위한 쓰레드 시작
	return 0;
}

void CTestApp::MonitorNetworkThreadDo()
{
	// 감시 모드 동작
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

	// 보안모드 작동
	PlayAlertSound();

	//
	AfxMessageBox("네트워크 연결 끊김!\n보안 모드 작동!");
}


void CTestApp::PlayAlertSound()
{
#ifndef TEST
	std::string wav = "Test.wav";
	PlaySound(wav.c_str(),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
#endif
}
