#include "resource.h"       // 주 기호입니다.
#include "mmsystem.h"
#include <vector>

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

	static UINT ThreadFunction(LPVOID pParam);
	void ThreadDo();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	SoundMixer * _soundMixer;
	OpenCV * _openCV;
	LoginDlg * _dlg;
	int _rePassCount;
};
