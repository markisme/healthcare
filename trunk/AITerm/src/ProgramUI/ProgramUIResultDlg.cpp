// ProgramUIDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ProgramUI.h"
#include "ProgramUIResultDlg.h"
#include "TestCase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CProgramUIDlg 대화 상자
CProgramUIResultDlg::CProgramUIResultDlg(CWnd* pParent /*=NULL*/)
: CDialog(CProgramUIResultDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProgramUIResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_QS, _questionCombo);
	DDX_Control(pDX, IDC_EDIT_QS, _qs);
	DDX_Control(pDX, IDC_LIST_NAMEENTITY, _nameEntity);
	DDX_Control(pDX, IDC_LIST_TEMPLATE, _template);
	DDX_Control(pDX, IDC_LIST_DB, _db);
	DDX_Control(pDX, IDC_EDIT_SQL, _sql);
}

BEGIN_MESSAGE_MAP(CProgramUIResultDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_QS, &CProgramUIResultDlg::OnCbnSelchangeComboQs)
END_MESSAGE_MAP()


// CProgramUIDlg 메시지 처리기
BOOL CProgramUIResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	RefreshResult();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

//
void CProgramUIResultDlg::OnPaint()
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

HCURSOR CProgramUIResultDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProgramUIResultDlg::OnCbnSelchangeComboQs()
{
	RefreshResult();
}

void CProgramUIResultDlg::RefreshResult()
{
	// 콤보박스 초기화
	int index = _questionCombo.GetCurSel();
	if( index < 0 ) index = 0;
	_questionCombo.ResetContent();

	_qs.SetWindowText( "" );
	_sql.SetWindowText( "" );

	RefreshQuestion( index );
	RefreshNameEntity( index );
	RefreshTemplate( index );
	RefreshSQL( index );
	RefreshDB( index );

	_questionCombo.SetCurSel( index );
}

void CProgramUIResultDlg::RefreshQuestion( int index )
{
	// 질문 xml 로드
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
		_qs.SetWindowText( qs.c_str() );
	}
}

void CProgramUIResultDlg::RefreshNameEntity( int index )
{
	Datas dataList;

	// 결과 xml 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/ResultNamedEntityRecognition.xml";
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

		//
		std::string item = "question" + qsNum;
		_questionCombo.AddString( item.c_str() );

		if( index != num )
		{
			continue;
		}

		//
		int tagCount = qsNode->GetNodeCount( "tag" );
		for( int cnt = tagCount-1; cnt >= 0; cnt-- )
		{
			const XmlNode * tagNode = qsNode->GetNode( "tag", cnt );

			std::string name = tagNode->GetAttribute( "name" );
			std::string curpos = tagNode->GetAttribute( "curpos" );
			std::string deppos = tagNode->GetAttribute( "deppos" );
			std::string part = tagNode->GetAttribute( "part" );
			std::string word = tagNode->GetText();

			DataRow data;
			data.push_back( name );
			data.push_back( curpos );
			data.push_back( deppos );
			data.push_back( part );
			data.push_back( word );
			dataList.push_back( data );
		}
	}

	DataRow fieldList;
	fieldList.push_back( "name" );
	fieldList.push_back( "curpos" );
	fieldList.push_back( "deppos" );
	fieldList.push_back( "part" );
	fieldList.push_back( "word" );

	PrintList( _nameEntity, fieldList, dataList );
}

void CProgramUIResultDlg::RefreshTemplate( int index )
{
	Datas dataList;

	// 결과 xml 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/ResultSemanticTemplateProcess.xml";
	xmlDoc.LoadFile( path.c_str() );

	//
	const XmlNode *resNode = xmlDoc.GetNode( "resource");

	//
	int nodeCount = resNode->GetNodeCount( "question" );
	for( int num = 0; num < nodeCount; num++ )
	{	
		//
		const XmlNode * qsNode = resNode->GetNode( "question", num );
		std::string qsNum = qsNode->GetAttribute( "questionno" );
		std::string tpNum = qsNode->GetAttribute( "templateno" );

		if( index != num )
		{
			continue;
		}

		//
		int slotCount = qsNode->GetNodeCount( "slot" );
		for( int cnt = slotCount-1; cnt >= 0; cnt-- )
		{
			const XmlNode * slotNode = qsNode->GetNode( "slot", cnt );

			std::string type = slotNode->GetAttribute( "type" );
			std::string tagname = slotNode->GetAttribute( "tagname" );
			std::string word = slotNode->GetText();

			DataRow data;
			data.push_back( tpNum );
			data.push_back( type );
			data.push_back( tagname );
			data.push_back( word );
			dataList.push_back( data );
		}
	}

	DataRow fieldList;
	fieldList.push_back( "templateNo" );
	fieldList.push_back( "type" );
	fieldList.push_back( "tagname" );
	fieldList.push_back( "word" );

	PrintList( _template, fieldList, dataList );
}

void CProgramUIResultDlg::RefreshSQL( int index )
{
	// 결과 xml 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/ResultSQL.xml";
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

		std::string sql = qsNode->GetText();
		_sql.SetWindowText( sql.c_str() );
	}
}

void CProgramUIResultDlg::RefreshDB( int index )
{
	Datas dataList;
	DataRow fieldList;

	// 결과 xml 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/ResultDB.xml";
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

		//
		int rowCount = qsNode->GetNodeCount( "row" );
		for( int cnt = rowCount-1; cnt >= 0; cnt-- )
		{
			const XmlNode * rowNode = qsNode->GetNode( "row", cnt );

			DataRow dataRow;

			//
			int dataCount = rowNode->GetNodeCount( "data" );
			for( int k = dataCount-1; k >= 0; k-- )
			{
				const XmlNode * dataNode = rowNode->GetNode( "data", k );

				std::string name = dataNode->GetAttribute( "name" );
				std::string data = dataNode->GetText();

				if( cnt == 0 )
				{
					fieldList.push_back( name );
				}

				dataRow.push_back( data );
			}

			dataList.push_back( dataRow );
		}
	}

	PrintList( _db, fieldList, dataList );
}

void CProgramUIResultDlg::PrintList( CListCtrl & list, DataRow & fieldList, Datas & dataList )
{
	// 테이블 초기화
	list.DeleteAllItems();
	while(list.DeleteColumn(0)) {} 

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
		list.InsertColumn( i, &lvColumn );
	}

	list.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	// 테이블 데이터 출력
	int count = dataList.size();
	for( int num = 0; num < count; num++ )
	{
		DataRow & data = dataList[ num ];

		int size = data.size();
		for( int cnt = 0; cnt < size; cnt++ )
		{
			CString itemText( data[cnt].c_str() );

			if( cnt == 0 )
			{
				LV_ITEM lvItem;
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = 0;
				lvItem.iSubItem = 0;
				lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
				list.InsertItem(&lvItem);
			}
			else
			{
				LV_ITEM lvItem;
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = 0;
				lvItem.iSubItem = cnt;
				lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
				list.SetItem(&lvItem);
			}
		}
	}
}

