#include "stdafx.h"
#include "MainDlg.h"


IMPLEMENT_DYNAMIC(MainDlg, ScrollDlg)
MainDlg::MainDlg(CWnd* pParent /*=NULL*/)	: ScrollDlg(MainDlg::IDD, pParent)
{	
}

MainDlg::~MainDlg()
{
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	ScrollDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_LIST, _userList);
	DDX_Control(pDX, IDC_DATA_LIST, _dataList);
}

BEGIN_MESSAGE_MAP(MainDlg, ScrollDlg)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_USER_LIST, &MainDlg::OnLbnSelchangeUserList)
END_MESSAGE_MAP()

BOOL MainDlg::OnInitDialog()
{
	ScrollDlg::OnInitDialog();

	wchar_t name1[20] = L"김태현";
	wchar_t name2[20] = L"정도영";
	wchar_t name3[20] = L"한규혁";

	_userList.AddString( (LPCTSTR)(name1) );
	_userList.AddString( (LPCTSTR)(name2) );
	_userList.AddString( (LPCTSTR)(name3) );

	LV_COLUMN lvColumn;
	wchar_t * headerName[3] = { L"시간", L"혈류량", L"온도" };		

	for( int i = 0; i < 3; i++ )
	{
		lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_CENTER;
		lvColumn.pszText = headerName[i];
		lvColumn.iSubItem = i;
		lvColumn.cx = 70;
		_dataList.InsertColumn( i, &lvColumn );
	}

	_dataList.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	m_wndView = new CTestView();

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 350, 200), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("보기 창을 만들지 못했습니다.\n");
		return -1;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL MainDlg::Init( CWnd* parent )
{
	this->Create( IDD_FORMVIEW, parent );
	this->SetParent( parent );
	this->ShowWindow( true );

	CRect rect;
	parent->GetClientRect( rect );

	this->MoveWindow( rect, TRUE );

	return true;
}

void MainDlg::Update( void )
{
}

void MainDlg::RefreshControls()
{
}

void MainDlg::OnSize(UINT nType, int cx, int cy)
{
	ScrollDlg::OnSize(nType, cx, cy);

	//CRect mainRect;
	//GetWindowRect( mainRect );
	//ScreenToClient( mainRect );
	//int mainWidth = mainRect.Width();
	//int mainHeight = mainRect.Height();
}

int MainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ScrollDlg::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void MainDlg::OnLbnSelchangeUserList()
{
	_dataList.DeleteAllItems();

	CString itemText;
	itemText = "1분";

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = 0;
	lvItem.iSubItem = 0;
	lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
	_dataList.InsertItem(&lvItem);

	itemText = "90회";
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = 0;
	lvItem.iSubItem = 1;
	lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
	_dataList.SetItem(&lvItem);

	itemText = "36도";
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = 0;
	lvItem.iSubItem = 2;
	lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
	_dataList.SetItem(&lvItem);
}
