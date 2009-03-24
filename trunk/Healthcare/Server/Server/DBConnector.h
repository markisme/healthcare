#pragma once

#include <winsock2.h> 
#include <mysql.h>
#include <string>
#include "CommonType.h"

class DBConnector
{
public:
	DBConnector();
	~DBConnector();

	void Init();
	void Uninit();

	int GetUserNo( std::string id );
	bool ConfirmLogin( std::string id, std::string pass );
	bool ConfirmHost( std::string id );
	void GetUserInfo( UserList & userInfoList );
	void GetUserData( int userNo, UserDataList & userDataList );
	void AddUserData( int userNo, UserData & userData );

	static DBConnector & GetInstance() { return _instance; }

private:
	static DBConnector _instance;

	MYSQL _mysql;
};