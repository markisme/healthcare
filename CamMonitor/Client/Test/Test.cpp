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
_rePassCount( 0 )
{
}

CTestApp theApp;

BOOL CTestApp::InitInstance()
{
	CWinApp::InitInstance();

	// ���� ����
	bool isHost = false;
	std::string id;
	std::string pw;
	int clientPort = 200;
	std::string ip = "211.189.20.246";
	//std::string ip = "211.189.19.160";
	//std::string ip = "127.0.0.1";
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
	_dlg->DoModal();
	if(  Network::GetInstance()._isSuccessAuth == false )
	{
		AfxMessageBox("��й�ȣ�� Ʋ�Ƚ��ϴ�.");
		return -1;
	}

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
		// ��ũ�����̹� ����
		m_pMainWnd->ShowWindow( TRUE );
		HWND hwnd = GetActiveWindow();
		ScreenSaver::GetInstance().StartScreenSaver( hwnd );
		m_pMainWnd->ShowWindow( FALSE );
	}

	//
	if( _dlg->DoModal() == IDOK )
	{
		if( Network::GetInstance()._isSuccessAuth == true )
		{
			// ���α׷� ����
			AfxMessageBox("���� ��� ����");
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
		//
		std::string wav = "Test.wav";
		PlaySound(wav.c_str(),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		//
		AfxMessageBox("��й�ȣ 3ȸ ����!\n���� ��� �۵�!");
	}

	//
	Network::GetInstance().ProcPacket();

	return TRUE;
}

UINT CTestApp::ThreadFunction(LPVOID pParam)
{
	CTestApp *pthis = (CTestApp*)pParam;     
	pthis->ThreadDo(); // ���ø� ���� ������ ����
	return 0;
}

void CTestApp::ThreadDo()
{
	// ���� ��� ����
	_openCV->StartMonitor();
	ScreenSaver::GetInstance().KillScreenSaver();

	// ���ȸ�� �۵�
	std::string wav = "Test.wav";
	PlaySound(wav.c_str(),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

	//
	AfxMessageBox("��ķ ����!\n���� ��� �۵�!");
}