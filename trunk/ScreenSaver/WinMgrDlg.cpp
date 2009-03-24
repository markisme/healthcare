// WinMgrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinMgr.h"
#include "WinMgrDlg.h"
#include <dos.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinMgrDlg dialog

CWinMgrDlg::CWinMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinMgrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWinMgrDlg)
	m_strEditSSStatus = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinMgrDlg)
	DDX_Text(pDX, IDC_EDITSSSTATUS, m_strEditSSStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinMgrDlg, CDialog)
	//{{AFX_MSG_MAP(CWinMgrDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONREBOOT, OnButtonreboot)
	ON_BN_CLICKED(IDC_BUTTONDISABLESS, OnButtondisabless)
	ON_BN_CLICKED(IDC_BUTTONENABLESS, OnButtonenabless)
	ON_BN_CLICKED(IDC_BUTTONACTIVATESS, OnButtonactivatess)
	ON_BN_CLICKED(IDC_BUTTONLOGOFF, OnButtonlogoff)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(IDD_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_HELP_INFO, OnHelpInfo)
	ON_COMMAND(ID_OPTIONS_CONFIG, OnOptionsConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinMgrDlg message handlers

BOOL CWinMgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CheckSSStatus();	//Call the SS Status function to get current SS state
						//& update the dialog with the value
	
	ButtonUpdateSSConfig(); //Call func to update the values for ActivateSS button
	
	CreateToolTips();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinMgrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinMgrDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinMgrDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWinMgrDlg::CreateToolTips()
{
		// Create the ToolTip control.
		m_tooltip.Create(this);

		// TODO: Use one of the following forms to add controls:
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTONDISABLESS), IDS_DISABLESS);
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTONENABLESS), IDS_ENABLESS);
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTONACTIVATESS), IDS_ACTIVATESS);
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTONLOGOFF), IDS_WINLOGOFF);
		m_tooltip.AddTool(GetDlgItem(IDC_BUTTONREBOOT), IDS_WINREBOOT);
		m_tooltip.AddTool(GetDlgItem(IDC_EDITSSSTATUS), IDS_SSSTATUS);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		
		m_tooltip.Activate(TRUE);

} //End CreateToolTips()

void CWinMgrDlg::ButtonUpdateSSConfig()
{
	///GET VALUES FROM FILE FOR CONFIG DLG	
	CString strSection       = "SSConfig";
	CString strIntItem       = "SSValue";   
	
	CWinApp* pApp = AfxGetApp();

	//Get value written in file, if exist...if not exist, value is 1
	int nRadioVal = pApp->GetProfileInt(strSection, strIntItem, 1);

	//Code sets default val of 1 or val from file to m_iSSValue so SSActivate() works
	m_dConfigDlg.m_iSSValue = nRadioVal;
	//UpdateData(TRUE);

} //End ButtonUpdateSSConfig()

void CWinMgrDlg::OnButtonactivatess() 
{
	// TODO: Add your control notification handler code here
		//Add code to check the state of the two check boxes for ActivateSS options
		//if()
	int ActSSVal;
	ActSSVal = m_dConfigDlg.m_iSSValue;

		switch(ActSSVal)
		{
		case 0: //Exit radio button
			ActivateSS();
			OnOK(); //Now Exit WinMgr
			break;

		case 1: //Minimize radio button
			ShowWindow(SW_MINIMIZE);
			ActivateSS();
			break;
		}

} //End OnButtonactivatess()

void CWinMgrDlg::ActivateSS(void)
{
/*** Code contributed by anonymous source on CodeGuru discussion board...Thank You!***/

		CDialog dlg; //Create temp modal dialog
		dlg.Create(IDD_LAUNCHSS_DLG); //Create temp dialog using IDD_LAUNCHSS_DLG dlg
		dlg.SendMessage(WM_SYSCOMMAND, SC_SCREENSAVE); //send SysCommand message calling SS
		dlg.DestroyWindow(); //destroy temporary dialog window
		
	//**Notice no call to DoModal()...therefore no window is shown**
	
	//OnSysCommand(SC_SCREENSAVE, SC_SCREENSAVE); //Other way to activate SS...doesn't work here
		
/*** Code contributed by anonymous source on CodeGuru discussion board...Thank You!***/
}


void CWinMgrDlg::OnButtondisabless() 
{
	// TODO: Add your control notification handler code here
	// NOTE: The following code was corrected by Kunz Dieter...Thanks!

	BOOL pvParam; //declare local bool variable for the SystemParametersInfo func()

	BOOL l_RetVal_b = SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &pvParam, 0);
	
  // the function call was ok - only in this case the pvParam holds valid data
	if(l_RetVal_b)
	{
		if (pvParam == FALSE){
		//If SS already disabled, do nothing
		} //end if
	
		//If SS enabled, disable it
	else if (pvParam != FALSE){
		
		//Disable screen saving capability for the current user session
		SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, 0, 0); 
		
		//Call CheckSSStatus() func to check performance of SystemParameterInfo() func 
		//& update the dialog's display with current SS status
	
		CheckSSStatus();
	}
	
	} //end top if
	
} //End OnButtondisabless()

