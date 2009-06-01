#include "stdafx.h"
#include "DBDictionary.h"

DBDictionary::DBDictionary()
{
}

DBDictionary::~DBDictionary()
{
}

void DBDictionary::Init()
{
	// 사전 초기화
	_dic.clear();

	// 사전 생성 옵션 로드
	XmlDocument xmlDoc;
	std::string path = "./resource/DBDicConfig.xml";
	xmlDoc.LoadFile( path.c_str() );

	const XmlNode * resNode = xmlDoc.GetNode( "resource" );
	std::string create = resNode->GetAttribute( "create" );

	if( create == "true" )
	{
		// 사전 생성
		CreateDBDic( resNode );

		// 사전 저장
		XmlDocument xmlDoc;
		XmlNode * resNode = xmlDoc.AddNode( "resource" );
		SaveToXML( resNode );

		std::string path = "./resource/DBDic.xml";
		xmlDoc.SaveFile( path.c_str() );
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
}

void DBDictionary::Uninit()
{
	// 사전 초기화
	_dic.clear();
}

bool DBDictionary::CreateDBDic( const XmlNode * resNode )
{
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

bool DBDictionary::LoadFromXML( const XmlNode * resNode )
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

bool DBDictionary::SaveToXML( XmlNode * resNode )
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

int DBDictionary::GetTagName( const std::vector<std::string> & inDataList, std::string & outTagName, std::string & outWords )
{
	// 단어인 경우
	int size = _dic.size();
	for( int num = 0; num < size; num++ )
	{
		const TableDataForDic & tableDataList = _dic[num];

		int colSize = tableDataList._dataList.size();
		for( int cnt = 0; cnt < colSize; cnt++ )
		{
			const ColDataForDic & colDataList = tableDataList._dataList[ cnt ];
			std::string colName = colDataList._colName;

			int dataSize = colDataList._dataList.size();
			for( int k = 0; k < dataSize; k++ )
			{
				const std::string & data = colDataList._dataList[ k ];
				std::string word = ToLowerCase( (char*)data.c_str() );

				int count = IsSameWords( inDataList, word, outWords );
				if( count >= 0 )
				{
					outTagName = colName;
					return count;
				}
			}
		}
	}
	return 0;
}

int DBDictionary::IsSameWords( const std::vector<std::string> & lWordList, std::string rWord, std::string & outWords )
{
	// 연속된 단어 비교
	std::string words;

	int count = lWordList.size();
	for( int num = 0; num < count; num++ )
	{
		std::string word = lWordList[ num ];
		if( words.length() > 0 )
		{
			char prefix = words[words.length()-1];
			if( word == "-" || prefix == '-')
			{
				words += word;
			}
			else
			{
				words += " ";
				words += word;
			}
		}
		else
		{
			words = word;
		}
		
		if( words == rWord )
		{
			outWords = words;
			return num;
		}
	}

	std::string lWord = lWordList[ 0 ];

	//
	int length = lWord.size();
	if( length < 3 )
	{
		return -1;
	}

	// 복수 비교 (s)
	if( lWord[length-1] == 's' )
	{
		lWord.erase(length-1);
		
		if( lWord == rWord )
		{
			outWords = lWord;
			return 0;
		}
	}

	// 복수 비교 (es)
	if( lWord[length-1] == 's' && lWord[length-2] == 'e' )
	{
		lWord.erase(length-1);
		lWord.erase(length-2);
		
		if( lWord == rWord )
		{
			outWords = lWord;
			return 0;
		}
	}

	return -1;
}

std::string DBDictionary::GetTagName( const std::string & inData )
{
	return GetColName( inData );
}

std::string DBDictionary::GetColName( const std::string & inData )
{
	int size = _dic.size();
	for( int num = 0; num < size; num++ )
	{
		const TableDataForDic & tableDataList = _dic[num];

		int colSize = tableDataList._dataList.size();
		for( int cnt = 0; cnt < colSize; cnt++ )
		{
			const ColDataForDic & colDataList = tableDataList._dataList[ cnt ];
			std::string colName = colDataList._colName;

			int dataSize = colDataList._dataList.size();
			for( int k = 0; k < dataSize; k++ )
			{
				const std::string & data = colDataList._dataList[ k ];
				std::string word = ToLowerCase( (char*)data.c_str() );

				if( inData == word ) 
				{
					return colName;
				}
			}
		}
	}

	std::string ret;
	return ret;
}

std::string DBDictionary::GetTableName( const std::string & inData )
{
	int size = _dic.size();
	for( int num = 0; num < size; num++ )
	{
		const TableDataForDic & tableDataList = _dic[num];
		std::string tableName = tableDataList._tableName;

		int colSize = tableDataList._dataList.size();
		for( int cnt = 0; cnt < colSize; cnt++ )
		{
			const ColDataForDic & colDataList = tableDataList._dataList[ cnt ];

			int dataSize = colDataList._dataList.size();
			for( int k = 0; k < dataSize; k++ )
			{
				const std::string & data = colDataList._dataList[ k ];
				std::string word = ToLowerCase( (char*)data.c_str() );

				if( inData == word ) 
				{
					return tableName;
				}
			}
		}
	}

	std::string ret;
	return ret;
}

int DBDictionary::GetColNum( std::string name, const ColList & colList )
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

void DBDictionary::GetColData( int colNum, const DataList & dataList, std::vector<std::string> & outList )
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

bool DBDictionary::IsSameData( std::string data, const std::vector<std::string> & inList  )
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