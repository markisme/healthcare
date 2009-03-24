// WinMgrDlg.h : header file
//

#if !defined(AFX_WINMGRDLG_H__BEAD4E86_F81B_11D2_95F4_D164A61AE6D6__INCLUDED_)
#define AFX_WINMGRDLG_H__BEAD4E86_F81B_11D2_95F4_D164A61AE6D6__INCLUDED_

#include "ConfigDlg.h"	// Added by ClassView
#include "InfoDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWinMgrDlg dialog

class CWinMgrDlg : public CDialog
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CWinMgrDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWinMgrDlg)
	enum { IDD = IDD_WINMGR_DIALOG };
	CString	m_strEditSSStatus;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinMgrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWinMgrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void CreateToolTips();
	afx_msg void ButtonUpdateSSConfig();
	afx_msg void OnButtonreboot();
	afx_msg void OnButtondisabless();
	afx_msg void OnButtonenabless();
	afx_msg void OnButtonactivatess();
	afx_msg void OnButtonlogoff();
	afx_msg void CheckSSStatus(void);
	afx_msg void OnHelpAbout();
	afx_msg void OnOptionsSetoptions();
	afx_msg void OnFileExit();
	afx_msg void OnHelpInfo();
	afx_msg void OnOptionsConfig();
	afx_msg void ActivateSS(void);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CInfoDlg m_dInfoDlg;
	CConfigDlg m_dConfigDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINMGRDLG_H__BEAD4E86_F81B_11D2_95F4_D164A61AE6D6__INCLUDED_)
