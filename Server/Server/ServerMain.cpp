#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
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

#if defined(_CONSOLE_2)
#include "Console2SampleIncludes.h"
#endif

#ifdef _CONSOLE_2
_CONSOLE_2_SetSystemProcessParams
#endif

int main(void)
{
	RakPeerInterface *server=RakNetworkFactory::GetRakPeerInterface();
	RakNetStatistics *rss;

	int i = server->GetNumberOfAddresses();
	server->SetIncomingPassword("Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));

	Packet* p;

	unsigned char packetIdentifier;

	std::vector<SystemAddress> _clientList;
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
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
		
			case ID_NEW_INCOMING_CONNECTION:
				printf("ID_NEW_INCOMING_CONNECTION from %s\n", p->systemAddress.ToString());
				clientID=p->systemAddress;
				_clientList.push_back( clientID );
				break;

			case ID_MODIFIED_PACKET:
				printf("ID_MODIFIED_PACKET\n");
				break;

			case ID_CONNECTION_LOST:
				printf("ID_CONNECTION_LOST\n");
				break;

			default:
				//int x = 0;
				//inStream.Read( x );
				//
				//int y = 0;
				//inStream.Read( y );

				//printf("%d, %d\n", x, y );

				int count = _clientList.size();
				for( int num = 0; num < count; num++ )
				{
					if( _clientList[ num ] != p->systemAddress )
					{
						sprintf(message, "%s", p->data);
						server->Send(&inStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, _clientList[ num ], false);
					}
				}
			
				break;
		}

		server->DeallocatePacket(p);
	}

	server->Shutdown(300);
	RakNetworkFactory::DestroyRakPeerInterface(server);

	return 0;
}
