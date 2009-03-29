#include "stdafx.h"
#include "TableTree.h"
#include "TableManagerDlg.h"

#define ID_FOLDERTREE_POPUPMENU_UPDATE		0x8001
#define ID_FOLDERTREE_POPUPMENU_CREATE		0x8002
#define ID_FOLDERTREE_POPUPMENU_DELETE		0x8004

IMPLEMENT_DYNAMIC(CTableTree, CTreeCtrl)
CTableTree::CTableTree()
{
}

CTableTree::~CTableTree()
{
}

BEGIN_MESSAGE_MAP(CTableTree, CTreeCtrl)			
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_COMMAND(ID_FOLDERTREE_POPUPMENU_UPDATE, OnUpdate)
	ON_COMMAND(ID_FOLDERTREE_POPUPMENU_CREATE, OnCreateTable)
	ON_COMMAND(ID_FOLDERTREE_POPUPMENU_DELETE, OnDelete)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//

void CTableTree::OnDestroy()
{
	Clear();
	CTreeCtrl::OnDestroy();
}

void CTableTree::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{	
	// 선택된 것이 없으면 팝업 메뉴 안뜸
	HTREEITEM treeItem = GetSelectedItem();
	if( treeItem == NULL )
	{
		return;
	}

	CPoint point;
	CPoint S2Cpoint;
	CPoint C2Spoint;	
	::GetCursorPos( &point ); // 현재 마우스 포인터의 좌표. (스크린좌표임)

	// 오른쪽 버튼이 입력되었을때 해당 아이템이 선택되도록 한다.
	S2Cpoint = point;
	ScreenToClient( &S2Cpoint ); // 스크린 좌표를 크라이언트 좌표로 변환...

	UINT flags = 0;
	HTREEITEM hItem = HitTest( S2Cpoint, &flags );
	if (hItem) 
	{
		SelectItem(hItem);
	}

	// 팝업메뉴를 띄운다.
	C2Spoint = point;
	::ClientToScreen( NULL, &C2Spoint );

	_popupMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, C2Spoint.x, C2Spoint.y, this );

	*pResult = 0;
}


void CTableTree::OnUpdate( void )
{
	HTREEITEM treeItem = GetSelectedItem();
	if( treeItem == NULL )
	{
		AfxMessageBox( "테이블을 선택해 주세요." );
		return;
	}

	std::string tableName = GetItemText( treeItem );

	TableManagerDlg dlg;
	dlg.DoModal( 1, tableName );

	Refresh( "test" );
}

void CTableTree::OnCreateTable( void )
{
	TableManagerDlg dlg;
	dlg.DoModal( 0, "" );

	Refresh( "test" );
}

void CTableTree::OnDelete( void )
{
	HTREEITEM treeItem = GetSelectedItem();
	if( treeItem == NULL )
	{
		AfxMessageBox( "테이블을 선택해 주세요." );
		return;
	}

	std::string tableName = GetItemText( treeItem );

	if( AfxMessageBox( "테이블을 삭제하시겠습니까?", MB_YESNO ) == IDYES )
	{
		char query[1024];
		sprintf(query,"DROP TABLE %s;", tableName.c_str());
		DBConnector::GetInstance().MysqlQuery( query );

		Refresh( "test" );
	}
}

bool CTableTree::Refresh( std::string dbName, bool expand )
{
	// 기존 tree 삭제
	Clear();

	// 테이블 가져오기
	std::vector<std::string> tableList;
	DBConnector::GetInstance().GetTableList( tableList);

	// Tree 작성
	HTREEITEM rootTreeItem = InsertTreeItem( TVI_ROOT, dbName, true );
	HTREEITEM parentItem = rootTreeItem;

	int count = tableList.size();
	for( int num = 0; num < count; num++ )
	{
		std::string tableName = tableList[ num ];
		HTREEITEM item = InsertTreeItem( parentItem, tableName, false );
		HTREEITEM parentItem = item;
	}

	if( expand )
	{
		Expand( rootTreeItem, TVE_EXPAND );
	}

	return true;
}

void CTableTree::Clear( void )
{
	HTREEITEM item = GetRootItem();
	while( item )
	{		
		HTREEITEM nextItem = GetNextItem( item, TVGN_NEXT );
		
		DeleteItem( item );
		item = nextItem;
	}
}

HTREEITEM CTableTree::InsertTreeItem( HTREEITEM parent,std::string name, bool hasChild )
{
	HTREEITEM treeItem = NULL;
	TV_INSERTSTRUCT tvis;
	memset(&tvis, 0, sizeof(TV_INSERTSTRUCT));

	CString item( name.c_str() );

	tvis.hParent = parent;
	tvis.hInsertAfter = TVI_LAST;
	//	tvis.item.mask = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	tvis.item.mask = TVIF_CHILDREN | TVIF_TEXT | TVIF_PARAM;
	// tvis.item.lParam = (LPARAM)Item;
	tvis.item.pszText = (LPTSTR)(LPCTSTR)item;
	//	tvis.item.iImage = _imageList.GetIcon( folderPath., false );
	//	tvis.item.iSelectedImage = _imageList.GetIcon( folderPath, true );
	tvis.item.cChildren = hasChild;	

	treeItem = InsertItem(&tvis);

	return treeItem;
}

bool CTableTree::DeleteTreeItem( HTREEITEM item )
{
	return DeleteItem( item ) ? true : false;
}

bool CTableTree::InitPopupMenu( void )
{
	_popupMenu.CreatePopupMenu();
	_popupMenu.AppendMenu( MF_POPUP, ID_FOLDERTREE_POPUPMENU_UPDATE, "Alter" );
	_popupMenu.AppendMenu( MF_POPUP, ID_FOLDERTREE_POPUPMENU_CREATE, "Create" );
	_popupMenu.AppendMenu( MF_POPUP, ID_FOLDERTREE_POPUPMENU_DELETE, "Drop" );

	return true;
}

bool CTableTree::UninitPopupMenu( void )
{
	return _popupMenu.DestroyMenu() > 0 ? true : false;
}
