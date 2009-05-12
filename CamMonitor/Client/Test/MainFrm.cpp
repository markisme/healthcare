#include "stdafx.h"
#include "Test.h"

#include "MainFrm.h"

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//// 로긴 창 만들기
	//_dlg.Create( IDD_LOGIN_DLG, this );
	//_dlg.ShowWindow( TRUE );
	//_dlg.UpdateWindow();

	//// 로긴 창 이동
	//CRect mainRect;
	//GetWindowRect( mainRect );

	//CRect dlgRect;
	//_dlg.GetWindowRect( dlgRect );

	//_dlg.MoveWindow(mainRect.Width()/2, mainRect.Height()/2,dlgRect.Width(),dlgRect.Height());

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}