#pragma once
#include "resource.h"       // 주 기호입니다.
#include "mmsystem.h"
#include <powrprof.h>
#include <WinDef.h>

class SoundMixer;
class OpenCV;
class LoginDlg;

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

	static UINT WebCamThreadFunction(LPVOID pParam);
	void WebCamThreadDo();

	static UINT PowerEventThreadFunction(LPVOID pParam);
	void PowerEventThreadDo();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	SoundMixer * _soundMixer;
	OpenCV * _openCV;
	LoginDlg * _dlg;
	int _rePassCount;
};
