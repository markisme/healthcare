#include "stdafx.h"
#include "ConfigDlg.h"
#include "Config.h"

IMPLEMENT_DYNAMIC(ConfigDlg, CDialog)
ConfigDlg::ConfigDlg(CWnd* pParent /*=NULL*/)	: CDialog(ConfigDlg::IDD, pParent)
{	
}

ConfigDlg::~ConfigDlg()
{
}

void ConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ID, _editID);
	DDX_Control(pDX, IDC_EDIT_PW, _editPass);
	DDX_Control(pDX, IDC_CHECK1, _c1Box);
	DDX_Control(pDX, IDC_CHECK2, _c2Box);
	DDX_Control(pDX, IDC_CHECK3, _c3Box);
	DDX_Control(pDX, IDC_CHECK4, _c4Box);
}

BEGIN_MESSAGE_MAP(ConfigDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &ConfigDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL ConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_editID.SetWindowText( "asd" );
	_editPass.SetWindowText( "1234" );

	_c1Box.SetCheck( TRUE );
	_c2Box.SetCheck( TRUE );
	_c3Box.SetCheck( TRUE );
	_c4Box.SetCheck( TRUE );

	return TRUE;  // return TRUE unless you set the focus to a control
}

void ConfigDlg::OnBnClickedOk()
{
	CString id;
	_editID.GetWindowText( id );
	std::string idStr = id;

	CString pass;
	_editPass.GetWindowText( pass );
	std::string passStr = pass;

	// 로긴 요청
	Network::GetInstance()._isSuccessAuth = -1;
	Network::GetInstance().ReqLoginSend( idStr, passStr );

	while( true )
	{
		// 패킷 처리
		if( Network::GetInstance().ProcPacket() == TRUE )
		{
			if( Network::GetInstance()._isSuccessAuth == 1 )
			{
				// 프로그램 실행
				AfxMessageBox("도난 감시 모드를 시작 하겠습니다.");
				break;
			}
			else if( Network::GetInstance()._isSuccessAuth == 0 )
			{
				AfxMessageBox("비밀번호가 틀렸습니다.");
				return;
			}
		}
	}

	//
	Config::GetInstance()._isWebcamMode = IsWebcamMode();
	Config::GetInstance()._isACPowerMode = IsACPowerMode();
	Config::GetInstance()._isUSBMouseMode = IsUSBMouseMode();
	Config::GetInstance()._isScreenSaverMode = IsScreenSaverMode();

	//
	OnOK();
}

void ConfigDlg::OnClose()
{
	OnCancel();
}

BOOL ConfigDlg::IsWebcamMode()
{
	if( _c1Box.GetCheck() )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL ConfigDlg::IsACPowerMode()
{
	if( _c2Box.GetCheck() )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL ConfigDlg::IsUSBMouseMode()
{
	if( _c3Box.GetCheck() )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL ConfigDlg::IsScreenSaverMode()
{
	if( _c4Box.GetCheck() )
	{
		return TRUE;
	}
	return FALSE;
}