#include "stdafx.h"
#include "LoginDlg.h"

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
	DDX_Control(pDX, IDC_EDIT_ADDRESS, _editAddr);
	DDX_Control(pDX, IDC_EDIT_NAME, _editID);
	DDX_Control(pDX, IDC_EDIT_PASS, _editPass);
	DDX_Control(pDX, IDC_EDIT_PORT, _editPort);
	DDX_Control(pDX, IDC_EDIT_DB, _editDB);
}

BEGIN_MESSAGE_MAP(LoginDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &LoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL LoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_editAddr.SetWindowText( "localhost" );
	_editID.SetWindowText( "root" );
	_editPort.SetWindowText( "3306" );
	_editDB.SetWindowText( "test" );

	return TRUE;  // return TRUE unless you set the focus to a control
}

void LoginDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

int LoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
void LoginDlg::OnBnClickedOk()
{
	CString addr;
	_editAddr.GetWindowText( addr );

	CString name;
	_editID.GetWindowText( name );

	CString pass;
	_editPass.GetWindowText( pass );

	CString port;
	_editPort.GetWindowText( port );

	CString db;
	_editDB.GetWindowText( db );

	DBConnectInfo info;
	info.ip = addr;
	info.user = name;
	info.pass = pass;
	info.port = atoi(port);
	info.dbName = db;

	DBConnector::GetInstance().Init( info );

	OnOK();
}