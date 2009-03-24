// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinMgr.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	m_iSSValue = -5;
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Radio(pDX, IDC_RADIOEXITSS, m_iSSValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog()
{
	UpdateConfigDlgRadios();
	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		m_tooltip.AddTool(GetDlgItem(IDC_RADIOEXITSS), IDS_CONFIGRADEXIT);
		m_tooltip.AddTool(GetDlgItem(IDC_RADIOMINSS), IDS_CONFIGRADMIN);
		m_tooltip.AddTool(GetDlgItem(IDOK), IDS_CONFIGOKBUTTON);
		m_tooltip.AddTool(GetDlgItem(IDCANCEL), IDS_CONFIGCANCELBUTTON);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
} //End OnInitDialog()

void CConfigDlg::UpdateConfigDlgRadios()
{	
	CString strSection       = "SSConfig";
	CString strIntItem       = "SSValue";   
	
	CWinApp* pApp = AfxGetApp();

	//Get value written in file, if exist...if not exist, value is -1
	int nRadioVal = pApp->GetProfileInt(strSection, strIntItem, -1);
	
	//If no value exists, write default value of 1 & check minimize radio button
	if(nRadioVal == -1){
		pApp->WriteProfileInt(strSection, strIntItem, 1);
		CheckRadioButton(IDC_RADIOEXITSS, IDC_RADIOMINSS, IDC_RADIOMINSS);
		//UpdateData(TRUE);
	} //end if
	//If exist, is value 0 (exit)
	else if(nRadioVal == 0){
		CheckRadioButton(IDC_RADIOEXITSS, IDC_RADIOMINSS, IDC_RADIOEXITSS);
		//UpdateData(TRUE);
	} //end if
	//If exist, is value 1 (min)
	else if(nRadioVal == 1){
		CheckRadioButton(IDC_RADIOEXITSS, IDC_RADIOMINSS, IDC_RADIOMINSS);
		//UpdateData(TRUE);
	} //end if
	//No button selected?
	else if(GetCheckedRadioButton(IDC_RADIOEXITSS, IDC_RADIOMINSS) == 0){
		CheckRadioButton(IDC_RADIOEXITSS, IDC_RADIOMINSS, IDC_RADIOMINSS);
		//UpdateData(TRUE);
	} //end if
UpdateData(TRUE);

} //End UpdateConfigDlgRadios()

void CConfigDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
} // End OnCancel

void CConfigDlg::OnOK()
{
	// TODO: Add extra validation here
	
	UpdateData(TRUE); //Update the data before writing to file

	//Does not check file for same values, just writes current option values to the file
	CString strSection       = "SSConfig";
	CString strIntItem       = "SSValue";   
	
	CWinApp* pApp = AfxGetApp();

	int WriteVal = m_iSSValue;

	pApp->WriteProfileInt(strSection, strIntItem, WriteVal);
	
	CDialog::OnOK(); //Exit dialog
} //End OnOK()

BOOL CConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}
