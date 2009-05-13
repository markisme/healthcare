#pragma once

#include "Resource.h"
#include <string>

class ConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfigDlg)

public:
	ConfigDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ConfigDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
