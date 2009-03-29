#include "stdafx.h"
#include "DataManagerDlg.h"

IMPLEMENT_DYNAMIC(DataManagerDlg, CDialog)
DataManagerDlg::DataManagerDlg(CWnd* pParent /*=NULL*/)	: CDialog(DataManagerDlg::IDD, pParent)
{
}

DataManagerDlg::~DataManagerDlg()
{
}

void DataManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(DataManagerDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &DataManagerDlg::OnBnClickedOk)
END_MESSAGE_MAP() 


INT_PTR DataManagerDlg::DoModal( std::string tableName, int selectColNo )
{
	_tableName = tableName;
	_selectColNo = selectColNo;
	return CDialog::DoModal();
}

BOOL DataManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//
	int count = _dlgList.size();
	for( int num = 0; num < count; num++ )
	{
		DataListItem * item = _dlgList[num];
		item->ShowWindow( SW_SHOW );
		item->UpdateWindow();

		item->ModifyStyleEx( 0, WS_EX_CONTROLPARENT );
		item->ModifyStyle( 0, WS_TABSTOP );
	}

	// ������ ���� �����ϱ�
	int cnt = _dlgList.size();
	for( int num = 0; num < cnt; num++ )
	{
		DataListItem * item = _dlgList[num];
		item->SetColName( _colList[num]._name.c_str() );
		item->SetFiledType( _colList[num]._typeList[0] );
		item->SetFiledNull( _colList[num]._typeList[1] );
		item->SetFiledPK( _colList[num]._typeList[2] );
		if( _selectColNo >= 0 )
		{
			RowData data = _dataList[ _selectColNo ];
			item->SetFiledText( data._data[num].c_str() );
		}
	}

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

int DataManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ���� ������ �÷����� ���
	DBConnector::GetInstance().GetColumnsList( _tableName, _colList );

	// ��� Ŀ���� �����ؼ� ������ ��������
	DBConnector::GetInstance().GetDataList( _tableName, _dataList );

	int count = _colList.size();
	for( int num = 0; num < count; num++ )
	{
		// ������ ����� ���̱�
		DataListItem * item = new DataListItem;
		item->Create(IDD_DATA_LIST_DLG,this);
		_dlgList.push_back( item );
	}
}

void DataManagerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	int count = _dlgList.size();
	for( int num = 0; num < count; num++ )
	{
		DataListItem * item = _dlgList[num];
		item->DestroyWindow();
		SAFE_DELETE(item);
	}
}

void DataManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// �̵�
	int count = _dlgList.size();
	for( int num = 0; num < count; num++ )
	{
		DataListItem * item = _dlgList[num];

		CRect fieldDlgRect;
		item->GetClientRect( fieldDlgRect );

		uint fieldDlgWidth = (uint)fieldDlgRect.Width();
		uint fieldDlgHeight = (uint)fieldDlgRect.Height();
		item->MoveWindow( 20, 20+fieldDlgHeight*num, fieldDlgWidth, fieldDlgHeight );
	}
}

void DataManagerDlg::OnBnClickedOk()
{
	// ���� ������
	if( _selectColNo < 0 )
	{
		//
		std::string colNameList;
		std::string valueList;

		// ������ ���� �ܾ� ����
		int cnt = _dlgList.size();
		for( int num = 0; num < cnt; num++ )
		{
			DataListItem * item = _dlgList[num];
			std::string name = item->GetColName();
			std::string text = item->GetFieldText();
			colNameList += name + ",";
			valueList += "\"" + text + "\"" + ",";
		}

		colNameList.erase( colNameList.size() - 1 );
		valueList.erase( valueList.size() - 1 );

		char query[1024];
		sprintf(query,"INSERT INTO %s(%s) VALUES(%s);", _tableName.c_str(), colNameList.c_str(), valueList.c_str() );
		if( DBConnector::GetInstance().MysqlQuery( query ) == FALSE )
		{
			return;
		}
	}
	else
	{
		//
		std::string valueList;
		std::string whereList;

		int cnt = _dlgList.size();
		for( int num = 0; num < cnt; num++ )
		{
			// ������ ���� �ܾ� ����
			DataListItem * item = _dlgList[num];
			std::string name = item->GetColName();
			std::string text = item->GetFieldText();
			valueList += name + "=" + "\"" + text + "\"" + ",";

			// ���� ���� ���� ����
			RowData data = _dataList[ _selectColNo ];
			std::string preText = data._data[num];
			if( preText.length() != 0 )
			{
				whereList += name + "=" + "\"" + preText + "\"" + " AND ";
			}
		}

		valueList.erase( valueList.size() - 1 );
		whereList.erase( whereList.size() - 5 );

		//
		char query[1024];
		sprintf(query,"UPDATE %s SET %s WHERE %s;", _tableName.c_str(), valueList.c_str(), whereList.c_str() );
		if( DBConnector::GetInstance().MysqlQuery( query ) == FALSE )
		{
			return;
		}
	}

	OnOK();
}
