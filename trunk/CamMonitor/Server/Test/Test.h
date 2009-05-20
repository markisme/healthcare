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
	}

	inline void DisConnect( std::string addr )
	{
		// ���� ����
	}

private:
	UserList userList;
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
	void SendSMS();

private:
	RakPeerInterface * _server;
	UserManager _userManager;
};
