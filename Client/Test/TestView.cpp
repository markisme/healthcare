// TestView.cpp : CTestView Ŭ������ ����
//

#include "stdafx.h"
#include "Test.h"
#include "TestView.h"
#include "Network.h"

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
	//Invalidate();

	pDC->SetBkColor( COLORREF(0x00000000) );
	pDC->SetDCPenColor( COLORREF(0x00ff0000) );
	pDC->SetDCBrushColor( COLORREF(0x00ff0000) );

	pDC->MoveTo(0,100);

	for(int num=0; num<30; num++)
	{
		for(int cnt=0; cnt<10; cnt++)
		{
			int x = (num*10) + cnt;
			int y = 50 + rand() % 10 * 10;
			pDC->LineTo(x,y);

			char message[100];
			sprintf( message, "%d,%d", x, y );

			RakPeerInterface * client = Network::GetInstance().GetClient();
			if( client )
			{
				client->Send(message, (int) strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
			}

			Sleep(10);
		}		
	}

	//Sleep(1000);
}