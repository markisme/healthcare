#pragma once

struct ColDataForDic
{
	std::string _colName;
	std::vector<std::string> _dataList;
};

struct TableDataForDic
{
	std::string _tableName;
	std::vector<ColDataForDic> _dataList;
};

typedef std::vector<TableDataForDic> Dic;

class DBDictionary
{
public:
	DBDictionary();
	~DBDictionary();

	void Init( bool createDic = false );
	void Uninit();

	bool LoadFromXML( const XmlNode * resNode );
	bool SaveToXML( XmlNode * resNode );

	std::string GetTagName( const std::string & inData );

	std::string GetColName( const std::string & inData );
	std::string GetTableName( const std::string & inData );

private:
	bool CreateDBDic();
	int GetColNum( std::string name, const ColList & colList );
	void GetColData( int colNum, const DataList & dataList, std::vector<std::string> & outList );
	bool IsSameData( std::string data, const std::vector<std::string> & inList  );

private:
	Dic _dic;
};