#include "stdafx.h"
#include "TableManagerDlg.h"

IMPLEMENT_DYNAMIC(TableManagerDlg, CDialog)
TableManagerDlg::TableManagerDlg(CWnd* pParent /*=NULL*/)	: CDialog(TableManagerDlg::IDD, pParent)
{
}

TableManagerDlg::~TableManagerDlg()
{
}

void TableManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TABLE_NAME, _tableName);
}

BEGIN_MESSAGE_MAP(TableManagerDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &TableManagerDlg::OnBnClickedOk)
END_MESSAGE_MAP() 


INT_PTR TableManagerDlg::DoModal( int command, std::string tableName )
{
	_command = command;
	_tbName = tableName;
	return CDialog::DoModal();		
}

BOOL TableManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 윈도우 내용 갱신하기
	int cnt = _colList.size();
	for( int num = 0; num < cnt; num++ )
	{
		std::string type, length;
		BOOL notNull= FALSE, primaryKey= FALSE, autoIncr = FALSE;
		std::string buf = _colList[num]._typeList[0];
		int index = buf.find('(');
		if( index > 0 )
		{
			type = buf.substr( 0, index );
			length = buf.substr( index + 1, buf.size() - index - 2);
		}
		else
		{
			type = buf;
		}

		int count = _colList[num]._typeList.size();
		if( count >= 2 )
		{
			std::string str = _colList[num]._typeList[1];
			if( str == "NO" )
			{
				notNull = TRUE;
			}
		}
		if( count >= 3 )
		{
			std::string str = _colList[num]._typeList[2];
			if( str == "PRI" )
			{
				primaryKey = TRUE;
			}
		}
		if( count >= 5 )
		{
			std::string str = _colList[num]._typeList[4];
			if( str == "auto_increment" )
			{
				autoIncr = TRUE;
			}
		}

		TableListItem * item = _dlgList[num];
		item->SetFieldName( _colList[num]._name.c_str() );
		item->SetFieldType( type );
		item->SetFieldLenth( length );
		item->SetNotNull( notNull );
		item->SetPK( primaryKey );
		item->SetAutoIncr( autoIncr );
		item->Refresh();
	}

	if( _command == 1 )
	{
		_tableName.SetWindowText( _tbName.c_str() );
	}

	Refresh();
	
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

int TableManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if( _command == 0 ) // 생성
	{
		// 생성창을 위해 만들어 둠
		TableListItem * item = new TableListItem();
		item->Create(IDD_TABLE_LIST_DLG,this);
		item->Init( this );
		_dlgList.push_back(item);
	}
	else // 수정
	{
		// 쿼리 날려서 컬럼정보 얻기
		DBConnector::GetInstance().GetColumnsList( _tbName, _colList );

		int count = _colList.size();
		for( int num = 0; num < count; num++ )
		{
			TableListItem * item = new TableListItem();
			item->Create(IDD_TABLE_LIST_DLG,this);
			item->Init( this );
			_dlgList.push_back(item);
		}
	}
}

void TableManagerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	int count = _dlgList.size();
	for( int num = 0; num < count; num++ )
	{
		TableListItem * item = _dlgList[num];
		item->Uninit();
		item->DestroyWindow();
		SAFE_DELETE(item);
	}
}

void TableManagerDlg::AddField( CDialog * child )
{
	// 필드 20개 이상 생성 불가
	int count = _dlgList.size();
	if( count >= 20 )
	{
		return;
	}
	
	// 수정 아이템은 필드 추가 금지
	if( _command == 1 )
	{
		return;
	}

	// 마지막 아이템인지 검사
	TableListItem * item = _dlgList[count-1];
	if( item == child )
	{
		TableListItem * item = new TableListItem();
		item->Create(IDD_TABLE_LIST_DLG,this);
		item->Init( this );
		_dlgList.push_back(item);

		Refresh();
	}
}

