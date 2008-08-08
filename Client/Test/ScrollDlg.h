#pragma once


// ScrollDlg ��ȭ �����Դϴ�.

const int SCROLLDLG_WHELL_STEP = 10;

class ScrollDlg : public CDialog
{
	DECLARE_DYNAMIC(ScrollDlg)

public:
	ScrollDlg( DWORD id, CWnd* pParent = NULL );   // ǥ�� �������Դϴ�.
	virtual ~ScrollDlg();

// ��ȭ ���� �������Դϴ�.
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
