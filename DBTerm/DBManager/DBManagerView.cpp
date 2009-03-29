// TestView.cpp : CTestView Ŭ������ ����
//

#include "stdafx.h"
#include "DBManagerApp.h"
#include "DBManagerView.h"

// CTestView

IMPLEMENT_DYNCREATE(CDBManagerView, CView)

BEGIN_MESSAGE_MAP(CDBManagerView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CTestView ����/�Ҹ�

CDBManagerView::CDBManagerView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CDBManagerView::~CDBManagerView()
{
}

int CDBManagerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

// CTestView �׸���
void CDBManagerView::OnDraw(CDC* pDC)
{
	pDC->SetDCPenColor( COLORREF(0x00ff9999) );
	pDC->MoveTo( 0, 150 );
	pDC->LineTo( 800, 150 );

	pDC->SetBkColor( COLORREF(0x00000000) );
	pDC->SetDCPenColor( COLORREF(0x00ff0000) );
	pDC->SetDCBrushColor( COLORREF(0x00ff0000) );
}