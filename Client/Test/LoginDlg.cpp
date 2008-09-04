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
	DDX_Text(pDX, IDC_EDIT_ID, _id);
	DDX_Text(pDX, IDC_EDIT_PW, _pass);
	DDX_Text(pDX, IDC_EDIT_CPORT, _cPort);
	DDX_Text(pDX, IDC_EDIT_IP, _ip);
	DDX_Text(pDX, IDC_EDIT_PORT, _port);

	DDX_Control(pDX, IDC_EDIT_ID, _editID);
	DDX_Control(pDX, IDC_EDIT_PW, _editPass);
	DDX_Control(pDX, IDC_EDIT_CPORT, _editCPort);
	DDX_Control(pDX, IDC_EDIT_IP, _editIP);
	DDX_Control(pDX, IDC_EDIT_PORT, _editPort);
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

	//_editID.SetWindowTextW( L"host_client" );
	_editPass.SetWindowTextW( L"1234" );
	_editCPort.SetWindowTextW( L"100" );
	_editIP.SetWindowTextW( L"211.189.19.160" );
	_editPort.SetWindowTextW( L"10000" );

	CWinThread * pThread = AfxBeginThread(ThreadFunction, this);

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
	USES_CONVERSION;

	CString id;
	_editID.GetWindowText( id );

	CString pass;
	_editPass.GetWindowText( pass );

	// 로긴
	Network::GetInstance().ReqLoginSend( (LPCSTR)T2A(id), (LPCSTR)T2A(pass) );

	Sleep(1000);

	if( Network::GetInstance()._isSuccessAuth )
	{
		OnOK();
	}
}

UINT LoginDlg::ThreadFunction(LPVOID pParam)
{
	LoginDlg *pthis = (LoginDlg*)pParam;     
	pthis->ThreadDo();               // Thread를 시작한다.

	return 0;
}

void LoginDlg::ThreadDo()
{
	while (1)
	{
		Network::GetInstance().ProcPacket();

		if( Network::GetInstance()._isSuccessAuth )
		{
			break;
		}
	}
}
