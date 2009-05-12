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

	static UINT MonitorPowerThreadFunction(LPVOID pParam);
	void MonitorPowerThreadDo();

	static UINT MonitorNetworkThreadFunction(LPVOID pParam);
	void MonitorNetworkThreadDo();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	void PlayAlertSound();
	void RegisterServiceProcess(DWORD dwProcessId, DWORD dwType);
	void HideApp(bool Hide);

private:
	SoundMixer * _soundMixer;
	OpenCV * _openCV;
	int _rePassCount;
	bool _isMonitorMode;

	bool _isShowLoginDlg;
};
