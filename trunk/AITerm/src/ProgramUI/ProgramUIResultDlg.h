// ProgramUIDlg.h : 헤더 파일
//

#pragma once
typedef std::vector<std::string> DataRow;
typedef std::vector<DataRow> Datas;

// CProgramUIDlg 대화 상자
class CProgramUIResultDlg : public CDialog
{
// 생성입니다.
public:
	CProgramUIResultDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RESULT };

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
