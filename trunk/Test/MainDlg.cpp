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
	DDX_Control(pDX, IDC_USERDATA_LIST, _userDataList);
	DDX_Control(pDX, IDC_IMAGE, _image);

	DDX_Control(pDX, IDC_COMBO_YEAR, _yearList);
	DDX_Control(pDX, IDC_COMBO_MONTH, _monthList);
	DDX_Control(pDX, IDC_COMBO_DAY, _dayList);
	DDX_Control(pDX, IDC_COMBO_HOUR, _hourList);
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

	{
		wchar_t name1[20] = L"김태현";
		wchar_t name2[20] = L"정도영";
		wchar_t name3[20] = L"한규혁";

		_userList.AddString( (LPCTSTR)(name1) );
		_userList.AddString( (LPCTSTR)(name2) );
		_userList.AddString( (LPCTSTR)(name3) );
	}

	{
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
	}

	{
		LV_COLUMN lvColumn;
		wchar_t * headerName[2] = { L"항목", L"내용" };		

		for( int i = 0; i < 2; i++ )
		{
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_CENTER;
			lvColumn.pszText = headerName[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = 70;
			_userDataList.InsertColumn( i, &lvColumn );
		}

		_userDataList.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	}

	m_wndView = new CTestView;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 350, 200), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("보기 창을 만들지 못했습니다.\n");
		return -1;
	}

	{
		for(int num = 2000; num < 2009; num++)
		{
			CString itemText;
			itemText.Format(L"%d 년", num);

			_yearList.AddString( itemText );
		}
		_yearList.SetCurSel(8);

		for(int num = 1; num < 13; num++)
		{
			CString itemText;
			itemText.Format(L"%d 월", num);

			_monthList.AddString( itemText );
		}
		_monthList.SetCurSel(8);

		for(int num = 1; num < 32; num++)
		{
			CString itemText;
			itemText.Format(L"%d 일", num);

			_dayList.AddString( itemText );
		}
		_dayList.SetCurSel(7);

		for(int num = 0; num < 24; num++)
		{
			CString itemText;
			itemText.Format(L"%d 시", num);

			_hourList.AddString( itemText );
		}
		_hourList.SetCurSel(21);
	}

	_userList.SetCurSel( 0 );
	OnLbnSelchangeUserList();

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
	Invalidate();

	_dataList.DeleteAllItems();

	for(int num=1; num<61; num++)
	{
		CString itemText;
		itemText.Format(L"%d 분", num);			

		LV_ITEM lvItem;
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 0;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
		_dataList.InsertItem(&lvItem);

		itemText.Format(L"%d 회", 90 - rand()%10 );
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 0;
		lvItem.iSubItem = 1;
		lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
		_dataList.SetItem(&lvItem);

		float th = 37.f - (float)(rand()%10)/10.f;
		itemText.Format(L"%f 도", th );
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 0;
		lvItem.iSubItem = 2;
		lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
		_dataList.SetItem(&lvItem);
	}

	{
		CString age;
		CString sex;
		CString tall;
		CString weight;
		CString pic;

		int index = _userList.GetCurSel();
		switch(index)
		{
		case 0:
			age = "25";
			sex = "남";
			tall = "183";
			weight = "86";
			pic = "../Test/pic/kim.bmp";
			break;
		case 1:
			age = "27";
			sex = "남";
			tall = "170";
			weight = "60";
			pic = "../Test/pic/jung.bmp";
			break;
		case 2:
			age = "28";
			sex = "남";
			tall = "171";
			weight = "67";
			pic = "../Test/pic/han.bmp";
			break;
		}

		_userDataList.DeleteAllItems();

		LV_ITEM lvItem;
		CString itemText;
		itemText = "나이";
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 0;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
		_userDataList.InsertItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 0;
		lvItem.iSubItem = 1;
		lvItem.pszText = (LPTSTR)(LPCTSTR)age;
		_userDataList.SetItem(&lvItem);

		itemText = "성별";
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 1;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
		_userDataList.InsertItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 1;
		lvItem.iSubItem = 1;
		lvItem.pszText = (LPTSTR)(LPCTSTR)sex;
		_userDataList.SetItem(&lvItem);

		itemText = "키";
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 2;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
		_userDataList.InsertItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 2;
		lvItem.iSubItem = 1;
		lvItem.pszText = (LPTSTR)(LPCTSTR)tall;
		_userDataList.SetItem(&lvItem);

		itemText = "몸무게";
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 3;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
		_userDataList.InsertItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 3;
		lvItem.iSubItem = 1;
		lvItem.pszText = (LPTSTR)(LPCTSTR)weight;
		_userDataList.SetItem(&lvItem);

		HBITMAP hBmp = (HBITMAP)LoadImage(NULL, pic, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		_image.SetBitmap( hBmp );
	}
}
