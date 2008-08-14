#pragma once

#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include <assert.h>
//#include <cstdio>
//#include <cstring>
//#include <stdlib.h>
//#include <windows.h> // Sleep

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

private:
	static Network _instance;

	RakPeerInterface * _client;
};