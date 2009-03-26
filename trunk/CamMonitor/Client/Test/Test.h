#pragma once

#include "resource.h"       // 주 기호입니다.
#include "mmsystem.h"
#include <vector>

class CMainFrame;

class CTestApp : public CWinApp
{
public:
	CTestApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	virtual BOOL OnIdle( LONG lCount );

	void Init();
	void Uninit();
	void ProcPacket();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	BOOL InitMicControl();
	BOOL SetMute(BOOL bMode);
	BOOL SetVolumn(INT nValue);

private:
	HMIXER m_hMixer;
	DWORD m_dwControlID;  
	DWORD m_dwLineID;
	DWORD m_dwChannels;
	DWORD m_VolumneID; 
	MMRESULT m_nStatus;
};