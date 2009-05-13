#pragma once

#include "Resource.h"
#include <string>

class ConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfigDlg)

public:
	ConfigDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ConfigDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

private:
	BOOL IsWebcamMode();
	BOOL IsACPowerMode();
	BOOL IsUSBMouseMode();
	BOOL IsScreenSaverMode();
	
private:
	CEdit _editID;
	CEdit _editPass;
	CButton _c1Box;
	CButton _c2Box;
	CButton _c3Box;
	CButton _c4Box;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
};
