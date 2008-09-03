#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "DBConnector.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#ifdef _WIN32
#include "Kbhit.h"
#endif
#include <stdio.h>
#include <string.h>
#include <vector>


#include "CommonType.h"
#include "msgID.h"

struct PeerType
{
	SystemAddress _addr;
	bool _isHost;
};

int main(void)
{
	// DB �ʱ�ȭ
	DBConnector::GetInstance().Init();

	RakPeerInterface *server=RakNetworkFactory::GetRakPeerInterface();
	RakNetStatistics *rss;

	int i = server->GetNumberOfAddresses();
	server->SetIncomingPassword("Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));

	Packet* p;

	unsigned char packetIdentifier;

	std::vector<PeerType> _peerList;
	SystemAddress clientID=UNASSIGNED_SYSTEM_ADDRESS;

	char portstring[30];
	strcpy(portstring, "10000");
	
	puts("Starting server.");

	SocketDescriptor socketDescriptor(atoi(portstring),0);
	bool b = server->Startup(32, 30, &socketDescriptor, 1 );
	server->SetMaximumIncomingConnections(2);
	if (!b)
	{
		exit(1);
	}

	server->SetOccasionalPing(true);
	printf("My IP is %s\n", server->GetLocalIP(0));

	//
	char message[2048];

	while (1)
	{
		//
		RakSleep(30);

		p = server->Receive();
		if (p==0)
			continue;

		RakNet::BitStream inStream( p->data, p->length, false );
		
		unsigned char packetIdentifier;
		inStream.Read( packetIdentifier );

		switch (packetIdentifier)
		{
			case ID_DISCONNECTION_NOTIFICATION:
				{
					printf("ID_DISCONNECTION_NOTIFICATION\n");
				}
				break;
		
			case ID_NEW_INCOMING_CONNECTION:
				{
					printf("ID_NEW_INCOMING_CONNECTION from %s\n", p->systemAddress.ToString());

					RakNet::BitStream stream;
					stream.Write( (unsigned char)MessageType::S2CH_SUCCESS_CONNECTED );
					server->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
				}
				break;

			case ID_CONNECTION_LOST:
				{
					printf("ID_CONNECTION_LOST\n");
				}
				break;

			case C2S_CLIENT_REQ:
				{
					PeerType type;
					type._addr = p->systemAddress;
					type._isHost = false;
					_peerList.push_back( type );
				}
				break;

			case H2S_HOST_REQ:
				{
					PeerType type;
					type._addr = p->systemAddress;
					type._isHost = true;
					_peerList.push_back( type );
				}
				break;

			case H2S_GET_USERINFO_LIST:
				{
					// ���� ��Ŷ ����
					RakNet::BitStream outBuffer;
					outBuffer.Write( (unsigned char)MessageType::S2H_GET_USERINFO_LIST_RES );

					UserList userInfoList;
					DBConnector::GetInstance().GetUserInfo( userInfoList );

					int count = userInfoList.size();
					outBuffer.Write( count );

					for( int num = 0; num < count; num++ )
					{
						UserInfo & userInfo = userInfoList[ num ];
						outBuffer.Write( userInfo._userNo );
						outBuffer.Write( userInfo._userName );
						outBuffer.Write( userInfo._age );
						outBuffer.Write( userInfo._sex );
						outBuffer.Write( userInfo._tall );
						outBuffer.Write( userInfo._weight );
						outBuffer.Write( userInfo._blood );
						outBuffer.Write( userInfo._tel );
						outBuffer.Write( userInfo._pic );
					}

					// ȣ��Ʈ���� ��Ŷ ������
					server->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
				}
				break;

			case CH2S_GET_USERDATA_LIST:
				{
					int userNo;
					inStream.Read( userNo );

					UserDataList userDataList;
					DBConnector::GetInstance().GetUserData( userNo, userDataList );

					// ���� ��Ŷ ����
					RakNet::BitStream outBuffer;
					outBuffer.Write( (unsigned char)MessageType::S2CH_GET_USERDATA_LIST_RES );

					int count = userDataList.size();
					outBuffer.Write( count );

					for( int num = 0; num < count; num++ )
					{
						UserData & userData = userDataList[ num ];
						outBuffer.Write( userData._year );
						outBuffer.Write( userData._month );
						outBuffer.Write( userData._day );
						outBuffer.Write( userData._hour );
						outBuffer.Write( userData._min );
						outBuffer.Write( userData._value );
						outBuffer.Write( userData._temp );
					}

					// ȣ��Ʈ���� ��Ŷ ������
					server->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
				}
				break;

			case C2S_ADD_USERDATA:
				{
					int userNo = 0;
					inStream.Read( userNo );

					UserData userData;
					inStream.Read( userData._year );
					inStream.Read( userData._month );
					inStream.Read( userData._day );
					inStream.Read( userData._hour );
					inStream.Read( userData._min );
					inStream.Read( userData._value );
					inStream.Read( userData._temp );
					
					//
					DBConnector::GetInstance().AddUserData( userNo, userData );
				}
				break;

			case C2S_CLIENT_DATA:
				{
					// ��Ŷ �б�
					std::vector<PacketData> dataList;

					int count = 0;
					inStream.Read( count );

					for( int num = 0; num < count; num++ )
					{
						PacketData data;
						inStream.Read( data );

						dataList.push_back( data );
					}

					// ���� ��Ŷ ����
					RakNet::BitStream outBuffer;
					outBuffer.Write( (unsigned char)MessageType::S2H_CLIENT_DATA_REQ );

					int dataCount = dataList.size();
					outBuffer.Write( dataCount );
		
					for( int num = 0; num < dataCount; num++ )
					{
						PacketData data = dataList[ num ];
						outBuffer.Write( data );
					}

					// ȣ��Ʈ���� ��Ŷ ������
					int peerCount = _peerList.size();
					for( int num = 0; num < peerCount; num++ )
					{
						if( _peerList[ num ]._isHost )
						{
							server->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, _peerList[ num ]._addr, false);
						}
					}
				}
				break;

			default:
				{
				}
				break;
		}

		server->DeallocatePacket(p);
	}

	server->Shutdown(300);
	RakNetworkFactory::DestroyRakPeerInterface(server);

	DBConnector::GetInstance().Uninit();

	return 0;
}
