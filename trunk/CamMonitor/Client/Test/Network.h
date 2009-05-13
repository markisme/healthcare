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

	//
	void Disconnect();

	//
	RakPeerInterface * GetClient() { return _client; }

	//
	static Network & GetInstance() { return _instance; }

	//
	int _isSuccessAuth;

	//
	int _isConnecting;

private:
	static Network _instance;
	RakPeerInterface * _client;
};