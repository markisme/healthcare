#pragma once

#include <vector>

struct PacketData
{
	PacketData( int x = 0, int y = 0 ) : _x( x ), _y( y ) {};
	~PacketData() {};

	int _x;
	int _y;
	int _temp;	
};

typedef std::vector<PacketData> DataList;

struct UserInfo
{
	std::string _userNo;
	std::string _userName;
	std::string _age;
	std::string _sex;
	std::string _tall;
	std::string _weight;
	std::string _blood;
	std::string _tel;
	std::string _pic;
};

typedef std::vector<UserInfo> UserList;

struct UserData
{
	std::string _year;
	std::string _month;
	std::string _day;
	std::string _hour;
	std::string _min;
	std::string _value;
	std::string _temp;
};

typedef std::vector<UserData> UserDataList;