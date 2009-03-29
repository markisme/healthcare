#pragma once

#include "DBManagerView.h"
#include "Resource.h"
#include <vector>
#include <map>
#include <string>

class CMainFrame;
class CTableTree;

class MainDlg : public CDialog
{
	DECLARE_DYNAMIC(MainDlg)

public:
	MainDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~MainDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAIN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	BOOL Init( CWnd* parent );

public:
	void Update( void );
	void RefreshControls();

	CDBManagerView * GetView() { return m_wndView; }

private:
	CDBManagerView * m_wndView;
	CMainFrame * m_mainFrame;
	CTableTree * _tableTree;
	CListCtrl _dataList;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnTvnSelchangedTreeTable(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedInsertButton();
	afx_msg void OnBnClickedModifyButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedRefreshButton();
};
