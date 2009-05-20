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
#include <atlsmtpconnection.h>

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

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// �� MDI ������ â�� ����ϴ�.
	m_pMainWnd = new CMainFrame;
	if (!m_pMainWnd || !((CMainFrame*)m_pMainWnd)->LoadFrame(IDR_MAINFRAME))
	{
		delete m_pMainWnd;
		return FALSE;
	}

	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	CRect mainRect;
	m_pMainWnd->GetWindowRect( mainRect );
	m_pMainWnd->MoveWindow( 0, 0, 0, 0 );
	m_pMainWnd->ShowWindow( TRUE );

	// DB �ʱ�ȭ
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
	ProcessPacket();

	return TRUE;
}

BOOL CTestApp::ProcessPacket()
{
	//
	char message[2048];

	//
	RakSleep(30);

	Packet * p = _server->Receive();
	if (p==0)
		return FALSE;

	RakNet::BitStream inStream( p->data, p->length, false );

	unsigned char packetIdentifier;
	inStream.Read( packetIdentifier );

	switch (packetIdentifier)
	{
	case ID_DISCONNECTION_NOTIFICATION:
		{
			printf("ID_DISCONNECTION_NOTIFICATION\n");
			// ���� ����
			_userManager.DisConnect( p->systemAddress.ToString() );
		}
		break;

	case ID_NEW_INCOMING_CONNECTION:
		{
			printf("ID_NEW_INCOMING_CONNECTION from %s\n", p->systemAddress.ToString());
			// Ÿ�� �ƿ� ����
			_server->SetTimeoutTime( 1000, p->systemAddress );
		}
		break;

	case ID_CONNECTION_LOST:
		{
			printf("ID_CONNECTION_LOST\n");
			// ������ ���� ��ȣ �� �̸��� ��������
			std::string id = _userManager.GetUserID( p->systemAddress.ToString() );
			std::string number;
			BOOL ret = DBConnector::GetInstance().GetMobileNumber( id, number );
			std::string addr;
			ret =  DBConnector::GetInstance().GetEmailAddress( id, addr );
			// ������ ���� ��� �߼�
			SendSMS( number );
			// ���� �߼�
			SendMail( addr );
			// ���� ���� ����
			_userManager.DisConnect( p->systemAddress.ToString() );
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
			_server->Send(&outBuffer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);

			// ���� �� ���� ����
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

	return TRUE;
}

void CTestApp::SendSMS( std::string number )
{
	BOOL ret = FALSE;
	CSMS sms;
	RECT rc = { 0, 0, 0, 0};
	CString userid("dmlgus77");					// ���̵�
	CString passwd("rladmlgus");				// �н�����

	CString rcvno(number.c_str());				// �޴�(����)��ȣ
	CString callback("01196306210");			// �����¹�ȣ(ȸ�Ź�ȣ)
	CString message("��Ʈ�� ���� ���!!!!");	// �޽��� ���� (80����Ʈ ����)
	CString refname("���");					// ������ �̸�
	CString reserv("");							// �����Ͻ�(YYYYMMDDHHMISS ����, "" �̸� �ٷ� ����)

	if (sms.Create(NULL, WS_CHILD, rc, m_pMainWnd, 1) != 0)
	{
		sms.SetUser(userid, passwd);
		// �޴¹�ȣ, �����¹�ȣ, �޽�������(80����Ʈ), ������ �̸�, �����Ͻ� ������ �Է�.
		sms.Add(rcvno, callback, message, refname, reserv);
		ret = sms.Connect();
		ret = sms.Send();
		sms.Close();
	}
}

void CTestApp::SendMail( std::string addr )
{
	CoInitialize(0);

	CString sendaddr("naver01@naver.com");		// ������ ��� �̸��� �ּ�
	CString name("���ȼַ��");					// ������ ��� �̸�
	CString recvaddr( addr.c_str() );			// �޴� ��� �̸��� �ּ�
	CString subject("��Ʈ�� ���� ���!!!");		// ����
	CString text("��Ʈ�� ���� ���!!!!");		// ����

	CMimeMessage msg;
	msg.SetSender( sendaddr );
	msg.SetSenderName( name );
	msg.AddRecipient( recvaddr );
	msg.SetSubject( subject );
	msg.AddText( text );

	CSMTPConnection conn;
	if(conn.Connect( _T("rcvmail7.naver.com") ))
	{
		bool ret = conn.SendMessage(msg);
		conn.Disconnect();  
	}
}
