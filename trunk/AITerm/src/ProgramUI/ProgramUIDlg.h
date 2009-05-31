// ProgramUIDlg.h : 헤더 파일
//

#pragma once

struct TableData
{
	int _rowNum;
	std::string _fieldName;
	std::string _fieldData;
};

typedef std::vector<TableData> Table;


// CProgramUIDlg 대화 상자
class CProgramUIDlg : public CDialog
{
// 생성입니다.
public:
	CProgramUIDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROGRAMUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void ProcessQuestion( std::string path );
	void RefreshAnswer();
	void PrintTable( std::string word );
	void GetFieldList( Table & table, std::vector<std::string> & fieldList );
	int GetFieldNo( std::string fieldName, std::vector<std::string> & fieldList );

private:
	CEdit _answer;
	CEdit _question;
	CComboBox _questionCombo;
	CListCtrl _table;

public:
	afx_msg void OnBnClickedButtonQuestion();
	afx_msg void OnCbnSelchangeComboQuestion();
	afx_msg void OnBnClickedButtonResult();
	afx_msg void OnBnClickedButtonLoad();
};
