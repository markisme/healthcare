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
		// 유저 추가
	}

	inline void DisConnect( std::string addr )
	{
		// 유저 삭제
	}

private:
	UserList userList;
};


int main(void)
{
	// 유저 관리자
	UserManager userManager;
	
	// DB 초기화
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
					// 정상 종료
					userManager.DisConnect( p->systemAddress.ToString() );
				}
				break;
		
			case ID_NEW_INCOMING_CONNECTION:
				{
					printf("ID_NEW_INCOMING_CONNECTION from %s\n", p->systemAddress.ToString());
					// 타임 아웃 설정
					server->SetTimeoutTime( 1000, p->systemAddress );
				}
				break;

			case ID_CONNECTION_LOST:
				{
					printf("ID_CONNECTION_LOST\n");
					// 비정상 종료 경고 발송
					// 메일 발송
					userManager.DisConnect( p->systemAddress.ToString() );
				}
				break;

			case CH2S_LOGIN:
				{
					// 패킷 읽기
					std::string id;
					inStream.Read( id );

					std::string pass;
					inStream.Read( pass );

					// DB에서 데이터 확인
					BOOL isAuth = DBConnector::GetInstance().ConfirmLogin( id, pass );

					// 보낼 패킷 쓰기
					RakNet::BitStream outBuffer;
					outBuffer.Write( (unsigned char)MessageType::S2CH_LOGIN_RES );
					outBuffer.Write( isAuth );

					// 패킷 보내기
					server->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);

					// 인증 된 유저 접속
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
	msg.SetSenderName( _T("한규혁") );
	msg.AddRecipient( _T("naid01@naver.com") );
	msg.SetSubject( _T("환자 상태 경고") );
	msg.AddText( _T("환자 상태에 문제가 있습니다.") );

	CSMTPConnection conn;
	if(conn.Connect( _T("rcvmail7.naver.com") ))
	{
		if( conn.SendMessage(msg) == TRUE )
			std::cout << _T("Mail sent successfully") << std::endl;
		conn.Disconnect();  
	}
}
