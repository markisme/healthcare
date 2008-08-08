#pragma once

#include "ScrollDlg.h"
#include "TestView.h"
#include "Resource.h"
#include <vector>
#include <map>
#include <string>

class MainDlg : public ScrollDlg
{
	DECLARE_DYNAMIC(MainDlg)

public:
	MainDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~MainDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	BOOL Init( CWnd* parent );

public:
	void Update( void );
	void RefreshControls();

private:
	CListBox _userList;
	CListCtrl _dataList;
	CListCtrl _userDataList;

	CComboBox _yearList;
	CComboBox _monthList;
	CComboBox _dayList;
	CComboBox _hourList;

	CStatic _image;
	CTestView * m_wndView;

	struct HealthInfo
	{
		int time;
		int data1;
		int data2;
	};

	std::map<std::string,HealthInfo*> _infoMap; 
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
public:
	afx_msg void OnLbnSelchangeUserList();
};
