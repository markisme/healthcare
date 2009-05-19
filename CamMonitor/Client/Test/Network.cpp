#include "stdafx.h"
#include "Network.h"
#include "CommonType.h"

Network Network::_instance;

Network::Network() : 
_isSuccessAuth( -1 ),
_isConnecting( 1 )
{
}

Network::~Network()
{
}

void Network::Init()
{
	// 서버 설정
	int clientPort = 200;
	//std::string ip = "211.189.20.246";
	std::string ip = "211.189.19.160";
	int serverPort = 10000;

	//
	RakNetStatistics *rss;
	_client=RakNetworkFactory::GetRakPeerInterface();
	_client->AllowConnectionResponseIPMigration(false);	
	
	//
	SocketDescriptor socketDescriptor( clientPort, 0 );
	_client->Startup(1,30,&socketDescriptor, 1);
	_client->SetOccasionalPing(true);
	BOOL b = _client->Connect(ip.c_str(), serverPort, "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"));	
	if (!b)
	{
		exit(1);
	}

	_isConnecting = 1;
	_isSuccessAuth = -1;
}

int Network::IsConnected()
{
	return _client->NumberOfConnections();
}

void Network::Uninit()
{
	//
	Sleep(500);

	//
	_client->Shutdown(300);
	RakNetworkFactory::DestroyRakPeerInterface(_client);
}

bool Network::ProcPacket()
{
	if( _client == NULL )
	{
		return FALSE;
	}

	Sleep(30);

	Packet* p;
	p = _client->Receive();

	if (p==0)
		return FALSE;

	RakNet::BitStream inStream( p->data, p->length, false );

	unsigned char packetIdentifier;
	inStream.Read( packetIdentifier );

	switch (packetIdentifier)
	{
	case ID_CONNECTION_LOST:
		{
			// 데이터 저장
			Network::GetInstance()._isConnecting = 0;
		}
		break;

	case S2CH_LOGIN_RES:
		{
			// 패킷 읽기
			int isSuccessAuth;
			inStream.Read( isSuccessAuth );

			// 데이터 저장
			Network::GetInstance()._isSuccessAuth = isSuccessAuth;
		}
		break;

	default:
		{
			
		}
		break;
	}

	_client->DeallocatePacket(p);

	return TRUE;
}

void Network::ReqLoginSend( std::string id, std::string pass )
{
	RakNet::BitStream outBuffer;
	outBuffer.Write( (unsigned char)MessageType::CH2S_LOGIN );

	outBuffer.Write( id );
	outBuffer.Write( pass );

	if( _client )
	{
		_client->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}

void Network::Disconnect()
{
	if( _client )
	{
		_client->Shutdown( 0 );
	}
}