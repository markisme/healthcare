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

class DicCreator
{
public:
	DicCreator();
	~DicCreator();

	void Init();
	void Uninit();

	bool LoadFromXML( const XmlNode * resNode );
	bool SaveToXML( XmlNode * resNode );

private:
	int GetColNum( std::string name, const ColList & colList );
	void GetColData( int colNum, const DataList & dataList, std::vector<std::string> & outList );
	bool IsSameData( std::string data, const std::vector<std::string> & inList  );

private:
	Dic _dic;
};