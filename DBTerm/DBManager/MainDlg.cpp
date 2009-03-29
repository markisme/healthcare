#include "stdafx.h"
#include "MainDlg.h"
#include "MainFrm.h"
#include "TableTree.h"
#include "DataManagerDlg.h"

IMPLEMENT_DYNAMIC(MainDlg, CDialog)
MainDlg::MainDlg(CWnd* pParent /*=NULL*/)	: CDialog(MainDlg::IDD, pParent)
{
	_tableTree = new CTableTree();
}

MainDlg::~MainDlg()
{
	SAFE_DELETE( _tableTree );
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_TABLE, *_tableTree);
	DDX_Control(pDX, IDC_DATA_LIST, _dataList);
}

BEGIN_MESSAGE_MAP(MainDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TABLE, &MainDlg::OnTvnSelchangedTreeTable)
	ON_BN_CLICKED(IDC_INSERT_BUTTON, &MainDlg::OnBnClickedInsertButton)
	ON_BN_CLICKED(IDC_MODIFY_BUTTON, &MainDlg::OnBnClickedModifyButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &MainDlg::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON, &MainDlg::OnBnClickedRefreshButton)
END_MESSAGE_MAP() 

BOOL MainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// popup menu 초기화 ( table tree 에서 WM_CREATE 가 발생하지 않는관계로 외부에서 호출하도록 했음 )
	_tableTree->InitPopupMenu();
	_tableTree->Refresh( "test" );

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL MainDlg::Init( CWnd* parent )
{
	//
	m_mainFrame = (CMainFrame*)parent;

	//
	this->Create( IDD_MAIN_DLG, parent );
	this->SetParent( parent );
	this->ShowWindow( true );

	CRect rect;
	parent->GetClientRect( rect );

	this->MoveWindow( 0, 0, 800, 600, TRUE );

	//
	_dataList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); 

	return true;
}

void MainDlg::Update( void )
{
}

void MainDlg::RefreshControls()
{
	HTREEITEM selectedItem = _tableTree->GetSelectedItem();
	if( selectedItem )
	{
		// 테이블 네임 얻어 오기
		CString itemText = _tableTree->GetItemText( selectedItem );
		std::string tableName = itemText;

		// 헤더 갱신
		{
			// 헤더 초기화
			CHeaderCtrl * headerCtrl = _dataList.GetHeaderCtrl();
			int colCount = headerCtrl->GetItemCount();
			for( int num = 0; num < colCount; num++ )
			{
				_dataList.DeleteColumn( 0 );
			}

			// 쿼리 날려서 컬럼정보 얻기
			ColList colList;
			DBConnector::GetInstance().GetColumnsList( tableName, colList );

			// 헤더 출력 하기
			int columnsCnt = colList.size();
			for( int num = 0; num < columnsCnt; num++ )
			{
				LVCOLUMN lvColumn;
				lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
				lvColumn.fmt = LVCFMT_LEFT;
				lvColumn.pszText = (LPSTR)colList[num]._name.c_str();
				lvColumn.iSubItem = num;
				lvColumn.cx = 100;

				_dataList.InsertColumn( num, &lvColumn );
			}
		}

		// 데이터 갱신
		{
			// 데이터 초기화
			_dataList.DeleteAllItems();

			// 디비 커넥터 접속해서 데이터 가져오기
			DataList dataList;
			DBConnector::GetInstance().GetDataList( tableName, dataList );

			int dataCount = dataList.size();
			for( uint num = 0; num < dataCount; num++ )
			{
				// 가져온 데이터 출력하기
				RowData data = dataList[num];
				
				int colCnt = data._data.size();
				for( int cnt = 0; cnt < colCnt; cnt++ )
				{
					std::string text = data._data[cnt];
					
					//
					LVITEM lvItem;
					lvItem.mask = LVIF_TEXT;
					lvItem.iItem = num;
					lvItem.iSubItem = cnt;
					lvItem.pszText = (LPTSTR)text.c_str();

					if( cnt == 0 )
					{
						_dataList.InsertItem( &lvItem );
					}
					else
					{
						_dataList.SetItem( &lvItem );
					}
				}
			}
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
void MainDlg::OnTvnSelchangedTreeTable(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if( !pNMTreeView->action )
	{
		return;
	}

	RefreshControls();

	*pResult = 0;
}

void MainDlg::OnBnClickedInsertButton()
{
	HTREEITEM selectedItem = _tableTree->GetSelectedItem();
	if( selectedItem == NULL )
	{
		AfxMessageBox( "테이블을 선택해 주세요." );
		return;
	}

	// 테이블 네임 얻어 오기
	CString itemText = _tableTree->GetItemText( selectedItem );
	std::string tableName = itemText;

	DataManagerDlg dlg;
	dlg.DoModal( tableName );

	// 갱신
	RefreshControls();
}

void MainDlg::OnBnClickedModifyButton()
{
	HTREEITEM selectedItem = _tableTree->GetSelectedItem();
	if( selectedItem == NULL )
	{
		AfxMessageBox( "테이블을 선택해 주세요." );
		return;
	}

	// 현재 선택된 로우값 가져오기
	int index = (int)_dataList.GetFirstSelectedItemPosition();
	if( index <= 0 )
	{
		AfxMessageBox( "수정할 데이터를 선택해 주세요." );
		return;
	}

	// 테이블 네임 얻어 오기
	CString itemText = _tableTree->GetItemText( selectedItem );
	std::string tableName = itemText;

	DataManagerDlg dlg;
	dlg.DoModal( tableName, index-1 );

	// 갱신
	RefreshControls();
}

void MainDlg::OnBnClickedDeleteButton()
{
	HTREEITEM selectedItem = _tableTree->GetSelectedItem();
	if( selectedItem == NULL )
	{
		AfxMessageBox( "테이블을 선택해 주세요." );
		return;
	}

	// 현재 선택된 로우값 가져오기
	int index = (int)_dataList.GetFirstSelectedItemPosition();
	if( index <= 0 )
	{
		AfxMessageBox( "삭제할 데이터를 선택해 주세요." );
		return;
	}

	if( AfxMessageBox( "테이터를 삭제하시겠습니까?", MB_YESNO ) == IDNO )
	{
		return;
	}

	// 테이블 네임 얻어 오기
	CString itemText = _tableTree->GetItemText( selectedItem );
	std::string tableName = itemText;

	// 쿼리 날려서 컬럼정보 얻기
	ColList colList;
	DBConnector::GetInstance().GetColumnsList( tableName, colList );

	// 디비 커넥터 접속해서 데이터 가져오기
	DataList dataList;	
	DBConnector::GetInstance().GetDataList( tableName, dataList );

	std::string whereList;

	// 원래 정보 가져 오기
	int count = colList.size();
	for( int num = 0; num < count; num++ )
	{
		RowData data = dataList[ index-1 ];

		//
		std::string name = colList[num]._name;
		std::string text = data._data[num].c_str();
		if( text.length() != 0 )
		{
			whereList += name + "=" + "\"" + text + "\"" + " AND ";
		}
	}

	whereList.erase( whereList.size() - 5 );

	char query[1024];
	sprintf(query,"DELETE FROM %s WHERE %s;", tableName.c_str(), whereList.c_str() );
	DBConnector::GetInstance().MysqlQuery( query );

	// 갱신
	RefreshControls();	
}

void MainDlg::OnBnClickedRefreshButton()
{
	RefreshControls();
}
