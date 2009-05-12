#pragma once 
#include "LoginDlg.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	LoginDlg * GetLoginDlg() { return &_dlg; }
	BOOL IsDeviceChange() { return _isDeviceChange; };

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);

	DECLARE_MESSAGE_MAP()

private:
	LoginDlg _dlg;
	BOOL _isDeviceChange;
};


