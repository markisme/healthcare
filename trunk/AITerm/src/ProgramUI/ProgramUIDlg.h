// ProgramUIDlg.h : ��� ����
//

#pragma once

struct TableData
{
	int _rowNum;
	std::string _fieldName;
	std::string _fieldData;
};

typedef std::vector<TableData> Table;


// CProgramUIDlg ��ȭ ����
class CProgramUIDlg : public CDialog
{
// �����Դϴ�.
public:
	CProgramUIDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PROGRAMUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
