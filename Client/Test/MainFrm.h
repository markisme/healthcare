#pragma once

#include "MainDlg.h"

class RakPeerInterface;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

	RakPeerInterface * GetClient() { return _client; }
	void SetClient( RakPeerInterface * client ) { _client = client; }

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

private:
	MainDlg _mainDlg;
	RakPeerInterface * _client;
};


