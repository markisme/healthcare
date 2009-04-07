// Test.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "Network.h"
#include "LoginDlg.h"
#include "OpenCV.h"
#include "SoundMixer.h"

static int count = 0;

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

	// ��Ʈ��ũ �ʱ�ȭ
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

	// ��ũ�����̹� ��� ����
	//::SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,TRUE,0,0);

	// ķ ���� ��� �۵�
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
	// Update �Լ�
	//m_pMainWnd->ShowWindow( TRUE );
	//HWND hwnd = GetActiveWindow();	
	//SendMessage(hwnd,WM_SYSCOMMAND, SC_SCREENSAVE,NULL); //send SysCommand message calling SS
	//m_pMainWnd->ShowWindow( FALSE );
	SendMessage(HWND_BROADCAST, WM_SYSCOMMAND,  SC_SCREENSAVE, 1);   

	LoginDlg dlg;
	if( dlg.DoModal() != IDOK )
	{
		// �ٽ� �������
		return FALSE;
	}
	else
	{
		// ���α׷� ����
		PostQuitMessage(0);
	}

	//
	Network::GetInstance().ProcPacket();

	return TRUE;
}

UINT CTestApp::ThreadFunction(LPVOID pParam)
{
	CTestApp *pthis = (CTestApp*)pParam;     
	pthis->ThreadDo(); // ��Ŷ�� �ޱ� ���� Thread ����

	return 0;
}

void CTestApp::ThreadDo()
{
	while (1)
	{
		Sleep(100);
		
		//AfxMessageBox("ȣ��");
		//::SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,FALSE,0,0);

		//SendMessage(hwnd,WM_SYSCOMMAND, SC_SCREENSAVE,NULL); //send SysCommand message calling SS
		SendMessage(HWND_BROADCAST, WM_SYSCOMMAND,  SC_RESTORE, 1);

		char str[1024];
		sprintf(str, "%d\n", count++);
		OutputDebugString( str );

		if( ::SystemParametersInfo(SPI_GETSCREENSAVERRUNNING,TRUE,0,0) == TRUE )
		{
			AfxMessageBox("ȣ��");
		}
	}
}