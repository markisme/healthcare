// Test.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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
	int clientPort = 200;
	//std::string ip = "211.189.19.160";
	std::string ip = "127.0.0.1";
	int serverPort = 10000;

	// 네트워크 초기화
	Network::GetInstance().Init( clientPort, ip, serverPort );

	//
	InitMicControl();
	SetMute(FALSE);
	SetVolumn(65535);

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

BOOL CTestApp::InitMicControl()
{
	DWORD LineType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;

	m_nStatus = MMSYSERR_NOERROR;
	m_hMixer = 0;
	m_dwChannels = 1;
	m_VolumneID = 0; 
	m_dwControlID = 0; 
	m_dwLineID = 0;
	// 믹서 디바이스 열기
	m_nStatus = mixerOpen(&m_hMixer, 0, 0, 0L, CALLBACK_NULL);
	if (m_nStatus != MMSYSERR_NOERROR)
	{
		TRACE("mixerOpen failed.n");
		return FALSE;
	}
	// 믹서 라인 얻기
	MIXERLINE ml;
	ml.cbStruct = sizeof(MIXERLINE);
	ml.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;

	m_nStatus = mixerGetLineInfo((HMIXEROBJ)m_hMixer, &ml, 
		MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE);
	if(m_nStatus != MMSYSERR_NOERROR)
	{
		TRACE("mixerGetLineInfo failed.n");
		return FALSE;
	}
	m_dwChannels = ml.cChannels;
	m_dwLineID = ml.dwLineID;

	return TRUE;
}


BOOL CTestApp::SetMute(BOOL bMode)
{
	// 믹서 라인 컨트롤들 얻기 위한 구조체 초기화
	MIXERCONTROL mc;
	MIXERLINECONTROLS mlc;
	mlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mlc.dwLineID = m_dwLineID;
	mlc.cControls = 1;
	mlc.cbmxctrl = sizeof(MIXERCONTROL);
	mlc.pamxctrl = &mc;

	// Mute 컨트롤 얻기
	mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
	m_nStatus = mixerGetLineControls((HMIXEROBJ)m_hMixer, &mlc, 
		MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE);
	if(m_nStatus != MMSYSERR_NOERROR)
	{
		TRACE("Volumne mixerGetLineControls failed.n");
		return FALSE;
	}
	m_dwControlID = mc.dwControlID;

	MIXERCONTROLDETAILS mcd = {0};
	MIXERCONTROLDETAILS_BOOLEAN mcb = {0};
	mcb.fValue    = bMode;
	mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mcd.hwndOwner = 0;
	mcd.dwControlID = m_dwControlID;
	mcd.cChannels = 1;
	mcd.cMultipleItems = 0;
	mcd.paDetails = &mcb;
	mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	m_nStatus = mixerSetControlDetails((HMIXEROBJ) m_hMixer, 
		&mcd, MIXER_SETCONTROLDETAILSF_VALUE);
	if(m_nStatus != MMSYSERR_NOERROR)
	{
		TRACE("Volumne mixerGetLineControls failed.n");
		return FALSE;
	}
	return TRUE;
}

BOOL CTestApp::SetVolumn(INT nValue)
{
	// 믹서 라인 컨트롤들 얻기 위한 구조체 초기화
	MIXERCONTROL mc;
	MIXERLINECONTROLS mlc;
	mlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mlc.dwLineID = m_dwLineID;
	mlc.cControls = 1;
	mlc.cbmxctrl = sizeof(MIXERCONTROL);
	mlc.pamxctrl = &mc;
	// 볼륨 컨트롤 얻기
	mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	m_nStatus = mixerGetLineControls((HMIXEROBJ)m_hMixer, &mlc, 
		MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE);
	if(m_nStatus != MMSYSERR_NOERROR)
	{
		TRACE("Volumne mixerGetLineControls failed.n");
		return FALSE;
	}
	m_dwControlID = mc.dwControlID;

	MIXERCONTROLDETAILS mcd = {0};
	MIXERCONTROLDETAILS_UNSIGNED mcdu = {0};
	mcdu.dwValue = nValue; // the volume is a number between 0 and 65535
	mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mcd.hwndOwner = 0;
	mcd.dwControlID = m_dwControlID;
	mcd.paDetails = &mcdu;
	mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mcd.cChannels = 1;
	m_nStatus = mixerSetControlDetails((HMIXEROBJ) m_hMixer, 
		&mcd, MIXER_SETCONTROLDETAILSF_VALUE);
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
	// Update 함수

	//
	Network::GetInstance().ProcPacket();

	return TRUE;
}