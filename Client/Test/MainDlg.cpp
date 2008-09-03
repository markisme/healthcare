#include "stdafx.h"
#include "MainDlg.h"
#include "MainFrm.h"

#include "RakPeerInterface.h"
#include "Network.h"


IMPLEMENT_DYNAMIC(MainDlg, CDialog)
MainDlg::MainDlg(CWnd* pParent /*=NULL*/)	: CDialog(MainDlg::IDD, pParent)
{	
}

MainDlg::~MainDlg()
{
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_LIST, _dataList);
	DDX_Control(pDX, IDC_USER_LIST, _userList);
	DDX_Control(pDX, IDC_USERDATA_LIST, _userDataList);
	DDX_Control(pDX, IDC_IMAGE, _image);

	DDX_Control(pDX, IDC_COMBO_YEAR, _yearList);
	DDX_Control(pDX, IDC_COMBO_MONTH, _monthList);
	DDX_Control(pDX, IDC_COMBO_DAY, _dayList);
	DDX_Control(pDX, IDC_COMBO_HOUR, _hourList);
}

BEGIN_MESSAGE_MAP(MainDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_USER_LIST, &MainDlg::OnLbnSelchangeUserList)
	ON_CBN_SELCHANGE(IDC_COMBO_HOUR, &MainDlg::OnCbnSelchangeComboHour)
END_MESSAGE_MAP() 

BOOL MainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( Network::GetInstance()._isHost )
	{
		_userList.ShowWindow( TRUE );
		_userDataList.ShowWindow( TRUE );
		_image.ShowWindow( TRUE );		
	}
	else
	{
		_userList.ShowWindow( FALSE );
		_userDataList.ShowWindow( FALSE );
		_image.ShowWindow( FALSE );
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
			lvColumn.cx = 70*(i+1);
			_userDataList.InsertColumn( i, &lvColumn );
		}

		_userDataList.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	}

	m_wndView = new CTestView;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 800, 300), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("보기 창을 만들지 못했습니다.\n");
		return -1;
	}

	//
	{
		CTime t = CTime::GetCurrentTime();
		int year = t.GetYear();
		int month = t.GetMonth();
		int day = t.GetDay();
		int hour = t.GetHour();
		int curSel = 0;

		int index = 0;
		for(int num = 2000; num < 2009; num++, index++)
		{
			CString itemText;
			itemText.Format(L"%d 년", num);

			_yearList.AddString( itemText );

			if( num == year )
			{
				curSel = index;
			}
		}
		_yearList.SetCurSel( curSel );

		index = 0;
		for(int num = 1; num < 13; num++, index++)
		{
			CString itemText;
			if( num < 10 )
			{
				itemText.Format(L"0%d 월", num);
			}
			else
			{
				itemText.Format(L"%d 월", num);
			}

			_monthList.AddString( itemText );

			if( num == month )
			{
				curSel = index;
			}
		}
		_monthList.SetCurSel(curSel);

		index = 0;
		for(int num = 1; num < 32; num++,index++)
		{
			CString itemText;
			if( num < 10 )
			{
				itemText.Format(L"0%d 일", num);
			}
			else
			{
				itemText.Format(L"%d 일", num);
			}

			_dayList.AddString( itemText );

			if( num == day )
			{
				curSel = index;
			}
		}
		_dayList.SetCurSel(curSel);

		index = 0;
		for(int num = 0; num < 24; num++,index++)
		{
			CString itemText;
			if( num < 10 )
			{
				itemText.Format(L"0%d 시", num);
			}
			else
			{
				itemText.Format(L"%d 시", num);
			}

			_hourList.AddString( itemText );

			if( num == hour )
			{
				curSel = index;
			}
		}
		_hourList.SetCurSel(curSel);
	}

	_userList.SetCurSel( 0 );
	OnLbnSelchangeUserList();
	OnCbnSelchangeComboHour();

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL MainDlg::Init( CWnd* parent )
{
	//
	m_mainFrame = (CMainFrame*)parent;

	//
	this->Create( IDD_FORMVIEW, parent );
	this->SetParent( parent );
	this->ShowWindow( true );

	CRect rect;
	parent->GetClientRect( rect );

	this->MoveWindow( 0, 0, 800, 600, TRUE );

	return true;
}

void MainDlg::Update( void )
{
}

