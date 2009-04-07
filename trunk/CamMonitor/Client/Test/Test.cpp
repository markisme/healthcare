// Test.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "Network.h"
#include "LoginDlg.h"
#include "OpenCV.h"
#include "SoundMixer.h"

BOOL CALLBACK KillScreenSaverFunc(HWND hWnd, LPARAM lParam);

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
	int clientPort = 200;
	std::string ip = "211.189.19.160";
	//std::string ip = "127.0.0.1";
	int serverPort = 10000;

	// 네트워크 초기화
	Network::GetInstance().Init( clientPort, ip, serverPort );

	_openCV = new OpenCV;
	_openCV->Init();

	_soundMixer = new SoundMixer;
	_soundMixer->Init();

	_soundMixer->SetMute( FALSE );
	_soundMixer->SetVolumn( 65535 );

	//
	//std::string wav = "Test.wav";
	//PlaySound(wav.c_str(),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

	//LoginDlg dlg;
	//if( dlg.DoModal() != IDOK )
	//{
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

	// 스크린세이버 모드 실행
	//::SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,TRUE,0,0);

	// 캠 제어 모듈 작동
	CWinThread * pThread = AfxBeginThread(ThreadFunction, this);

	return TRUE;
}

int CTestApp::ExitInstance()
{
	_openCV->Uninit();
	delete( _openCV );

	_soundMixer->Uninit();
	delete( _soundMixer );

	//
	Network::GetInstance().Uninit();

	//
	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return TRUE;
}

BOOL CTestApp::OnIdle( LONG lCount )
{
	// Update 함수
	m_pMainWnd->ShowWindow( TRUE );
	HWND hwnd = GetActiveWindow();	
	SendMessage(hwnd,WM_SYSCOMMAND, SC_SCREENSAVE,NULL);
	m_pMainWnd->ShowWindow( FALSE );

	//SendMessage(HWND_BROADCAST,WM_SYSCOMMAND, SC_SCREENSAVE,NULL);

	LoginDlg dlg;
	if( dlg.DoModal() != IDOK )
	{
		// 다시 제어모드로
		return FALSE;
	}
	else
	{
		// 프로그램 종료
		PostQuitMessage(0);
	}

	//
	Network::GetInstance().ProcPacket();

	return TRUE;
}

UINT CTestApp::ThreadFunction(LPVOID pParam)
{
	CTestApp *pthis = (CTestApp*)pParam;     
	pthis->ThreadDo(); // 패킷을 받기 위한 Thread 시작

	return 0;
}

void CTestApp::ThreadDo()
{
	while (1)
	{
		Sleep(3000);

		HDESK hDesktop = OpenDesktop( TEXT("screen-saver"), 0, FALSE, DESKTOP_READOBJECTS | DESKTOP_WRITEOBJECTS ); 

		if( hDesktop != 0 )
		{
			EnumDesktopWindows( hDesktop, (WNDENUMPROC)KillScreenSaverFunc , 0 );
			CloseDesktop( hDesktop );
			OutputDebugString( "킬 스크린세이버\n" );
		}
		//else
		//{
		//	PostMessage( GetForegroundWindow(), WM_CLOSE, 0, 0 );
		//}
	}
}

BOOL CALLBACK KillScreenSaverFunc(HWND hWnd, LPARAM lParam)
{
	if( IsWindowVisible( hWnd ) )
	{
		::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		OutputDebugString( "킬 스크린세이버\n" );
	}
	return true;
}