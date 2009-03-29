// TestView.cpp : CTestView 클래스의 구현
//

#include "stdafx.h"
#include "DBManagerApp.h"
#include "DBManagerView.h"

// CTestView

IMPLEMENT_DYNCREATE(CDBManagerView, CView)

BEGIN_MESSAGE_MAP(CDBManagerView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CTestView 생성/소멸

CDBManagerView::CDBManagerView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
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

// CTestView 그리기
void CDBManagerView::OnDraw(CDC* pDC)
{
	pDC->SetDCPenColor( COLORREF(0x00ff9999) );
	pDC->MoveTo( 0, 150 );
	pDC->LineTo( 800, 150 );

	pDC->SetBkColor( COLORREF(0x00000000) );
	pDC->SetDCPenColor( COLORREF(0x00ff0000) );
	pDC->SetDCBrushColor( COLORREF(0x00ff0000) );
}