#include "stdafx.h"
#include "Network.h"
#include "CommonType.h"

Network Network::_instance;

Network::Network()
{
}

Network::~Network()
{
}

void Network::Init( bool isHost, int clientPort, std::string ip, int serverPort )
{
	//
	RakNetStatistics *rss;
	_client=RakNetworkFactory::GetRakPeerInterface();
	_client->AllowConnectionResponseIPMigration(false);	
	
	//
	Network::GetInstance()._isHost = isHost;

	//
	SocketDescriptor socketDescriptor( clientPort, 0 );
	_client->Startup(1,30,&socketDescriptor, 1);
	_client->SetOccasionalPing(true);
	bool b = _client->Connect(ip.c_str(), serverPort, "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"));	
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
	case ID_DISCONNECTION_NOTIFICATION:
		break;
	case ID_ALREADY_CONNECTED:
		break;
	case ID_REMOTE_DISCONNECTION_NOTIFICATION:
		break;
	case ID_REMOTE_CONNECTION_LOST:
		break;
	case ID_CONNECTION_ATTEMPT_FAILED:
		break;
	case ID_NO_FREE_INCOMING_CONNECTIONS:
		break;
	case ID_MODIFIED_PACKET:
		break;
	case ID_INVALID_PASSWORD:
		break;
	case ID_CONNECTION_LOST:
		break;
	case ID_CONNECTION_REQUEST_ACCEPTED:
		break;
	case S2CH_SUCCESS_CONNECTED:
		{
			RakNet::BitStream stream;
			if( Network::GetInstance()._isHost )
			{
				stream.Write( MessageType::H2S_HOST_REQ );
			}
			else
			{
				stream.Write( MessageType::C2S_CLIENT_REQ );
			}

			_client->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);

			if( Network::GetInstance()._isHost )
			{
				RakNet::BitStream stream;
				stream.Write( MessageType::H2S_GET_USERINFO_LIST );
				_client->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
			}

			return TRUE;
		}
		break;
	case S2H_GET_USERINFO_LIST_RES:
		{
			int count = 0;
			inStream.Read( count );

			for( int num = 0; num < count; num++ )
			{
				UserInfo userInfo;
				inStream.Read( userInfo._userNo );
				inStream.Read( userInfo._userName );
				inStream.Read( userInfo._age );
				inStream.Read( userInfo._sex );
				inStream.Read( userInfo._tall );
				inStream.Read( userInfo._weight );
				inStream.Read( userInfo._blood );
				inStream.Read( userInfo._tel );
				inStream.Read( userInfo._pic );
				_userList.push_back( userInfo );
			}

			return TRUE;
		}
		break;
	case S2CH_GET_USERDATA_LIST_RES:
		{
			_userDataList.clear();

			int count = 0;
			inStream.Read( count );

			for( int num = 0; num < count; num++ )
			{
				UserData userData;
				inStream.Read( userData._year );
				inStream.Read( userData._month );
				inStream.Read( userData._day );
				inStream.Read( userData._hour );
				inStream.Read( userData._min );
				inStream.Read( userData._value );
				inStream.Read( userData._temp );
				_userDataList.push_back( userData );
			}

			return TRUE;
		}
		break;
	case S2H_CLIENT_DATA_REQ:
		{
			_dataList.clear();

			int count = 0;
			inStream.Read( count );

			for( int num = 0; num < count; num++ )
			{
				PacketData data;
				inStream.Read( data );

				_dataList.push_back( data );
			}

			return TRUE;
		}
		break;
	default:
		{
			
		}
		break;
	}

	_client->DeallocatePacket(p);
}

void Network::ClientDataSend()
{
	RakNet::BitStream outBuffer;
	outBuffer.Write( (unsigned char)MessageType::C2S_CLIENT_DATA );

	int count = _dataList.size();
	outBuffer.Write( count );

	for( int num = 0; num < count; num++ )
	{
		PacketData data = _dataList[ num ];
		outBuffer.Write( data );
	}

	RakPeerInterface * client = Network::GetInstance().GetClient();
	if( client )
	{
		client->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}

void Network::ReqGetUserData( int userNo )
{
	RakNet::BitStream outBuffer;
	outBuffer.Write( (unsigned char)MessageType::CH2S_GET_USERDATA_LIST );
	outBuffer.Write( userNo );

	RakPeerInterface * client = Network::GetInstance().GetClient();
	if( client )
	{
		client->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}

void Network::ReqAddUserData( int userNo, UserData & userData )
{
	RakNet::BitStream outBuffer;
	outBuffer.Write( (unsigned char)MessageType::C2S_ADD_USERDATA );
	outBuffer.Write( userNo );

	outBuffer.Write( userData._year );
	outBuffer.Write( userData._month );
	outBuffer.Write( userData._day );
	outBuffer.Write( userData._hour );
	outBuffer.Write( userData._min );
	outBuffer.Write( userData._value );
	outBuffer.Write( userData._temp );

	RakPeerInterface * client = Network::GetInstance().GetClient();
	if( client )
	{
		client->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}

int Network::GetIndexForUserNo( int userNo )
{
	int count = _userList.size();
	for( int num = 0; num < count; num++ )
	{
		UserInfo & userInfo = _userList[ num ];
		int curUserNo = atoi( userInfo._userNo.c_str() );
		if( curUserNo == userNo )
		{
			return num;
		}
	}
	
	return -1;
}