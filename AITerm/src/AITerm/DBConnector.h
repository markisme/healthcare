#pragma once

#include <winsock2.h> 
#include <mysql.h>
#include <string>

struct DBConnectInfo
{
	std::string ip;
	std::string user;
	std::string pass;
	std::string dbName;
	int port;
};

struct ColData
{
	std::string _name;
	std::vector<std::string> _typeList;
};

struct RowData
{
	int _rowNum;
	std::vector<std::string> _data;
};

typedef std::vector<ColData> ColList;
typedef std::vector<RowData> DataList;

class DBConnector
{
public:
	DBConnector();
	~DBConnector();

	void Init( DBConnectInfo info );
	void Uninit();

	bool GetTableList( std::vector<std::string> & tableList );
	bool GetColumnsList( std::string tableName, ColList & colList );
	bool GetDataList( std::string tableName, DataList & dataList );
	bool MysqlQuery( std::string query );
	bool MysqlQuery( std::string query, DataList & dataList );

	static DBConnector & GetInstance() { return _instance; }

private:
	static DBConnector _instance;

	MYSQL _mysql;
	DBConnectInfo _info;
};