void CWinMgrDlg::OnButtonenabless() 
{
	// TODO: Add your control notification handler code here
	// NOTE: The following code was corrected by Kunz Dieter...Thanks!
	
	BOOL pvParam; //declare local bool variable for the SystemParametersInfo func()

	BOOL l_RetVal_b = SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &pvParam, 0);
	
  // the function call was ok - only in this case the pvParam holds valid data
	if(l_RetVal_b)
	{
		if (pvParam != FALSE){
		//If SS already enabled, do nothing
		} //end if
		
		//If SS disabled, enable it
		else if (pvParam == FALSE){
			//Enable screen saving capability for the current user session
		SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, 0, 0); 
		
		//Call CheckSSStatus() func to check performance of SystemParameterInfo() func 
		//& update the dialog's display with current SS status
	
		CheckSSStatus();
		} //end else if	
	} //end top if

} //end OnButtonenabless()

void CWinMgrDlg::OnButtonlogoff() 
{
	// TODO: Add your control notification handler code here
	
	//Logoff/Restart Win
	//MS Word won't prompt to save changes (according to MSDN documentation)
	ExitWindowsEx(EWX_LOGOFF,0);
	
	//Close prog after logoff call
	OnOK();
}

void CWinMgrDlg::OnButtonreboot() 
{
	// TODO: Add your control notification handler code here
	
	//Forced reboot code:
	//ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0L);
	
	//Non-forced reboot code:
	//MS Word won't prompt to save changes (according to MSDN documentation)
	ExitWindowsEx(EWX_REBOOT ,0);

	//Close prog after reboot call
	OnOK();
}

void CWinMgrDlg::CheckSSStatus(void)
{
	
	BOOL pvParam; //declare local bool variable for the SystemParametersInfo func()

	BOOL l_RetVal_b = SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &pvParam, 0);
	
  // the function call was ok - only in this case the pvParam holds valid data
	if(l_RetVal_b)
	{
		if (pvParam == FALSE){
		  
		  //If SS disabled, send "DISABLED" to read-only edit control and update the display
			m_strEditSSStatus = "DISABLED"; //m_EditSSStatus is a member Cstring var for the dialog's edit box
			UpdateData(FALSE);
		} //end if

		else if (pvParam != FALSE){  //NOTE: also tried pvParam here as test value

		//If SS enabled, send "ENABLED" to read-only edit control and update the display
		  
		m_strEditSSStatus = "ENABLED"; //m_EditSSStatus is a member Cstring var for the dialog's edit box
		UpdateData(FALSE);
		} //end else if
	} //end top if
	
} //end CheckSSStatus()

void CWinMgrDlg::OnHelpAbout() 
{
	// TODO: Add your command handler code here
	//Declare instance of About window
	CAboutDlg dlgAbout;

	//Show About window
	dlgAbout.DoModal();
}

void CWinMgrDlg::OnFileExit() 
{
	// TODO: Add your command handler code here
	OnOK();
}

void CWinMgrDlg::OnHelpInfo() 
{
	// TODO: Add your command handler code here
	//Declare instance of Config window
	CDialog CInfoDlg;

	//Show Config window
	
	//dlgConfig.DoModal();
	m_dInfoDlg.DoModal();
}

void CWinMgrDlg::OnOptionsConfig() 
{
	// TODO: Add your command handler code here
	//Declare instance of Config window
	CDialog dlgConfig;

	//Show Config window
	
	//dlgConfig.DoModal();
	m_dConfigDlg.DoModal();

} //End OnOptionsConfig

BOOL CWinMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	// CG:  This was added by System Info Component.

	// CG: Following block was added by System Info Component.
	{
		CString strFreeDiskSpace;
		CString strFreeMemory;
		CString strFmt;

		// Fill available memory
		MEMORYSTATUS MemStat;
		MemStat.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&MemStat);
		strFmt.LoadString(CG_IDS_PHYSICAL_MEM);
		strFreeMemory.Format(strFmt, MemStat.dwTotalPhys / 1024L);

		//TODO: Add a static control to your About Box to receive the memory
		//      information.  Initialize the control with code like this:
		 SetDlgItemText(IDC_PHYSICAL_MEM, strFreeMemory);

		// Fill disk free information
		struct _diskfree_t diskfree;
		int nDrive = _getdrive(); // use current default drive
		if (_getdiskfree(nDrive, &diskfree) == 0)
		{
			strFmt.LoadString(CG_IDS_DISK_SPACE);
			strFreeDiskSpace.Format(strFmt,
				(DWORD)diskfree.avail_clusters *
				(DWORD)diskfree.sectors_per_cluster *
				(DWORD)diskfree.bytes_per_sector / (DWORD)1024L,
				nDrive-1 + _T('A'));
		}
		else
			strFreeDiskSpace.LoadString(CG_IDS_DISK_SPACE_UNAVAIL);

		//TODO: Add a static control to your About Box to receive the memory
		//      information.  Initialize the control with code like this:
		 SetDlgItemText(IDC_DISK_SPACE, strFreeDiskSpace);
	}

	return TRUE;	// CG:  This was added by System Info Component.

}
