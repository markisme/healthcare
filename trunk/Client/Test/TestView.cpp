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
	//Invalidate();

	pDC->SetBkColor( COLORREF(0x00000000) );
	pDC->SetDCPenColor( COLORREF(0x00ff0000) );
	pDC->SetDCBrushColor( COLORREF(0x00ff0000) );

	pDC->MoveTo(0,100);

	for(int num=0; num<50; num++)
	{
		for(int cnt=0; cnt<10; cnt++)
		{
			int x = (num*10) + cnt;
			int y = 50 + rand() % 10 * 10;
			pDC->LineTo(x,y);
			pDC->LineTo(x,y);
			pDC->LineTo(x,y);
			pDC->LineTo(x,y);
			pDC->LineTo(x,y);
			pDC->LineTo(x,y);
			pDC->LineTo(x,y);
			pDC->LineTo(x,y);
			pDC->LineTo(x,y);
			pDC->LineTo(x,y);
			pDC->LineTo(x,y);
		}		
	}

	//Sleep(1000);
}