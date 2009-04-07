// Test.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "TestView.h"
#include "Network.h"
#include "LoginDlg.h"

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
	//std::string ip = "211.189.19.160";
	std::string ip = "127.0.0.1";
	int serverPort = 10000;

	// ��Ʈ��ũ �ʱ�ȭ
	Network::GetInstance().Init( clientPort, ip, serverPort );



	//
	//InitMicControl();
	//SetMute(FALSE);
	//SetVolumn(65535);

	//
	std::string wav = "Test.wav";
	PlaySound(wav.c_str(),NULL,SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

	LoginDlg dlg;
	if( dlg.DoModal() != IDOK )
	{
		return -1;
	}

	if( dlg.DoModal() != IDOK )
	{
		return -1;
	}

	return TRUE;
}

int CTestApp::ExitInstance()
{
	//
	Network::GetInstance().Uninit();

	return TRUE;
}

BOOL CTestApp::OnIdle( LONG lCount )
{
	// Update �Լ�

	//
	Network::GetInstance().ProcPacket();

	return TRUE;
}