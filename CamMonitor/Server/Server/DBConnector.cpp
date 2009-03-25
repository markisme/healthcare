#include "DBConnector.h"

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "ws2_32.lib")

#define MYSQL_HOST  "211.239.163.46"
#define MYSQL_USER  "naid"
#define MYSQL_PWD   "rbgur0834#!"
#define MYSQL_DB "health"

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

int DBConnector::GetUserNo( std::string id )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;
	std::string userNo;

	char query[128];

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, MYSQL_HOST,MYSQL_USER,MYSQL_PWD,MYSQL_DB,3306,0,0))
	{
		sprintf(query,"SELECT * FROM login where id='%s';", id.c_str());
		mysql_query(&_mysql, (const char *)query);

		sql_result=mysql_store_result(&_mysql);
		if( sql_result )
		{
			row=mysql_fetch_row(sql_result);
			if( row == NULL )
			{
				return -1;
			}

			userNo = (char*)row[0];
		}

		mysql_close(&_mysql);
	}

	return atoi( userNo.c_str() );
}

bool DBConnector::ConfirmLogin( std::string id, std::string pass )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;

	char query[128];

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, MYSQL_HOST,MYSQL_USER,MYSQL_PWD,MYSQL_DB,3306,0,0))
	{
		sprintf(query,"SELECT * FROM login where id='%s';", id.c_str());
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

bool DBConnector::ConfirmHost( std::string id )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;

	char query[128];
	std::string isHost;

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, MYSQL_HOST,MYSQL_USER,MYSQL_PWD,MYSQL_DB,3306,0,0))
	{
		sprintf(query,"SELECT * FROM login where id='%s';", id.c_str());
		mysql_query(&_mysql, (const char *)query);

		sql_result=mysql_store_result(&_mysql);
		if( sql_result )
		{
			row=mysql_fetch_row(sql_result);
			if( row == NULL )
			{
				return FALSE;
			}

			isHost = (char*)row[3];
		}

		mysql_close(&_mysql);
	}

	int ret = atoi( isHost.c_str() );
	return ret;
}

void DBConnector::GetUserInfo( UserList & userInfoList )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;

	char query[128];
	
	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, MYSQL_HOST,MYSQL_USER,MYSQL_PWD,MYSQL_DB,3306,0,0))
	{
		sprintf(query,"SELECT * FROM userinfo;");
		mysql_query(&_mysql, (const char *)query);

		sql_result=mysql_store_result(&_mysql);
		int count = mysql_num_rows(sql_result);
		for( int num = 1; num <= count; num++ )
		{
			sprintf(query,"SELECT * FROM userinfo where userno=%d;", num);
			mysql_query(&_mysql, (const char *)query);

			sql_result=mysql_store_result(&_mysql);
			row=mysql_fetch_row(sql_result);

			UserInfo userInfo;
			userInfo._userNo = (char*)row[0];
			userInfo._userName = (char*)row[1];
			userInfo._age = (char*)row[2];
			userInfo._sex = (char*)row[3];
			userInfo._tall = (char*)row[4];
			userInfo._weight = (char*)row[5];
			userInfo._blood = (char*)row[6];
			userInfo._tel = (char*)row[7];
			userInfo._pic = (char*)row[8];

			userInfoList.push_back( userInfo );
		}
		
		mysql_close(&_mysql);
	}
}

void DBConnector::GetUserData( int userNo, UserDataList & userDataList )
{
	MYSQL_ROW row;
	MYSQL_RES *sql_result;

	char query[128];

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, MYSQL_HOST,MYSQL_USER,MYSQL_PWD,MYSQL_DB,3306,0,0))
	{
		sprintf(query,"SELECT * FROM data where userno=%d;", userNo);
		mysql_query(&_mysql, (const char *)query);

		sql_result=mysql_store_result(&_mysql);
		int count = mysql_num_rows(sql_result);
		for( int num = 1; num <= count; num++ )
		{
			row=mysql_fetch_row(sql_result);

			UserData userData;
			std::string datetime = (char*)row[1];
			userData._value = (char*)row[2];
			userData._temp = (char*)row[3];

			std::string time = datetime;
			userData._year = time.substr(0, 4);
			userData._month = time.substr(5, 2);
			userData._day = time.substr(8, 2);
			userData._hour = time.substr(11, 2);
			userData._min = time.substr(14, 2);			

			userDataList.push_back( userData );
		}

		mysql_close(&_mysql);
	}
}

void DBConnector::AddUserData( int userNo, UserData & userData )
{
	MYSQL_RES *sql_result;
	char query[128];

	mysql_init(&_mysql);
	if (mysql_real_connect(&_mysql, MYSQL_HOST,MYSQL_USER,MYSQL_PWD,MYSQL_DB,3306,0,0))
	{
		std::string dateTime = userData._year + "-" + userData._month + "-" + userData._day + " " + userData._hour + ":" + userData._min + ":" + "00";
		sprintf(query,"INSERT INTO data values( '%d', '%s', '%d', '%f' );", userNo, dateTime.c_str(), atoi(userData._value.c_str()), atof(userData._temp.c_str()) );
		mysql_query(&_mysql, (const char *)query);
		sql_result=mysql_store_result(&_mysql);
		mysql_close(&_mysql);
	}
}