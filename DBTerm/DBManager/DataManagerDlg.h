#pragma once

#include "DataListItem.h"
#include "Resource.h"
#include <vector>
#include <map>
#include <string>

class CMainFrame;
class CTableTree;

class DataManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(DataManagerDlg)

public:
	DataManagerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DataManagerDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DATA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	INT_PTR DoModal( std::string tableName, int selectColNo = -1 );

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();

public:
	std::vector<DataListItem*> _dlgList;
	std::string _tableName;
	int _selectColNo;
	ColList _colList;
	DataList _dataList;	
};