void TableManagerDlg::Refresh()
{
	//
	int count = _dlgList.size();
	for( int num = 0; num < count; num++ )
	{
		TableListItem * item = _dlgList[num];
		item->ShowWindow( SW_SHOW );
		item->UpdateWindow();

		item->ModifyStyleEx( 0, WS_EX_CONTROLPARENT );
		item->ModifyStyle( 0, WS_TABSTOP );
	}

	// 이동
	count = _dlgList.size();
	for( int num = 0; num < count; num++ )
	{
		TableListItem * item = _dlgList[num];

		CRect fieldDlgRect;
		item->GetClientRect( fieldDlgRect );

		uint fieldDlgWidth = (uint)fieldDlgRect.Width();
		uint fieldDlgHeight = (uint)fieldDlgRect.Height();
		item->MoveWindow( 10, 40+fieldDlgHeight*num, fieldDlgWidth, fieldDlgHeight );
	}
}

void TableManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	Refresh();
}

void TableManagerDlg::OnBnClickedOk()
{
	// 쿼리 날리기
	if( _command == 0 )
	{
		// 테이블 이름 가져오기
		CString tableName;
		_tableName.GetWindowText( tableName );
		//
		std::string fieldList;

		// 수정된 정보 긁어 오기
		int cnt = _dlgList.size();
		for( int num = 0; num < cnt; num++ )
		{
			TableListItem * item = _dlgList[num];

			std::string name = item->GetFieldName();
			std::string type = item->GetFieldType();
			std::string length = item->GetFieldLenth();
			bool notNull = item->IsNotNull();
			bool primaryKey = item->IsPK();
			bool autoIncr = item->IsAutoIncr();

			if( name.length() != 0 )
			{
				fieldList += name + " ";
			}
			else
			{
				continue;
			}

			if( type.length() != 0 )
			{
				fieldList += type;
			}
			else
			{
				continue;
			}

			if( length.length() != 0 )
			{
				fieldList += "(" + length + ")";
				fieldList += " ";
			}
			else
			{
				fieldList += " ";
			}

			if( notNull == TRUE )
			{
				fieldList += "not null";
				fieldList += " ";
			}

			if( primaryKey == TRUE )
			{
				fieldList += "primary key";
				fieldList += " ";
			}

			if( autoIncr == TRUE )
			{
				fieldList += "auto_increment";
				fieldList += " ";
			}
			
			fieldList += ",";
		}

		fieldList.erase( fieldList.size() - 1 );

		char query[1024];
		sprintf(query,"CREATE TABLE %s(%s);", tableName, fieldList.c_str() );
		if( DBConnector::GetInstance().MysqlQuery( query ) == FALSE )
		{
			return;
		}
	}
	else
	{
		// 테이블 이름 가져오기
		CString tableName;
		_tableName.GetWindowText( tableName );

		std::string pkList;

		// 수정된 정보 긁어 오기
		int cnt = _dlgList.size();
		for( int num = 0; num < cnt; num++ )
		{
			TableListItem * item = _dlgList[num];

			std::string name = item->GetFieldName();
			std::string type = item->GetFieldType();
			std::string length = item->GetFieldLenth();
			bool notNull = item->IsNotNull();
			bool primaryKey = item->IsPK();
			bool autoIncr = item->IsAutoIncr();

			if( name.length() == 0 )
			{
				continue;
			}

			if( type.length() == 0 )
			{
				continue;
			}

			std::string typeOpt;
			if( length.length() != 0 )
			{
				typeOpt += type + "(" + length + ")";
				typeOpt += " ";
			}
			else
			{
				typeOpt += type + " ";
			}

			std::string nullOpt;
			if( notNull == TRUE )
			{
				nullOpt += "not null";
				nullOpt += " ";
			}

			if( primaryKey == TRUE )
			{
				pkList += name;
				pkList += ",";
			}

			std::string incrOpt;
			if( autoIncr == TRUE )
			{
				incrOpt += "auto_increment";
				incrOpt += " ";
			}

			char query[1024];
			sprintf(query,"ALTER TABLE %s MODIFY %s %s %s %s;", tableName, name.c_str(), typeOpt.c_str(), nullOpt.c_str(), incrOpt.c_str() );
			if( DBConnector::GetInstance().MysqlQuery( query ) == FALSE )
			{
				return;
			}
		}

		pkList.erase( pkList.size() - 1 );

		if( pkList.size() > 0 )
		{
			char query[1024];
			sprintf(query,"ALTER TABLE %s add primary key (%s);", tableName, pkList.c_str() );
			if( DBConnector::GetInstance().MysqlQuery( query ) )
			{
				return;
			}
		}
	}

	OnOK();
}
