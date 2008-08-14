#pragma once

#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include <assert.h>
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

class Network
{
public:
	Network();
	~Network();

	void Init();
	void Uninit();
	void ProcPacket();

	RakPeerInterface * GetClient() { return _client; }

	static Network & GetInstance() { return _instance; }

	DataList & GetDataList() { return _dataList; }

	BOOL _isHost;

private:
	static Network _instance;
	RakPeerInterface * _client;
	DataList _dataList;
};