#pragma once


// ScrollDlg 대화 상자입니다.

const int SCROLLDLG_WHELL_STEP = 10;

class ScrollDlg : public CDialog
{
	DECLARE_DYNAMIC(ScrollDlg)

public:
	ScrollDlg( DWORD id, CWnd* pParent = NULL );   // 표준 생성자입니다.
	virtual ~ScrollDlg();

// 대화 상자 데이터입니다.
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

protected:
	CRect _firstRect;
};
