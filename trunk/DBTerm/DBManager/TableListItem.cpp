#include "stdafx.h"
#include "TableListItem.h"
#include "TableManagerDlg.h"

IMPLEMENT_DYNAMIC(TableListItem, CDialog)
TableListItem::TableListItem(CWnd* pParent /*=NULL*/)	: CDialog(TableListItem::IDD, pParent)
{
}

TableListItem::~TableListItem()
{
}

void TableListItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILED_NAME, _fieldName);
	DDX_Control(pDX, IDC_COMBO_FIELD_TYPE, _fieldType);
	DDX_Control(pDX, IDC_EDIT_FILED_LENGTH, _fieldLength);
	DDX_Control(pDX, IDC_CHECK_NOT_NULL, _notNull);
	DDX_Control(pDX, IDC_CHECK_PK, _primaryKey);
	DDX_Control(pDX, IDC_CHECK_AUTO_INCR, _autoIncr);
}

BEGIN_MESSAGE_MAP(TableListItem, CDialog)
	ON_EN_CHANGE(IDC_EDIT_FILED_NAME, &TableListItem::OnEnChangeEditFiledName)
	ON_BN_CLICKED(IDC_CHECK_AUTO_INCR, &TableListItem::OnBnClickedCheckAutoIncr)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD_TYPE, &TableListItem::OnCbnSelchangeComboFieldType)
	ON_BN_CLICKED(IDC_CHECK_PK, &TableListItem::OnBnClickedCheckPk)
	ON_BN_CLICKED(IDC_CHECK_NOT_NULL, &TableListItem::OnBnClickedCheckNotNull)
END_MESSAGE_MAP() 


BOOL TableListItem::OnInitDialog()
{
	CDialog::OnInitDialog();

	//
	_fieldName.ShowWindow( SW_SHOW );
	_fieldType.ShowWindow( SW_SHOW );
	_fieldLength.ShowWindow( SW_SHOW );
	_notNull.ShowWindow( SW_SHOW );
	_primaryKey.ShowWindow( SW_SHOW );
	_autoIncr.ShowWindow( SW_SHOW );

	//
	_fieldType.AddString( "bool" );
	_fieldType.AddString( "char" );
	_fieldType.AddString( "datetime" );
	_fieldType.AddString( "double" );
	_fieldType.AddString( "float" );
	_fieldType.AddString( "int" );
	_fieldType.AddString( "tinyint" );
	_fieldType.AddString( "tinychar" );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void TableListItem::Init( CDialog * parent )
{
	_parent = parent;
}

void TableListItem::Uninit()
{
	_parent = NULL;
}

void TableListItem::Refresh()
{
	// 길이가 고정된 필드
	std::string type = GetFieldType();
	if( type == "bool" || type == "datetime" || type == "double" )
	{
		_fieldLength.SetWindowText( "" );
		_fieldLength.EnableWindow( FALSE );
	}
	else
	{
		_fieldLength.EnableWindow( TRUE );
	}

	// 오토 증가 금지
	if( !( type == "bool" || type == "int" || type == "float" || type == "double" ) )
	{
		_autoIncr.SetCheck( FALSE );
	}
}

std::string TableListItem::GetFieldName()
{
	CString str;
	_fieldName.GetWindowText( str );

	return str;
}

void TableListItem::SetFieldName( std::string name )
{
	_fieldName.SetWindowText( name.c_str() );
}

std::string TableListItem::GetFieldType()
{
	int index = _fieldType.GetCurSel();
	if( index < 0 )
	{
		return "";
	}
	
	CString str;
	_fieldType.GetLBText( index, str );

	return str;
}

void TableListItem::SetFieldType( std::string type )
{
	int count = _fieldType.GetCount();
	for( int num = 0; num < count; num++ )
	{
		CString str;
		_fieldType.GetLBText( num, str );

		std::string curType = str;
		if( curType == type )
		{
			_fieldType.SetCurSel( num );
			break;
		}
	}
}

std::string TableListItem::GetFieldLenth()
{
	CString str;
	_fieldLength.GetWindowText( str );

	return str;
}

void TableListItem::SetFieldLenth( std::string length )
{
	_fieldLength.SetWindowText( length.c_str() );
}

bool TableListItem::IsNotNull()
{
	return _notNull.GetCheck();
}

void TableListItem::SetNotNull( bool value )
{
	_notNull.SetCheck( value );
}

bool TableListItem::IsPK()
{
	return _primaryKey.GetCheck();
}

void TableListItem::SetPK( bool value )
{
	_primaryKey.SetCheck( value );
}

bool TableListItem::IsAutoIncr()
{
	return _autoIncr.GetCheck();
}

void TableListItem::SetAutoIncr( bool value )
{
	_autoIncr.SetCheck( value );
}

void TableListItem::OnEnChangeEditFiledName()
{
	((TableManagerDlg*)_parent)->AddField( this );
	_fieldName.SetFocus();
}

void TableListItem::OnBnClickedCheckPk()
{
	if( _primaryKey.GetCheck() == TRUE )
	{
		_notNull.SetCheck( TRUE );
	}
	else
	{
		_autoIncr.SetCheck( FALSE );
	}
}

void TableListItem::OnBnClickedCheckNotNull()
{
	if( _primaryKey.GetCheck() == TRUE )
	{
		_notNull.SetCheck( TRUE );
	}
}

void TableListItem::OnBnClickedCheckAutoIncr()
{
	// 오토 증가 금지
	std::string type = GetFieldType();
	if( !( type == "bool" || type == "int" || type == "float" || type == "double" ) )
	{
		_autoIncr.SetCheck( FALSE );
	}
	else if( _autoIncr.GetCheck() == TRUE )
	{
		_primaryKey.SetCheck( TRUE );
		_notNull.SetCheck( TRUE );
	}
}

void TableListItem::OnCbnSelchangeComboFieldType()
{
	Refresh();
}