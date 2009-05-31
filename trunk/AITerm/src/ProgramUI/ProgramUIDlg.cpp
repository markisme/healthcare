// ProgramUIDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ProgramUI.h"
#include "ProgramUIDlg.h"
#include "ProgramUIResultDlg.h"
#include "TestCase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CProgramUIDlg ��ȭ ����
CProgramUIDlg::CProgramUIDlg(CWnd* pParent /*=NULL*/)
: CDialog(CProgramUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProgramUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ANSWER, _answer);
	DDX_Control(pDX, IDC_EDIT_QUESTION, _question);
	DDX_Control(pDX, IDC_COMBO_QUESTION, _questionCombo);
	DDX_Control(pDX, IDC_LIST_TABLE, _table);
}

BEGIN_MESSAGE_MAP(CProgramUIDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_QUESTION, &CProgramUIDlg::OnBnClickedButtonQuestion)
	ON_CBN_SELCHANGE(IDC_COMBO_QUESTION, &CProgramUIDlg::OnCbnSelchangeComboQuestion)
	ON_BN_CLICKED(IDC_BUTTON_RESULT, &CProgramUIDlg::OnBnClickedButtonResult)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CProgramUIDlg::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CProgramUIDlg �޽��� ó����
BOOL CProgramUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// DB ���� ����
	DBConnectInfo info;
	info.ip = "211.189.19.160";
	info.user = "root";
	info.pass = "1234";
	info.port = 3306;
	info.dbName = "AITerm";

	// DB ���� �ʱ�ȭ(�̱���)
	DBConnector::GetInstance().Init( info );

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

//
void CProgramUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CProgramUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProgramUIDlg::ProcessQuestion( std::string path )
{
	// ����ó��
	TestCase::GetInstance().TestStart( path );

	// �亯 ����
	RefreshAnswer();
}

void CProgramUIDlg::RefreshAnswer()
{
	// �޺��ڽ� �ʱ�ȭ
	int index = _questionCombo.GetCurSel();
	if( index < 0 ) index = 0;
	_questionCombo.ResetContent();

	// ���̺� �ʱ�ȭ
	_table.DeleteAllItems();
	while(_table.DeleteColumn(0)) {} 

	// ���� �ʱ�ȭ
	_answer.SetWindowText( "" );

	// ���� xml �ε�
	{
		XmlDocument xmlDoc;
		std::string path = "./resource/Questions.xml";
		xmlDoc.LoadFile( path.c_str() );

		//
		const XmlNode *resNode = xmlDoc.GetNode( "resource");

		//
		int nodeCount = resNode->GetNodeCount( "question" );
		for( int num = 0; num < nodeCount; num++ )
		{	
			//
			const XmlNode * qsNode = resNode->GetNode( "question", num );
			std::string qsNum = qsNode->GetAttribute( "no" );

			if( index != num )
			{
				continue;
			}

			std::string qs = qsNode->GetText();
			_question.SetWindowText( qs.c_str() );
		}
	}

	// ���� ���ø� xml �ε�
	{
		XmlDocument xmlDoc;
		std::string path = "./resource/ResultAnswer.xml";
		xmlDoc.LoadFile( path.c_str() );

		//
		const XmlNode *resNode = xmlDoc.GetNode( "resource");

		//
		int nodeCount = resNode->GetNodeCount( "question" );
		for( int num = 0; num < nodeCount; num++ )
		{	
			// Question template node���� ����,
			const XmlNode * qsNode = resNode->GetNode( "question", num );
			std::string qsNum = qsNode->GetAttribute( "no" );

			const XmlNode * tableNode = qsNode->GetNode( "table" );
			std::string table = tableNode->GetText();
			const XmlNode * ansNode = qsNode->GetNode( "answer" );
			std::string answer = ansNode->GetText();

			std::string item = "question" + qsNum;
			_questionCombo.AddString( item.c_str() );

			// ���� ������ ���
			if( index == num )
			{
				// ���̺� ���
				PrintTable( table );

				// ���� ���
				_answer.SetWindowText( answer.c_str() );
			}

			_questionCombo.SetCurSel( index );
		}
	}
}

