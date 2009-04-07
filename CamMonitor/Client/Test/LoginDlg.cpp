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
END_MESSAGE_MAP()

BOOL LoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_editID.SetWindowText( "host" );
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

	// �α� ��û
	Network::GetInstance().ReqLoginSend( idStr, passStr );

	// ���� �Ϸ� �� ���� ��ŸƮ
	while (1)
	{
		//
		Sleep( 10 );

		//
		if( Network::GetInstance()._isSuccessAuth )
		{
			// SendMessage(WM_SYSCOMMAND, SC_SCREENSAVE); //send SysCommand message calling SS
			OnOK();
			break;
		}
	}
}

UINT LoginDlg::ThreadFunction(LPVOID pParam)
{
	LoginDlg *pthis = (LoginDlg*)pParam;     
	pthis->ThreadDo(); // ��Ŷ�� �ޱ� ���� Thread ����

	return 0;
}

void LoginDlg::ThreadDo()
{
	while (1)
	{
		//
		Network::GetInstance().ProcPacket();

		//
		if( Network::GetInstance()._isSuccessAuth )
		{
			break;
		}
	}
}
