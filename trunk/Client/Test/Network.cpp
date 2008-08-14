#include "stdafx.h"
#include "Network.h"

Network Network::_instance;

unsigned char GetPacketIdentifier(Packet *p);

Network::Network()
{
}

Network::~Network()
{
}

void Network::Init()
{
	RakNetStatistics *rss;
	_client=RakNetworkFactory::GetRakPeerInterface();

	SystemAddress clientID=UNASSIGNED_SYSTEM_ADDRESS;
	char ip[30], serverPort[30], clientPort[30];

	strcpy(clientPort, "200");
	strcpy(ip, "211.189.19.161");
	strcpy(serverPort, "10000");

	_client->AllowConnectionResponseIPMigration(false);	

	SocketDescriptor socketDescriptor(atoi(clientPort),0);
	_client->Startup(1,30,&socketDescriptor, 1);
	_client->SetOccasionalPing(true);
	bool b = _client->Connect(ip, atoi(serverPort), "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"));	
	if (!b)
	{
		exit(1);
	}
}

void Network::Uninit()
{
	_client->Shutdown(300);
	RakNetworkFactory::DestroyRakPeerInterface(_client);
}

void Network::ProcPacket()
{
	// Loop for input
	//while (1)
	{
		Sleep(30);

		Packet* p;
		unsigned char packetIdentifier;

		p = _client->Receive();
		if (p==0)
			return; // Didn't get any packets

		packetIdentifier = GetPacketIdentifier(p);
		switch (packetIdentifier)
		{
			case ID_DISCONNECTION_NOTIFICATION:
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_ALREADY_CONNECTED:
				printf("ID_ALREADY_CONNECTED\n");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("ID_REMOTE_CONNECTION_LOST\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
				break;
			case ID_CONNECTION_BANNED:
				printf("We are banned from this server.\n");
				break;			
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;
			case ID_MODIFIED_PACKET:
				printf("ID_MODIFIED_PACKET\n");
				break;
			case ID_INVALID_PASSWORD:
				printf("ID_INVALID_PASSWORD\n");
				break;
			case ID_CONNECTION_LOST:
				printf("ID_CONNECTION_LOST\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
				break;
			default:
					printf("%s\n", p->data);
				break;
		}

		_client->DeallocatePacket(p);
	}
}

unsigned char GetPacketIdentifier(Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else
		return (unsigned char) p->data[0];
}