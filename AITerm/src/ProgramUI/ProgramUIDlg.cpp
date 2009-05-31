// ProgramUIDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ProgramUI.h"
#include "ProgramUIDlg.h"
#include "ProgramUIResultDlg.h"
#include "TestCase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CProgramUIDlg 대화 상자
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


// CProgramUIDlg 메시지 처리기
BOOL CProgramUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// DB 접속 정보
	DBConnectInfo info;
	info.ip = "211.189.19.160";
	info.user = "root";
	info.pass = "1234";
	info.port = 3306;
	info.dbName = "AITerm";

	// DB 정보 초기화(싱글턴)
	DBConnector::GetInstance().Init( info );

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

//
void CProgramUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
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
	// 질문처리
	TestCase::GetInstance().TestStart( path );

	// 답변 갱신
	RefreshAnswer();
}

void CProgramUIDlg::RefreshAnswer()
{
	// 콤보박스 초기화
	int index = _questionCombo.GetCurSel();
	if( index < 0 ) index = 0;
	_questionCombo.ResetContent();

	// 테이블 초기화
	_table.DeleteAllItems();
	while(_table.DeleteColumn(0)) {} 

	// 정답 초기화
	_answer.SetWindowText( "" );

	// 질문 xml 로드
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

	// 질문 템플릿 xml 로드
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
			// Question template node들을 돌며,
			const XmlNode * qsNode = resNode->GetNode( "question", num );
			std::string qsNum = qsNode->GetAttribute( "no" );

			const XmlNode * tableNode = qsNode->GetNode( "table" );
			std::string table = tableNode->GetText();
			const XmlNode * ansNode = qsNode->GetNode( "answer" );
			std::string answer = ansNode->GetText();

			std::string item = "question" + qsNum;
			_questionCombo.AddString( item.c_str() );

			// 현재 아이템 출력
			if( index == num )
			{
				// 테이블 출력
				PrintTable( table );

				// 정답 출력
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

	// 테이블 출력
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

	// 테이블 데이터 출력
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
		AfxMessageBox( "질문을 입력해 주세요." );
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

	// 질문 처리
	ProcessQuestion( path );
}

void CProgramUIDlg::OnBnClickedButtonLoad()
{
	// 파일 오픈
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, "XML Files (*.xml)|*.xml|All Files (*.*)|*.*||");
	CString prePath = dlg.GetOFN().lpstrInitialDir;
	dlg.GetOFN().lpstrInitialDir = ".\\";
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	CString FileName = dlg.GetFileName();

	// Questions로 카피
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

	// 질문 처리
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
