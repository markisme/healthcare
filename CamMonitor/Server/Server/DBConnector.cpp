#include "DBConnector.h"

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "ws2_32.lib")

#define MYSQL_HOST  "211.189.20.246"
#define MYSQL_USER  "ssm"
#define MYSQL_PWD   "gkswjdrla!"
#define MYSQL_DB "webcam"

//#define MYSQL_HOST  "211.189.19.160"
//#define MYSQL_USER  "root"
//#define MYSQL_PWD   "1234"
//#define MYSQL_DB "health"

DBConnector DBConnector::_instance;

DBConnector::DBConnector()
{
}

DBConnector::~DBConnector()
{
}

void DBConnector::Init()
{
}

void DBConnector::Uninit()
{
}

bool DBConnector::ConfirmLogin( std::string id, std::string pass )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;

	char query[128];

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, MYSQL_HOST,MYSQL_USER,MYSQL_PWD,MYSQL_DB,3306,0,0))
	{
		sprintf(query,"SELECT * FROM usertbl where userid='%s';", id.c_str());
		mysql_query(&_mysql, (const char *)query);

		sql_result=mysql_store_result(&_mysql);
		if( sql_result )
		{
			row=mysql_fetch_row(sql_result);
			if( row == NULL )
			{
				return FALSE;
			}

			std::string passStr = (char*)row[2];

			if( passStr == pass )
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		mysql_close(&_mysql);
	}

	return FALSE;
}