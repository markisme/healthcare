#pragma once
#include "resource.h"       // �� ��ȣ�Դϴ�.

#include <string.h>
#include <vector>
#include <map>

class RakPeerInterface;

typedef std::map<std::string,std::string> UserList;

class UserManager
{
public:
	UserManager() {};
	~UserManager() {};

	inline void ConnectUser( std::string addr, std::string id )
	{
		// ���� �߰�
		_userList[ addr ] = id;
	}

	inline void DisConnect( std::string addr )
	{
		// ���� ����
		UserList::iterator it = _userList.begin();
		for( ; it != _userList.end(); it++ )
		{
			std::string addrKey = it->first;
			if( addrKey == addr )
			{
				_userList.erase( it );
				break;
			}
		}
	}

	inline std::string GetUserID( std::string addr )
	{
		// ���� ã��
		UserList::iterator it = _userList.begin();
		for( ; it != _userList.end(); it++ )
		{
			std::string addrKey = it->first;
			if( addrKey == addr )
			{
				return it->second;
			}
		}
	}

private:
	UserList _userList;
};


class CTestApp : public CWinApp
{
public:
	CTestApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	virtual BOOL OnIdle( LONG lCount );

	void Init();
	void Uninit();
	void ProcPacket();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	void SendSMS( std::string number );

private:
	RakPeerInterface * _server;
	UserManager _userManager;
};
