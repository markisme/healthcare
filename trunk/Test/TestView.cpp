// TestView.cpp : CTestView 클래스의 구현
//

#include "stdafx.h"
#include "Test.h"
#include "TestView.h"

// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CTestView 생성/소멸

CTestView::CTestView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CTestView::~CTestView()
{
}

int CTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

// CTestView 그리기

void CTestView::OnDraw(CDC* pDC)
{
	pDC->SetBkColor( COLORREF(0x00000000) );
	pDC->SetDCPenColor( COLORREF(0x00ff0000) );
	pDC->SetDCBrushColor( COLORREF(0x00ff0000) );

	pDC->MoveTo(100,100);
	pDC->LineTo(101,60);
	pDC->LineTo(102,20);
	pDC->LineTo(103,60);
	pDC->LineTo(104,100);
	pDC->LineTo(105,140);
	pDC->LineTo(106,180);
	pDC->LineTo(107,220);
	pDC->LineTo(108,180);
	pDC->LineTo(109,140);
}