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
	
private:
	CEdit _editID;
	CEdit _editPass;

public:
	afx_msg void OnBnClickedOk();
};
