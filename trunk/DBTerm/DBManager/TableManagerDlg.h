#pragma once

#include "TableListItem.h"
#include "Resource.h"
#include <vector>
#include <map>
#include <string>

class CMainFrame;
class CTableTree;

class TableManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(TableManagerDlg)

public:
	TableManagerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~TableManagerDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_TABLE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	INT_PTR DoModal( int command, std::string tableName );

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();

	void AddField( CDialog * child );
	void Refresh();

public:
	std::vector<TableListItem*> _dlgList;
	CEdit _tableName;
	int _command;
	std::string _tbName;
	ColList _colList;
};
