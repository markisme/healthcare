#pragma once

#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include <assert.h>
#include <vector>
#include "CommonType.h"
#include "msgID.h"
#include <map>

typedef std::map<int,DataList> DataMap;

class Network
{
public:
	Network();
	~Network();

	void Init( int clientPort, std::string ip, int serverPort );
	void Uninit();

	//
	bool ProcPacket();

	//
	void ReqLoginSend( std::string id, std::string pass );
	void ReqClientDataSend();
	void ReqGetUserInfoSend();
	void ReqGetUserDataSend( int userNo );
	void ReqAddUserDataSend( UserData & userData );

	RakPeerInterface * GetClient() { return _client; }

	static Network & GetInstance() { return _instance; }

	DataList & GetDataList( int userNo ) { return _dataMap[ userNo ]; }
	UserList & GetUserInfoList() { return _userList; }
	UserDataList & GetUserDataList() { return _userDataList; }
	int GetIndexForUserNo( int userNo );

	BOOL _isSuccessAuth;
	BOOL _isHost;
	int _myUserNo;

private:
	static Network _instance;
	RakPeerInterface * _client;
	DataMap _dataMap;
	UserList _userList;
	UserDataList _userDataList;
};