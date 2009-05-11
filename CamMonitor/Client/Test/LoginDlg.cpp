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
	std::string idStr = id;

	CString pass;
	_editPass.GetWindowText( pass );
	std::string passStr = pass;

	// 로긴 요청
	Network::GetInstance().ReqLoginSend( idStr, passStr );

	// 인증 완료 후 어플 스타트
	while (1)
	{
		//
		Sleep( 30 );

		//
		if( Network::GetInstance()._isSuccessAuth != -1 )
		{
			OnOK();
			break;
		}
	}
}

void LoginDlg::OnClose()
{
	//
	OnCancel();

	CDialog::OnClose();
}


UINT LoginDlg::ThreadFunction(LPVOID pParam)
{
	LoginDlg *pthis = (LoginDlg*)pParam;     
	pthis->ThreadDo(); // 패킷을 받기 위한 Thread 시작

	return 0;
}

void LoginDlg::ThreadDo()
{
	//
	Network::GetInstance()._isSuccessAuth = -1;

	//
	while (1)
	{
		//
		Network::GetInstance().ProcPacket();

		//
		if( Network::GetInstance()._isSuccessAuth != -1 )
		{
			break;
		}
	}
}