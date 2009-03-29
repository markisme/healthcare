#include "stdafx.h"
#include "DicCreator.h"

DicCreator::DicCreator()
{
}

DicCreator::~DicCreator()
{
}

void DicCreator::Init( bool createDic )
{
	// 사전 초기화
	_dic.clear();

	if( createDic == true )
	{
		// 사전 생성
		CreateDBDic();
	}
	else
	{
		// 사전 로드
		XmlDocument xmlDoc;
		std::string path = "./resource/DBDic.xml";
		xmlDoc.LoadFile( path.c_str() );

		const XmlNode * resNode = xmlDoc.GetNode( "resource" );
		LoadFromXML( resNode );
	}

	// 사전 저장
	XmlDocument xmlDoc;
	XmlNode * resNode = xmlDoc.AddNode( "resource" );
	SaveToXML( resNode );

	std::string path = "./resource/DBDic.xml";
	xmlDoc.SaveFile( path.c_str() );
}

void DicCreator::Uninit()
{
	// 사전 초기화
	_dic.clear();
}

bool DicCreator::CreateDBDic()
{
	XmlDocument xmlDoc;
	std::string path = "./resource/DBDicConfig.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode * resNode = xmlDoc.GetNode( "resource" );
	LoadFromXML( resNode );

	int nodeCount = resNode->GetNodeCount( "table" );
	for( int num = 0; num < nodeCount; num++ )
	{
		const XmlNode * tableNode = resNode->GetNode( "table", num );
		std::string tableName = tableNode->GetAttribute( "name" );

		// 디비 커넥터 접속해서 컬럼 가져오기
		ColList colList;
		DBConnector::GetInstance().GetColumnsList( tableName.c_str(), colList );

		// 디비 커넥터 접속해서 데이터 가져오기
		DataList dataList;
		DBConnector::GetInstance().GetDataList( tableName.c_str(), dataList );

		TableDataForDic tableDataList;
		tableDataList._tableName = tableName;

		int nodeCount = tableNode->GetNodeCount( "col" );
		for( int cnt = 0; cnt < nodeCount; cnt++ )
		{
			const XmlNode * colNode = tableNode->GetNode( "col", cnt );
			std::string colName = colNode->GetAttribute( "name" );

			ColDataForDic colDataList;
			colDataList._colName = colName;

			int colNum = GetColNum( colName, colList );
			GetColData( colNum, dataList, colDataList._dataList );

			tableDataList._dataList.push_back( colDataList );
		}

		_dic.push_back( tableDataList );
	}

	return true;
}

bool DicCreator::LoadFromXML( const XmlNode * resNode )
{
	int tableCount = resNode->GetNodeCount( "table" );
	for( int num = 0; num < tableCount; num++ )
	{
		const XmlNode * tableNode = resNode->GetNode( "table", num );
		std::string tableName = tableNode->GetAttribute( "name" );
		
		TableDataForDic tableDataList;
		tableDataList._tableName = tableName;

		int colCount = tableNode->GetNodeCount( "col" );
		for( int cnt = 0; cnt < colCount; cnt++ )
		{
			const XmlNode * colNode = tableNode->GetNode( "col", cnt );
			std::string colName = colNode->GetAttribute( "name" );

			ColDataForDic colDataList;
			colDataList._colName = colName;

			int dataCount = colNode->GetNodeCount( "data" );
			for( int k = 0; k < dataCount; k++ )
			{
				const XmlNode * dataNode = colNode->GetNode( "data", k );
				std::string text = dataNode->GetAttribute( "text" );

				colDataList._dataList.push_back( text );
			}

			tableDataList._dataList.push_back( colDataList );
		}

		_dic.push_back( tableDataList );
	}

	return true;
}

bool DicCreator::SaveToXML( XmlNode * resNode )
{
	int size = _dic.size();
	for( int num = 0; num < size; num++ )
	{
		TableDataForDic tableDataList = _dic[num];
		std::string tableName = tableDataList._tableName;

		XmlNode * tableNode = resNode->AddNode( "table" );
		tableNode->SetAttribute( "name", tableName.c_str() );

		int colSize = tableDataList._dataList.size();
		for( int cnt = 0; cnt < colSize; cnt++ )
		{
			const ColDataForDic & colDataList = tableDataList._dataList[ cnt ];
			std::string colName = colDataList._colName;

			XmlNode * colNode = tableNode->AddNode( "col" );
			colNode->SetAttribute( "name", colName.c_str() );

			int dataSize = colDataList._dataList.size();
			for( int k = 0; k < dataSize; k++ )
			{
				const std::string & data = colDataList._dataList[ k ];

				XmlNode * dataNode = colNode->AddNode( "data" );
				dataNode->SetAttribute( "text", data.c_str() );
			}
		}
	}
	return true;
}

int DicCreator::GetColNum( std::string name, const ColList & colList )
{
	int size = colList.size();
	for( int num = 0; num < size; num++ )
	{
		if( name == colList[ num ]._name )
		{
			return num;
		}
	}

	return -1;
}

void DicCreator::GetColData( int colNum, const DataList & dataList, std::vector<std::string> & outList )
{
	if( colNum < 0 )
	{
		printf( "read error" );
		return;
	}

	int size = dataList.size();
	for( int num = 0; num < size; num++ )
	{
		std::string data = dataList[num]._data[colNum];
		if( IsSameData( data, outList ) == false  )
		{
			printf( "%s \n", data.c_str() );
			outList.push_back( data );
		}
	}
}

bool DicCreator::IsSameData( std::string data, const std::vector<std::string> & inList  )
{
	if( data.empty() )
	{
		return true;
	}

	int size = inList.size();
	for( int num = 0; num < size; num++ )
	{
		if( data == inList[ num ] )
		{
			return true;
		}
	}

	return false;
}