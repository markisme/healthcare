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

void Network::Init( int clientPort, std::string ip, int serverPort )
{
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
}

void Network::ReqLoginSend( std::string id, std::string pass )
{
	RakNet::BitStream outBuffer;
	outBuffer.Write( (unsigned char)MessageType::CH2S_LOGIN );

	outBuffer.Write( id );
	outBuffer.Write( pass );

	RakPeerInterface * client = Network::GetInstance().GetClient();
	if( client )
	{
		client->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}