#pragma once

#define UM_FOLDER_CHANGED	WM_USER + 1

class CTableTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTableTree)

public:
	CTableTree();
	virtual ~CTableTree();

public:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnDestroy();
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUpdate();
	afx_msg void OnCreateTable();
	afx_msg void OnDelete();

public:
	bool Refresh( std::string dbName, bool expand = true );
	void Clear( void );

private:
	HTREEITEM InsertTreeItem( HTREEITEM parent,std::string name, bool hasChild );
	bool DeleteTreeItem( HTREEITEM item );	

public:
	bool InitPopupMenu( void );
	bool UninitPopupMenu( void );

private:
	CMenu _popupMenu;
};