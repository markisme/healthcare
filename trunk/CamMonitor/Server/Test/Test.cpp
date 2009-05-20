#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "CSMS.h"

#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "DBConnector.h"

#ifdef _WIN32
#include "Kbhit.h"
#endif

#include "CommonType.h"
#include "msgID.h"

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
END_MESSAGE_MAP()

CTestApp::CTestApp()
{
}

CTestApp theApp;

BOOL CTestApp::InitInstance()
{
	CWinApp::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// 주 MDI 프레임 창을 만듭니다.
	m_pMainWnd = new CMainFrame;
	if (!m_pMainWnd || !((CMainFrame*)m_pMainWnd)->LoadFrame(IDR_MAINFRAME))
	{
		delete m_pMainWnd;
		return FALSE;
	}

	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	CRect mainRect;
	m_pMainWnd->GetWindowRect( mainRect );
	m_pMainWnd->MoveWindow( 0, 0, 0, 0 );
	m_pMainWnd->ShowWindow( TRUE );

	// DB 초기화
	DBConnector::GetInstance().Init();

	_server=RakNetworkFactory::GetRakPeerInterface();
	RakNetStatistics *rss;

	int i = _server->GetNumberOfAddresses();
	_server->SetIncomingPassword("Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));

	char portstring[30];
	strcpy(portstring, "10000");
	
	puts("Starting server.");

	SocketDescriptor socketDescriptor(atoi(portstring),0);
	bool b = _server->Startup(32, 30, &socketDescriptor, 1 );
	_server->SetMaximumIncomingConnections(10);
	if (!b)
	{
		exit(1);
	}

	_server->SetOccasionalPing(true);
	printf("My IP is %s\n", _server->GetLocalIP(0));

	return TRUE;
}

int CTestApp::ExitInstance()
{
	_server->Shutdown(300);
	RakNetworkFactory::DestroyRakPeerInterface(_server);

	DBConnector::GetInstance().Uninit();

	return TRUE;
}

BOOL CTestApp::OnIdle( LONG lCount )
{
	//
	Packet* p;

	unsigned char packetIdentifier;

	//
	char message[2048];

	while (1)
	{
		//
		RakSleep(30);

		p = _server->Receive();
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
					_userManager.DisConnect( p->systemAddress.ToString() );
				}
				break;
		
			case ID_NEW_INCOMING_CONNECTION:
				{
					printf("ID_NEW_INCOMING_CONNECTION from %s\n", p->systemAddress.ToString());
					// 타임 아웃 설정
					_server->SetTimeoutTime( 1000, p->systemAddress );
				}
				break;

			case ID_CONNECTION_LOST:
				{
					printf("ID_CONNECTION_LOST\n");
					// 비정상 유저 번호 찾기
					std::string id = _userManager.GetUserID( p->systemAddress.ToString() );
					std::string number;
					BOOL ret = DBConnector::GetInstance().GetMobileNumber( id, number );
					// 비정상 종료 경고 발송
					// 메일 발송
					_userManager.DisConnect( p->systemAddress.ToString() );
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
					_server->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);

					// 인증 된 유저 접속
					if( isAuth )
					{
						_userManager.ConnectUser( p->systemAddress.ToString(), id );
					}
				}
				break;

			default:
				{
				}
				break;
		}

		_server->DeallocatePacket(p);
	}

	return TRUE;
}

void CTestApp::SendSMS( std::string number )
{
	BOOL ret = FALSE;
	CSMS sms;
	RECT rc = { 0, 0, 0, 0};
	CString userid("dmlgus77");	// 아이디
	CString passwd("rladmlgus");	// 패스워드

	CString rcvno(number.c_str());		// 받는(수신)번호
	CString callback("01196306210");	// 보내는번호(회신번호)
	CString message("노트북 경고!!!!");	// 메시지 내용 (80바이트 이하)
	CString refname("경고");			// 참조용 이름
	CString reserv("");					// 예약일시(YYYYMMDDHHMISS 형식, "" 이면 바로 전송)

	if (sms.Create(NULL, WS_CHILD, rc, m_pMainWnd, 1) != 0)
	{
		sms.SetUser(userid, passwd);
		// 받는번호, 보내는번호, 메시지내용(80바이트), 참조용 이름, 예약일시 순으로 입력.
		sms.Add(rcvno, callback, message, refname, reserv);
		ret = sms.Connect();
		ret = sms.Send();
		sms.Close();
	}
}