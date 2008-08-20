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

typedef std::vector<PacketData> DataList;

class Network
{
public:
	Network();
	~Network();

	void Init( bool isHost, int clientPort, std::string ip, int serverPort );
	void Uninit();
	void ProcPacket();
	void Send();

	RakPeerInterface * GetClient() { return _client; }

	static Network & GetInstance() { return _instance; }

	DataList & GetDataList() { return _dataList; }

	BOOL _isHost;

private:
	static Network _instance;
	RakPeerInterface * _client;
	DataList _dataList;
};