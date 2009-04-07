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
	SoundMixer * _soundMixer;
	OpenCV * _openCV;
};