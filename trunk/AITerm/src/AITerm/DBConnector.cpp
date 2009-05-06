#include "stdafx.h"
#include "DBConnector.h"

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "ws2_32.lib")

DBConnector DBConnector::_instance;

DBConnector::DBConnector()
{
}

DBConnector::~DBConnector()
{
}

void DBConnector::Init( DBConnectInfo info )
{
	_info = info;
}

void DBConnector::Uninit()
{
}

bool DBConnector::GetTableList( std::vector<std::string> & tableList )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;

	char query[128];

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, _info.ip.c_str(),_info.user.c_str(),_info.pass.c_str(),_info.dbName.c_str(),_info.port,0,0))
	{
		sprintf(query,"show tables;");
		mysql_query(&_mysql, (const char *)query);

		sql_result=mysql_store_result(&_mysql);
		if( sql_result )
		{
			row=mysql_fetch_row(sql_result);
			while( row != NULL )
			{
				char * str = (char*)row[0];
				tableList.push_back( str );

				row=mysql_fetch_row(sql_result);
			}
		}
		else
		{
			return FALSE;
		}

		mysql_close(&_mysql);
	}

	return TRUE;
}

bool DBConnector::GetColumnsList( std::string tableName, ColList & colList )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;

	char query[128];

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, _info.ip.c_str(),_info.user.c_str(),_info.pass.c_str(),_info.dbName.c_str(),_info.port,0,0))
	{
		sprintf(query,"SHOW COLUMNS FROM %s;", tableName.c_str());
		mysql_query(&_mysql, (const char *)query);

		sql_result=mysql_store_result(&_mysql);
		if( sql_result )
		{
			row=mysql_fetch_row(sql_result);
			while( row != NULL )
			{
				// col 구조체 만들기
				ColData coldata;
				coldata._name = (char*)row[0];

				// col에 데이터 채우기
				int count = mysql_num_fields(sql_result); // row 크기
				for( int num = 1; num < count; num++ )
				{
					char * str = (char*)row[num];
					if( str != NULL )
					{
						coldata._typeList.push_back(str);
					}
					else
					{
						coldata._typeList.push_back("");
					}
				}

				// 리스트에 row 데이터 추가
				colList.push_back(coldata);

				// 다음 row 패치
				row=mysql_fetch_row(sql_result);
			}
		}
		else
		{
			return FALSE;
		}

		mysql_close(&_mysql);
	}

	return TRUE;
}

bool DBConnector::GetDataList( std::string tableName, DataList & dataList )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;

	char query[128];
	int rowCount = 0;

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, _info.ip.c_str(),_info.user.c_str(),_info.pass.c_str(),_info.dbName.c_str(),_info.port,0,0))
	{
		sprintf(query,"SELECT * FROM %s;", tableName.c_str());
		mysql_query(&_mysql, (const char *)query);

		sql_result=mysql_store_result(&_mysql);
		if( sql_result )
		{
			row=mysql_fetch_row(sql_result);
			while( row != NULL )
			{
				// row 구조체 만들기
				RowData rowdata;
				rowdata._rowNum = rowCount;

				// row에 데이터 채우기
				int count = mysql_num_fields(sql_result); // row 크기
				for( int num = 0; num < count; num++ )
				{
					char * str = (char*)row[num];
					if( str != NULL )
					{
						rowdata._data.push_back(str);
					}
					else
					{
						rowdata._data.push_back("");
					}
				}

				// 리스트에 row 데이터 추가
				dataList.push_back(rowdata);
				rowCount++;

				// 다음 row 패치
				row=mysql_fetch_row(sql_result);
			}
		}
		else
		{
			return FALSE;
		}

		mysql_close(&_mysql);
	}

	return TRUE;
}

bool DBConnector::MysqlQuery( std::string query )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, _info.ip.c_str(),_info.user.c_str(),_info.pass.c_str(),_info.dbName.c_str(),_info.port,0,0))
	{
		int query_stat = mysql_query(&_mysql, query.c_str());
		if (query_stat != 0)
		{
			std::string msg = mysql_error(&_mysql);
			if( msg.length() != 0 )
			{
				printf( msg.c_str() );
				mysql_close(&_mysql);
				return FALSE;
			}
		}

		sql_result=mysql_store_result(&_mysql);
		if( sql_result )
		{
			row=mysql_fetch_row(sql_result);
			while( row != NULL )
			{
				row=mysql_fetch_row(sql_result);
			}
		}
		else
		{
			std::string msg = mysql_error(&_mysql);
			if( msg.length() != 0 )
			{
				printf( msg.c_str() );
				mysql_close(&_mysql);
				return FALSE;
			}
		}

		mysql_close(&_mysql);
	}

	return TRUE;
}

bool DBConnector::MysqlQuery( std::string query, DataList & dataList )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;

	int rowCount = 0;

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, _info.ip.c_str(),_info.user.c_str(),_info.pass.c_str(),_info.dbName.c_str(),_info.port,0,0))
	{
		mysql_query(&_mysql, query.c_str());

		sql_result=mysql_store_result(&_mysql);
		if( sql_result )
		{
			row=mysql_fetch_row(sql_result);
			while( row != NULL )
			{
				// row 구조체 만들기
				RowData rowdata;
				rowdata._rowNum = rowCount;

				// row에 데이터 채우기
				int count = mysql_num_fields(sql_result); // row 크기
				for( int num = 0; num < count; num++ )
				{
					char * str = (char*)row[num];
					if( str != NULL )
					{
						rowdata._data.push_back(str);
					}
					else
					{
						rowdata._data.push_back("");
					}
				}

				// 리스트에 row 데이터 추가
				dataList.push_back(rowdata);
				rowCount++;

				// 다음 row 패치
				row=mysql_fetch_row(sql_result);
			}
		}
		else
		{
			return FALSE;
		}

		mysql_close(&_mysql);
	}

	return TRUE;
}
