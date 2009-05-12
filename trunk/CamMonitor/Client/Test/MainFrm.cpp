#include "stdafx.h"
#include "MainFrm.h"

#include <Dbt.h>
#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")

static const GUID GUID_DEVINTERFACE_LIST[] = 
{
 // GUID_DEVINTERFACE_USB_DEVICE
 { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } }
 // // GUID_DEVINTERFACE_VOLUME
 //{ 0x53f5630d, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
 //// GUID_DEVINTERFACE_DISK
 //{ 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
 //// GUID_DEVINTERFACE_HID, 
 //{ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },
 //// GUID_NDIS_LAN_CLASS
 //{ 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } },
 //// GUID_DEVINTERFACE_COMPORT
 //{ 0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } },
 //// GUID_DEVINTERFACE_SERENUM_BUS_ENUMERATOR
 //{ 0x4D36E978, 0xE325, 0x11CE, { 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 } },
 //// GUID_DEVINTERFACE_PARALLEL
 //{ 0x97F76EF0, 0xF883, 0x11D0, { 0xAF, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x84, 0x5C } },
 //// GUID_DEVINTERFACE_PARCLASS
 //{ 0x811FC6A5, 0xF728, 0x11D0, { 0xA5, 0x37, 0x00, 0x00, 0xF8, 0x75, 0x3E, 0xD1 } }
};

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_DEVICECHANGE()
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

	// 로긴 창 만들기
	_dlg.Create( IDD_LOGIN_DLG, this );
	_dlg.ShowWindow( TRUE );
	_dlg.UpdateWindow();

	// 로긴 창 이동
	CRect mainRect;
	GetWindowRect( mainRect );

	CRect dlgRect;
	_dlg.GetWindowRect( dlgRect );

	_dlg.MoveWindow(mainRect.Width()/2, mainRect.Height()/2,dlgRect.Width(),dlgRect.Height());

	// 디바이스 셋팅
	HDEVNOTIFY hdevnotify =NULL;
	DEV_BROADCAST_DEVICEINTERFACE dbd   ={0,};

	dbd.dbcc_size  =sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	dbd.dbcc_devicetype =DBT_DEVTYP_DEVICEINTERFACE;
	for(int i=0; i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID); i++) {
		dbd.dbcc_classguid =GUID_DEVINTERFACE_LIST[i];
		hdevnotify   =RegisterDeviceNotification(this->GetSafeHwnd(), &dbd, DEVICE_NOTIFY_WINDOW_HANDLE);
		if(hdevnotify==NULL) {
			AfxMessageBox("Error");
		}
	}

	_isDeviceChange = FALSE;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

BOOL CMainFrame::OnDeviceChange( UINT nEventType, DWORD dwData )
{
	BOOL bReturn = CWnd::OnDeviceChange(nEventType, dwData); 

	switch(nEventType) 
	{
	case DBT_DEVICEARRIVAL: 
		{ 
			// USB 디바이스 삽입
		}
		break;
	case DBT_DEVICEREMOVECOMPLETE: 
		{
			// USB 디바이스 해제
			_isDeviceChange = TRUE;
		}
		break;
	default:
		break;
	}

	return bReturn;
}
