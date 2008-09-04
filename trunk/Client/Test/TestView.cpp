// TestView.cpp : CTestView 클래스의 구현
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
	pDC->SetDCPenColor( COLORREF(0x00ff9999) );
	pDC->MoveTo( 0, 150 );
	pDC->LineTo( 800, 150 );

	pDC->SetBkColor( COLORREF(0x00000000) );
	pDC->SetDCPenColor( COLORREF(0x00ff0000) );
	pDC->SetDCBrushColor( COLORREF(0x00ff0000) );

	// 보여줄 유저 선택
	int userNo = 0;
	if( Network::GetInstance()._isHost )
	{
		userNo = _curUserNo;
	}
	else
	{
		userNo = Network::GetInstance()._myUserNo;
	}

	// 데이터 그리기
	DataList & dataList = Network::GetInstance().GetDataList( userNo );

	int count = dataList.size();
	for( int num = 0; num < count; num++ )
	{
		PacketData data = dataList[ num ];

		if( num == 0 )
		{
			pDC->MoveTo( num, data._y );
		}
		else
		{
			pDC->LineTo( num, data._y );
		}
	}
}