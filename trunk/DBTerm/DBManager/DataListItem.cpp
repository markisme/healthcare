#include "stdafx.h"
#include "DataListItem.h"

IMPLEMENT_DYNAMIC(DataListItem, CDialog)
DataListItem::DataListItem(CWnd* pParent /*=NULL*/)	: CDialog(DataListItem::IDD, pParent)
{
}

DataListItem::~DataListItem()
{
}

void DataListItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DATA_COLNAME, _colName);
	DDX_Control(pDX, IDC_EDIT_DATA_VALUE, _dataValue);
	DDX_Control(pDX, IDC_STATIC_DATA_TYPE, _dataType);
	DDX_Control(pDX, IDC_STATIC_DATA_NULL, _dataNull);
	DDX_Control(pDX, IDC_STATIC_DATA_PK, _dataPK);		
}

BEGIN_MESSAGE_MAP(DataListItem, CDialog)
END_MESSAGE_MAP() 


BOOL DataListItem::OnInitDialog()
{
	CDialog::OnInitDialog();
	_colName.ShowWindow( SW_SHOW );
	_dataValue.ShowWindow( SW_SHOW );
	_dataType.ShowWindow( SW_SHOW );
	_dataNull.ShowWindow( SW_SHOW );
	_dataPK.ShowWindow( SW_SHOW );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void DataListItem::SetColName( std::string colName )
{
	_colName.SetWindowText( colName.c_str() );
}

std::string DataListItem::GetColName()
{
	CString text;
	_colName.GetWindowText(text);

	return text;
}

void DataListItem::SetFiledText( std::string text )
{
	_dataValue.SetWindowText( text.c_str() );
}

std::string DataListItem::GetFieldText()
{
	CString text;
	_dataValue.GetWindowText(text);

	return text;
}