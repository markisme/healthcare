#include "stdafx.h"
#include "ScrollDlg.h"

IMPLEMENT_DYNAMIC(ScrollDlg, CDialog)
ScrollDlg::ScrollDlg(DWORD id, CWnd* pParent /*=NULL*/)
	: CDialog( id, pParent )
{
}

ScrollDlg::~ScrollDlg()
{
}

void ScrollDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ScrollDlg, CDialog)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL ScrollDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetWindowRect( _firstRect );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void ScrollDlg::OnOK()
{
	// CDialog::OnOK();
}

void ScrollDlg::OnCancel()
{
	// CDialog::OnCancel();
}

// ScrollDlg 메시지 처리기입니다.
void ScrollDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int dy = 0;
	int oldVPos = GetScrollPos( SB_VERT );	

	switch( nSBCode )
	{
	case SB_LINEUP:
		dy = 1;	
		break;
	case SB_LINEDOWN:
		dy = -1;
		break;
	case SB_PAGEUP:
		dy = 10;	
		break;
	case SB_PAGEDOWN:
		dy = -10;	
		break;
	case SB_THUMBTRACK:				
		dy = oldVPos - nPos;		
		break;
	default:
		CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	ScrollWindow( 0, dy );

	int newVPos = oldVPos - dy;
	SetScrollPos( SB_VERT, newVPos, TRUE );

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void ScrollDlg::OnSize(UINT nType, int cx, int cy)
{
	// 맨위로 페이지 올림
	int pos = GetScrollPos( SB_VERT );
	ScrollWindow( 0, pos );
	SetScrollPos( SB_VERT, 0, FALSE );

	// 사이즈 변경
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetWindowRect( rect );

	int curHeight = rect.Height();
	int startHeight = _firstRect.Height();

	int minRange = 0;
	int maxRange = startHeight - curHeight;

	SetScrollRange( SB_VERT, 0, max( 0, maxRange ), TRUE );
}

void ScrollDlg::OnLButtonDown(UINT nFlags, CPoint point)
{	
	SetFocus();

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL ScrollDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int dy = 0;
	if( zDelta > 0 )
	{
		dy = SCROLLDLG_WHELL_STEP;
	}
	else
	{
		dy = -SCROLLDLG_WHELL_STEP;
	}

	int oldVPos = GetScrollPos( SB_VERT );
	int newVPos = oldVPos - dy;	
	
	int min, max;
	GetScrollRange( SB_VERT, &min, &max );

	if( min <= newVPos && newVPos <= max )
	{
		ScrollWindow( 0, dy );
		SetScrollPos( SB_VERT, newVPos, TRUE );
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}