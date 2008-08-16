// TestView.cpp : CTestView Ŭ������ ����
//

#include "stdafx.h"
#include "Test.h"
#include "TestView.h"
#include "Network.h"
#include "BitStream.h"

// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CTestView ����/�Ҹ�

CTestView::CTestView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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

// CTestView �׸���

void CTestView::OnDraw(CDC* pDC)
{
	pDC->SetBkColor( COLORREF(0x00000000) );
	pDC->SetDCPenColor( COLORREF(0x00ff0000) );
	pDC->SetDCBrushColor( COLORREF(0x00ff0000) );

	pDC->MoveTo(0,100);

	// ������ �׸���
	DataList & dataList = Network::GetInstance().GetDataList();

	int count = dataList.size();
	for( int num = 0; num < count; num++ )
	{
		PacketData data = dataList[ num ];
		pDC->LineTo( data._x, data._y );
	}
}