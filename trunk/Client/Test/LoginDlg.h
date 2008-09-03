#pragma once

#include "Resource.h"
#include <string>

class LoginDlg : public CDialog
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~LoginDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LOGIN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	static UINT ThreadFunction(LPVOID pParam);
	void ThreadDo();

	std::string GetID() { USES_CONVERSION;return (LPCSTR)T2A(_id); }
	std::string GetPW() { USES_CONVERSION;return (LPCSTR)T2A(_pass); }
	std::string GetCPort() { USES_CONVERSION;return (LPCSTR)T2A(_cPort); }
	std::string GetIP() { USES_CONVERSION;return (LPCSTR)T2A(_ip); }
	std::string GetPort() { USES_CONVERSION;return (LPCSTR)T2A(_port); }

private:
	CEdit _editID;
	CEdit _editPass;
	CEdit _editCPort;
	CEdit _editIP;
	CEdit _editPort;

	CString _id;
	CString _pass;
	CString _cPort;
	CString _ip;
	CString _port;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
public:
	afx_msg void OnBnClickedOk();
};