void CProgramUIDlg::PrintTable( std::string word )
{
	if( word.length() <= 0 )
	{
		return;
	}

	Table table;

	int end = 0;
	while( ( end = word.find("#") ) > 0 )
	{
		int start = word.find("@");
		std::string data = word.substr(start,end-start);
		word.erase(start,++end);

		int no = data.find("$");
		std::string snum = data.substr(1,no-1);
		int num = atoi( snum.c_str() );

		int mid = data.find(":");
		std::string fieldName = data.substr(++no,mid-no);
		std::string fieldData = data.substr(mid+1,data.size()-mid);

		TableData tData;
		tData._rowNum = num;
		tData._fieldName = fieldName;
		tData._fieldData = fieldData;
		table.push_back( tData );
	}

	std::vector<std::string> fieldList;
	GetFieldList( table, fieldList );

	// ���̺� ���
	LV_COLUMN lvColumn;

	int cnt = fieldList.size();
	for( int i = 0; i < cnt; i++ )
	{
		lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_CENTER;
		lvColumn.pszText = (LPSTR)fieldList[i].c_str();
		lvColumn.iSubItem = i;
		lvColumn.cx = 70;
		_table.InsertColumn( i, &lvColumn );
	}

	_table.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	int rowNo = 0;

	// ���̺� ������ ���
	int count = table.size();
	for( int num = 0; num < count; num++ )
	{
		TableData & tData = table[ num ];
		int no = GetFieldNo( tData._fieldName, fieldList );

		if( rowNo == tData._rowNum )
		{
			LV_ITEM lvItem;
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = 0;
			lvItem.iSubItem = 0;
			lvItem.pszText = (LPTSTR)(LPCTSTR)"";
			_table.InsertItem(&lvItem);
			rowNo++;
		}

		CString  itemText( tData._fieldData.c_str() );
		LV_ITEM lvItem;
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 0;
		lvItem.iSubItem = no;
		lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
		_table.SetItem(&lvItem);
	}
}

void CProgramUIDlg::GetFieldList( Table & table, std::vector<std::string> & fieldList )
{
	int count = table.size();
	for( int num = 0; num < count; num++ )
	{
		TableData & tData = table[ num ];
		std::string name = tData._fieldName;

		bool same = false;
		int size = fieldList.size();
		for( int cnt = 0; cnt < size; cnt++ )
		{
			std::string fName = fieldList[ cnt ];
			if( fName == name )
			{
				same = true;
			}
		}
		if( !same )
		{
			fieldList.push_back( name );
		}
	}
}

int CProgramUIDlg::GetFieldNo( std::string fieldName, std::vector<std::string> & fieldList )
{
	int count = fieldList.size();
	for( int num = 0; num < count; num++ )
	{
		std::string fName = fieldList[ num ];
		if( fName == fieldName )
		{
			return num;
		}
	}

	return -1;
}

//
void CProgramUIDlg::OnBnClickedButtonQuestion()
{
	//
	CString question;
	_question.GetWindowText( question );
	std::string qsStr = question;

	if( qsStr.length() <= 0 )
	{
		AfxMessageBox( "������ �Է��� �ּ���." );
		return;
	}

	//
	XmlDocument xmlDoc;
	XmlNode * resNode = xmlDoc.AddNode( "resource" );

	XmlNode * questionNode = resNode->AddNode( "question" );
	questionNode->SetAttribute( "no", "0" );
	questionNode->SetText( qsStr.c_str(), XmlNode::NUMBER );

	std::string path = "./resource/Questions.xml";
	xmlDoc.SaveFile( path.c_str() );

	// ���� ó��
	ProcessQuestion( path );
}

void CProgramUIDlg::OnBnClickedButtonLoad()
{
	// ���� ����
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, "XML Files (*.xml)|*.xml|All Files (*.*)|*.*||");
	CString prePath = dlg.GetOFN().lpstrInitialDir;
	dlg.GetOFN().lpstrInitialDir = ".\\";
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	CString FileName = dlg.GetFileName();

	// Questions�� ī��
	std::string srcPath = FileName;
	std::string destPath = "./resource/Questions.xml";

	CFile srcFile;
	CFile destFile;

	BOOL ret = srcFile.Open( srcPath.c_str(), CFile::modeRead|CFile::typeBinary );
	ret = destFile.Open( destPath.c_str(), CFile::modeCreate|CFile::modeWrite );

	DWORD dwfilelen = 0;
	while( dwfilelen < srcFile.GetLength() )
	{
		BYTE buf[1024];
		dwfilelen += 1024;

		int cnt = srcFile.Read( buf, 1024 );
		destFile.Write( buf, cnt );
	}

	srcFile.Close();
	destFile.Close();

	// ���� ó��
	ProcessQuestion( "./resource/Questions.xml" );
}


void CProgramUIDlg::OnCbnSelchangeComboQuestion()
{
	RefreshAnswer();
}

void CProgramUIDlg::OnBnClickedButtonResult()
{
	CProgramUIResultDlg dlg;
	dlg.DoModal();
}
