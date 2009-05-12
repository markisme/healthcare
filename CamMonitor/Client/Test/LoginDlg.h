#pragma once

#include "Resource.h"
#include <string>

class LoginDlg : public CDialog
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~LoginDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGIN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
private:
	CEdit _editID;
	CEdit _editPass;

public:
	afx_msg void OnBnClickedOk();
};
