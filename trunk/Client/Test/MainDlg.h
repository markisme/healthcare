#pragma once

#include "ScrollDlg.h"
#include "TestView.h"
#include "Resource.h"
#include <vector>
#include <map>
#include <string>

class CMainFrame;

class MainDlg : public ScrollDlg
{
	DECLARE_DYNAMIC(MainDlg)

public:
	MainDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~MainDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	BOOL Init( CWnd* parent );

public:
	void Update( void );
	void RefreshControls();

	CTestView * GetView() { return m_wndView; }

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
	CMainFrame * m_mainFrame;

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
	afx_msg void OnLbnSelchangeUserList();
};
