#include "stdafx.h"
#include "LoginDlg.h"
#include "Network.h"

IMPLEMENT_DYNAMIC(LoginDlg, CDialog)
LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)	: CDialog(LoginDlg::IDD, pParent)
{	
}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ID, _editID);
	DDX_Control(pDX, IDC_EDIT_PW, _editPass);
}

BEGIN_MESSAGE_MAP(LoginDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &LoginDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL LoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_editID.SetWindowText( "asd" );
	_editPass.SetWindowText( "1234" );

	return TRUE;  // return TRUE unless you set the focus to a control
}

void LoginDlg::OnBnClickedOk()
{
	CString id;
	_editID.GetWindowText( id );
	std::string idStr = id;

	CString pass;
	_editPass.GetWindowText( pass );
	std::string passStr = pass;

	// 로긴 요청
	Network::GetInstance().ReqLoginSend( idStr, passStr );
}