void MainDlg::RefreshControls()
{
	// 유저 인포 데이터 갱신
	int index = _userList.GetCurSel();
	_userList.ResetContent();

	UserList & userInfoList = Network::GetInstance().GetUserInfoList();
	int count = userInfoList.size();
	for( int num = 0; num < count; num++ )
	{
		UserInfo & userInfo = userInfoList[ num ];
		
		CString name(userInfo._userName.c_str());
		_userList.AddString( (LPTSTR)(LPCTSTR)(name) );
		_userList.SetItemData( num, atoi(userInfo._userNo.c_str()) );
	}
	_userList.SetCurSel( index );

	// 유저 데이터 갱신
	_dataList.DeleteAllItems();

	UserDataList & userDataList = Network::GetInstance().GetUserDataList();
	count = userDataList.size();

	for(int num = 0; num<count; num++)
	{
		UserData & data = userDataList[ num ];

		CString yearStr;
		_yearList.GetLBText( _yearList.GetCurSel(), yearStr );

		CString monthStr;
		_monthList.GetLBText( _monthList.GetCurSel(), monthStr );

		CString dayStr;
		_dayList.GetLBText( _dayList.GetCurSel(), dayStr );

		CString hourStr;
		_hourList.GetLBText( _hourList.GetCurSel(), hourStr );

		std::string curYear = data._year + " 년";
		std::string curMonth = data._month + " 월";
		std::string curDay = data._day + " 일";
		std::string curHour = data._hour + " 시";

		if( CString(curYear.c_str()) == yearStr &&
			CString(curMonth.c_str()) == monthStr &&
			CString(curDay.c_str()) == dayStr &&
			CString(curHour.c_str()) == hourStr )
		{
			std::string addStr = data._min + " 분";
			CString itemText1( addStr.c_str() );
			LV_ITEM lvItem;
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = 0;
			lvItem.iSubItem = 0;
			lvItem.pszText = (LPTSTR)(LPCTSTR)itemText1;
			_dataList.InsertItem(&lvItem);

			addStr = data._value + " 회";
			CString itemText2( addStr.c_str() );
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = 0;
			lvItem.iSubItem = 1;
			lvItem.pszText = (LPTSTR)(LPCTSTR)itemText2;
			_dataList.SetItem(&lvItem);

			addStr = data._temp + " 도";
			CString itemText3( addStr.c_str() );
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = 0;
			lvItem.iSubItem = 2;
			lvItem.pszText = (LPTSTR)(LPCTSTR)itemText3;
			_dataList.SetItem(&lvItem);
		}
	}
}

void MainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

int MainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void MainDlg::OnLbnSelchangeUserList()
{
	int curindex = _userList.GetCurSel();
	if( curindex >= 0 )
	{
		int userNo = (int)_userList.GetItemData( curindex );

		UserList & userInfoList = Network::GetInstance().GetUserInfoList();
		int index = Network::GetInstance().GetIndexForUserNo( userNo );
		UserInfo & userInfo = userInfoList[ index ];
		std::string picPath = "../Test/pic/" + userInfo._pic;
		
		CString age( userInfo._age.c_str() );
		CString sex( userInfo._sex.c_str() );
		CString tall( userInfo._tall.c_str() );
		CString weight( userInfo._weight.c_str() );
		CString blood( userInfo._blood.c_str() );
		CString tel( userInfo._tel.c_str() );
		CString pic( picPath.c_str() );

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

		itemText = "혈액형";
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 4;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
		_userDataList.InsertItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 4;
		lvItem.iSubItem = 1;
		lvItem.pszText = (LPTSTR)(LPCTSTR)blood;
		_userDataList.SetItem(&lvItem);

		itemText = "전화";
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 5;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPTSTR)(LPCTSTR)itemText;
		_userDataList.InsertItem(&lvItem);

		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = 5;
		lvItem.iSubItem = 1;
		lvItem.pszText = (LPTSTR)(LPCTSTR)tel;
		_userDataList.SetItem(&lvItem);

		HBITMAP hBmp = (HBITMAP)LoadImage(NULL, pic, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		_image.SetBitmap( hBmp );
	}

	OnCbnSelchangeComboHour();
}
void MainDlg::OnCbnSelchangeComboHour()
{
	int curindex = _userList.GetCurSel();
	if( curindex >= 0 )
	{
		int userNo = (int)_userList.GetItemData( curindex );
		Network::GetInstance().ReqGetUserDataSend( userNo );
	}
}
