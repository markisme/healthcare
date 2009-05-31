// ProgramUIDlg.h : ��� ����
//

#pragma once
typedef std::vector<std::string> DataRow;
typedef std::vector<DataRow> Datas;

// CProgramUIDlg ��ȭ ����
class CProgramUIResultDlg : public CDialog
{
// �����Դϴ�.
public:
	CProgramUIResultDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_RESULT };

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
	void RefreshResult();
	void RefreshQuestion( int index );
	void RefreshNameEntity( int index );
	void RefreshTemplate( int index );
	void RefreshSQL( int index );
	void RefreshDB( int index );
	void PrintList( CListCtrl & list, DataRow & fieldList, Datas & dataList );

private:
	CComboBox _questionCombo;
	CListCtrl _nameEntity;
	CListCtrl _template;
	CListCtrl _db;
	CEdit _qs;
	CEdit _sql;

public:
	afx_msg void OnCbnSelchangeComboQs();
};
