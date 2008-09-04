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

int test();


#include "CommonType.h"
#include "msgID.h"

struct PeerType
{
	SystemAddress _addr;
	bool _isHost;
};

int main(void)
{
	test();

	// DB 초기화
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
				}
				break;

			case ID_CONNECTION_LOST:
				{
					printf("ID_CONNECTION_LOST\n");
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
					BOOL isHost = DBConnector::GetInstance().ConfirmHost( id );
					int userNo = DBConnector::GetInstance().GetUserNo( id );

					// Peer 설정
					PeerType type;
					type._addr = p->systemAddress;
					type._isHost = isHost;
					_peerList.push_back( type );

					// 보낼 패킷 쓰기
					RakNet::BitStream outBuffer;
					outBuffer.Write( (unsigned char)MessageType::S2CH_LOGIN_RES );
					outBuffer.Write( isAuth );
					outBuffer.Write( isHost );
					outBuffer.Write( userNo );

					// 패킷 보내기
					server->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
				}
				break;

			case H2S_GET_USERINFO_LIST:
				{
					// DB에서 데이터 가져오기
					UserList userInfoList;
					DBConnector::GetInstance().GetUserInfo( userInfoList );

					// 보낼 패킷 쓰기
					RakNet::BitStream outBuffer;
					outBuffer.Write( (unsigned char)MessageType::S2H_GET_USERINFO_LIST_RES );

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

					// 패킷 보내기
					server->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
				}
				break;

			case CH2S_GET_USERDATA_LIST:
				{
					// 패킷 읽기
					int userNo;
					inStream.Read( userNo );

					// DB에서 데이터 가져오기
					UserDataList userDataList;
					DBConnector::GetInstance().GetUserData( userNo, userDataList );

					// 보낼 패킷 쓰기
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

					//
					server->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
				}
				break;

			case C2S_ADD_USERDATA:
				{
					// 패킷 읽기
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
					
					// DB에 저장하기
					DBConnector::GetInstance().AddUserData( userNo, userData );

					// 알람기능 가동
				}
				break;

			case C2S_CLIENT_DATA:
				{
					// 패킷 읽기
					int userNo = 0;
					inStream.Read( userNo );

					int count = 0;
					inStream.Read( count );

					std::vector<PacketData> dataList;
					for( int num = 0; num < count; num++ )
					{
						PacketData data;
						inStream.Read( data );

						dataList.push_back( data );
					}

					// 보낼 패킷 쓰기
					RakNet::BitStream outBuffer;
					outBuffer.Write( (unsigned char)MessageType::S2H_CLIENT_DATA_RES );
					outBuffer.Write( userNo );

					int dataCount = dataList.size();
					outBuffer.Write( dataCount );
		
					for( int num = 0; num < dataCount; num++ )
					{
						PacketData data = dataList[ num ];
						outBuffer.Write( data );
					}

					// 호스트에게 패킷 보내기
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

// 잃어버렸던 소스를 찾아냄 --;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
//#include <getopt.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>

static char *from_addr  = NULL;                /* 보내는 사람 주소        */
static char *reply_addr = NULL;                /* 받는 사람 주소        */
static char *mailhost   = NULL;                /* 메일서버 주소        */
static int   mailport   = 25;                /* 메일서버 포트        */
static char *subject    = 0;                /* 메일 제목                 */
static FILE *sfp;
static FILE *rfp;

/*
** 메일서버에서 응답받기
*/
void Get_Response(void)
{
	char buf[BUFSIZ];

	while (fgets(buf, sizeof(buf), rfp)) {
		buf[strlen(buf)-1] = 0;
		/*
		printf("%s --> %s\n", mailhost, buf);
		*/
		if (!isdigit(buf[0]) || buf[0] > '3') {
			printf("unexpected reply: %s\n", buf);
			exit(-1);
		}
		if (buf[4] != '-')
			break;
	}
	return;
}

/*
** 메일서버로 내용 전송하고 응답받기
*/
void Chat(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(sfp, fmt, ap);
	va_end(ap);

	/*
	va_start(ap, fmt);
	printf("%s <-- \n", mailhost);
	vprintf(fmt, ap);
	va_end(ap);
	*/

	fflush(sfp);
	Get_Response();
}

/*
**  main 함수
*/

void Usage(void)
{
	printf("\n");
	printf("usage   : cymail [-S host] [-s subject] <-r reply-addr> FILE\n");
	printf("options :\n");
	/*
	printf("  -S, --smtp-host=HOST        host where MTA can be contacted via SMTP\n")
	*/
	printf("          -S,       host where MTA can be contacted via SMTP\n");
	printf("          -s,       subject line of message\n");
	printf("          -r,       address of the sender for replies\n");
	printf("\n");
	exit(0);
}

int test()
{
	FILE *fpFile;
	char buf[BUFSIZ];
	char my_name[BUFSIZ];
	struct sockaddr_in sin;
	struct hostent *hp;
	struct servent *sp;
	int c;
	int s;
	int r;
	char *cp;

	if (mailhost == NULL)
	{
		if ((cp = getenv("SMTPSERVER")) != NULL)
		{
			mailhost = cp;
		}
		else
		{
			printf("SMTPSERVER is not setting\n");        
			exit(-1);
		}
	}

	/*
	*  메일 서버에 연결
	*/

	if ((hp = gethostbyname(mailhost)) == NULL) {
		printf("%s: unknown host\n", mailhost);
		exit(-1);
	}
	if (hp->h_addrtype != AF_INET) {
		printf("unknown address family: %d\n", hp->h_addrtype);
		exit(-1);
	}

	memset((char *)&sin, 0, sizeof(sin));
	memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);

	sin.sin_family = hp->h_addrtype;
	sin.sin_port = htons(mailport);

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket: error\n");
		exit(-1);
	}
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		printf("connect: error\n");
		exit(-1);
	}
	//if ((r = dup(s)) < 0) {
	//	printf("dup: error\n");
	//	exit(-1);
	//}
	if ((sfp = fdopen(s, "w")) == 0) {
		printf("fdopen: error\n");
		exit(-1);
	}
	if ((rfp = fdopen(r, "r")) == 0) {
		printf("fdopen: error\n");
		exit(-1);
	}

	/* 
	*  SMTP 헤더보내기
	*/
	Get_Response(); /* 초기 메시지 받기 */
	/*
	Chat("HELO %s\r\n", my_name);
	*/
	Chat("MAIL FROM: <%s>\r\n", from_addr);
	Chat("RCPT TO: <%s>\r\n", reply_addr);
	Chat("DATA\r\n");

	/* 
	*  메일 제목
	*/
	if( subject != NULL)
	{
		fprintf(sfp, "Subject: %s\r\n", subject);
		fprintf(sfp, "\r\n");
	}

	///* 
	//*  메일 내용 읽기
	//*/
	//if ((fpFile = fopen(argv[argc - 1], "r")) == NULL )
	//{
	//	printf("no message file \n");
	//	exit(-1);
	//}
	//else
	//{
	//	while(fgets(buf, sizeof(buf), fpFile))
	//	{
	//		buf[strlen(buf)-1] = 0;
	//		if (strcmp(buf, ".") == 0)
	//		{
	//			fprintf(sfp, ".\r\n");
	//		}
	//		else
	//		{
	//			fprintf(sfp, "%s\r\n", buf);
	//		}
	//	}
	//	fclose(fpFile);
	//}

	/* 
	*  내용 종료 및 통신 종료
	*/
	Chat(".\r\n");
	Chat("QUIT\r\n");

	printf("Your mail is sended successfully !!\n");
	exit(0);
} 