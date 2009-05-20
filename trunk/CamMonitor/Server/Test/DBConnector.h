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

	bool ConfirmLogin( std::string id, std::string pass );

	bool GetMobileNumber( std::string id, std::string & mobileNumber );

	bool GetEmailAddress( std::string id, std::string & addr );

	static DBConnector & GetInstance() { return _instance; }

private:
	static DBConnector _instance;

	MYSQL _mysql;
};