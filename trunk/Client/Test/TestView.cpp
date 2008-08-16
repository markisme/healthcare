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
	//Invalidate();

	pDC->SetBkColor( COLORREF(0x00000000) );
	pDC->SetDCPenColor( COLORREF(0x00ff0000) );
	pDC->SetDCBrushColor( COLORREF(0x00ff0000) );

	pDC->MoveTo(0,100);

	if( Network::GetInstance()._isHost )
	{
		// 호스트 이면 받은 데이터로 라인 그리기
		DataList & dataList = Network::GetInstance().GetDataList();
		
		int count = dataList.size();
		for( int num = 0; num < count; num++ )
		{
			PacketData data = dataList[ num ];
			pDC->LineTo( data._x, data._y );
		}

		dataList.clear();
	}
	else
	{
		// 호스트가 아니면 라인 그려서 보내기
		std::vector<PacketData> dataList;

		for(int num=0; num<20; num++)
		{
			for(int cnt=0; cnt<10; cnt++)
			{
				int x = (num*20) + cnt;
				int y = 50 + rand() % 10 * 10;
				pDC->LineTo(x,y);

				dataList.push_back( PacketData( x, y ) );
				Sleep(10);
			}		
		}

		RakNet::BitStream outBuffer;
		outBuffer.Write( (unsigned char)13 );
		outBuffer.Write( dataList );

		RakPeerInterface * client = Network::GetInstance().GetClient();
		if( client )
		{
			client->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		}
	}
}