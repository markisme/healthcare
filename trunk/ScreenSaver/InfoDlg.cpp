// InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinMgr.h"
#include "InfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg dialog


CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoDlg)
	m_strInfo = _T("");
	//}}AFX_DATA_INIT
}


void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoDlg)
	DDX_Text(pDX, IDC_EDITINFO, m_strInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CInfoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg message handlers

BOOL CInfoDlg::OnInitDialog()
{

	m_strInfo = "User Notes:\r\n\r\n--1. Activate SS button:  Activate the SS even if the user has previously pressed the disable SS button.  Uses the configuration options that are currently set in the Options...Configure menu of WinMgr.\r\n\r\n--2.Disable / Enable SS buttons: The disable button disables screen saving only for the current user in the current Win session.  If the computer is rebooted or the user logs off, the SS is reenabled.  Both buttons have only a temporary effect and don't actually change any Control Panel settings.  The enable button reactivates the SS that the disable button affected.\r\n\r\n--3. Logoff button: Logs current user off Win...serves as a \"quick reboot\" simply restarting Win.\r\n\r\n--4. Reboot button: Same as choosing the Restart option from the Win Start...Shut Down menu.  I was also considering adding a button to Shut Down the computer as well.  If anyone would like me to do so, please drop a line and I will gladly add this functionality.\r\n\r\n--5. Configuration Options:  Two options are available for user configuration.  Both options choose what happens to WinMgr after the Activate SS button is pressed.  The first option is the Exit option, the second is the Minimize option.  By default, WinMgr chooses is to minimize when the Activate SS button is pressed.  These options are saved when the user presses the OK button in the WinMgr Configure dialog box.";
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}
