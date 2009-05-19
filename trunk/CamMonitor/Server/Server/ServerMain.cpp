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
#include <stdio.h>
#include <iostream>
#include <atlsmtpconnection.h>

#ifdef _WIN32
#include "Kbhit.h"
#endif
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>

#include "CommonType.h"
#include "msgID.h"

void SendMail();

typedef std::map<std::string,std::string> UserList;

class UserManager
{
public:
	UserManager() {};
	~UserManager() {};

	inline void ConnectUser( std::string addr, std::string id )
	{
		// ���� �߰�
	}

	inline void DisConnect( std::string addr )
	{
		// ���� ����
	}

private:
	UserList userList;
};


int main(void)
{
	// ���� ������
	UserManager userManager;
	
	// DB �ʱ�ȭ
	DBConnector::GetInstance().Init();

	RakPeerInterface *server=RakNetworkFactory::GetRakPeerInterface();
	RakNetStatistics *rss;

	int i = server->GetNumberOfAddresses();
	server->SetIncomingPassword("Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));

	Packet* p;

	unsigned char packetIdentifier;

	char portstring[30];
	strcpy(portstring, "10000");
	
	puts("Starting server.");

	SocketDescriptor socketDescriptor(atoi(portstring),0);
	bool b = server->Startup(32, 30, &socketDescriptor, 1 );
	server->SetMaximumIncomingConnections(10);
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
					// ���� ����
					userManager.DisConnect( p->systemAddress.ToString() );
				}
				break;
		
			case ID_NEW_INCOMING_CONNECTION:
				{
					printf("ID_NEW_INCOMING_CONNECTION from %s\n", p->systemAddress.ToString());
					// Ÿ�� �ƿ� ����
					server->SetTimeoutTime( 1000, p->systemAddress );
				}
				break;

			case ID_CONNECTION_LOST:
				{
					printf("ID_CONNECTION_LOST\n");
					// ������ ���� ��� �߼�
					// ���� �߼�
					userManager.DisConnect( p->systemAddress.ToString() );
				}
				break;

			case CH2S_LOGIN:
				{
					// ��Ŷ �б�
					std::string id;
					inStream.Read( id );

					std::string pass;
					inStream.Read( pass );

					// DB���� ������ Ȯ��
					BOOL isAuth = DBConnector::GetInstance().ConfirmLogin( id, pass );

					// ���� ��Ŷ ����
					RakNet::BitStream outBuffer;
					outBuffer.Write( (unsigned char)MessageType::S2CH_LOGIN_RES );
					outBuffer.Write( isAuth );

					// ��Ŷ ������
					server->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);

					// ���� �� ���� ����
					if( isAuth )
					{
						userManager.ConnectUser( p->systemAddress.ToString(), id );
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

void SendMail()
{
	CoInitialize(0);

	CMimeMessage msg;
	msg.SetSender( _T("naid01@naver.com") );
	msg.SetSenderName( _T("�ѱ���") );
	msg.AddRecipient( _T("naid01@naver.com") );
	msg.SetSubject( _T("ȯ�� ���� ���") );
	msg.AddText( _T("ȯ�� ���¿� ������ �ֽ��ϴ�.") );

	CSMTPConnection conn;
	if(conn.Connect( _T("rcvmail7.naver.com") ))
	{
		if( conn.SendMessage(msg) == TRUE )
			std::cout << _T("Mail sent successfully") << std::endl;
		conn.Disconnect();  
	}
}
