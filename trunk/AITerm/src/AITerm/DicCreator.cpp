#include "stdafx.h"
#include "DicCreator.h"

DicCreator::DicCreator()
{
}

DicCreator::~DicCreator()
{
}

void DicCreator::Init()
{
	XmlDocument xmlDoc;
	std::string path = "./resource/CreateTable.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode * resNode = xmlDoc.GetNode( "resource" );

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

		int nodeCount = tableNode->GetNodeCount( "col" );
		for( int cnt = 0; cnt < nodeCount; cnt++ )
		{
			const XmlNode * colNode = tableNode->GetNode( "col", cnt );
			std::string colName = colNode->GetAttribute( "name" );

			int colNum = GetColNum( colName, colList );

			std::vector<std::string> dicList;
			GetColData( colNum, dataList, dicList );
		}
	}
}

void DicCreator::Uninit()
{
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
		outList.push_back( data );
		printf( "%s \n", data.c_str() );
	}
}