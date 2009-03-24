// Test.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "TestView.h"
#include "Network.h"
#include "LoginDlg.h"
#include <windows.h>

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
END_MESSAGE_MAP()

CTestApp::CTestApp()
{
}

CTestApp theApp;

BOOL CTestApp::InitInstance()
{
	CWinApp::InitInstance();

	// 클라이언트 시간 설정
	CTime t = CTime::GetCurrentTime();
	int curMin = t.GetMinute();
	_lastMin = curMin;

	bool isHost = false;
	std::string id;
	std::string pw;
	int clientPort = 200;
	std::string ip = "211.189.19.160";
	int serverPort = 10000;

	// 네트워크 초기화
	Network::GetInstance().Init( clientPort, ip, serverPort );

	LoginDlg dlg;
	if( dlg.DoModal() != IDOK )
	{
		return -1;
	}

	// 호스트인 경우 유저 리스트 요청
	if( Network::GetInstance()._isHost )
	{
		Network::GetInstance().ReqGetUserInfoSend();
	}

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}

	//
	_mainFrm = pMainFrame;
	m_pMainWnd = pMainFrame;

	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	CRect mainRect;
	pMainFrame->GetWindowRect( mainRect );
	pMainFrame->MoveWindow( mainRect.left, mainRect.top, 800, 600 );

	if( Network::GetInstance()._isHost == false )
	{
		hComm = CreateFile(L"COM2", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if(hComm == INVALID_HANDLE_VALUE)
			return -1;

		//* 포트 설정 얻고 수정 하여 반영하기
		DCB dcb;
		
		// 설정 얻기
		if( !GetCommState(hComm, &dcb) )
			return -1;
		
		// 설정 변경
		dcb.BaudRate = CBR_9600;
		dcb.fParity = FALSE;
		dcb.fNull = FALSE;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		
		// 설정 반영
		if( !SetCommState(hComm, &dcb) )
			return -1;

		//*   타임아웃 설정
		COMMTIMEOUTS timeouts;
		timeouts.ReadIntervalTimeout = 0;
		timeouts.ReadTotalTimeoutMultiplier = 0;
		timeouts.ReadTotalTimeoutConstant = 0;
		timeouts.WriteTotalTimeoutMultiplier = 0;
		timeouts.WriteTotalTimeoutConstant = 0;
		if( !SetCommTimeouts(hComm, &timeouts) )
			return -1;
	}

	return TRUE;
}

int CTestApp::ExitInstance()
{
	Network::GetInstance().Uninit();

	//* 포트 닫기
	CloseHandle(hComm);

	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return TRUE;
}

BOOL CTestApp::OnIdle( LONG lCount )
{
	if( Network::GetInstance().ProcPacket() )
	{
		_mainFrm->GetMainDlg().RefreshControls();
	}

	if( Network::GetInstance()._isHost == false )
	{
		// 데이터 통신 (수신과 송신)
		unsigned char buf[31];   // 통신에 사용될 버퍼
		DWORD byteRead, byteWritten;
		int retval;

		int userNo = Network::GetInstance()._myUserNo;
		DataList & dataList = Network::GetInstance().GetDataList( userNo );

		int count = dataList.size();
		if( count >= 800 )
		{
			DataList::iterator it = dataList.begin();
			dataList.erase( it, it + 5 );
		}

		// 데이터를 먼저 받아야 하는 경우 수신부터 구현
		retval = ReadFile(hComm, buf, 30, &byteRead, NULL);
		if( retval )
		{
			//// 1초에 한번 체크
			//CTime t = CTime::GetCurrentTime();
			//int curSec = t.GetSecond();

			//_dataCnt += 5;

			//if( _lastSec != curSec )
			//{
			//	//
			//	_lastSec = curSec;

			//	//
			//	CString buf;
			//	buf.Format(L"DEBUG : %d \n", _dataCnt);
			//	OutputDebugString( (LPCWSTR)(buf) );

			//	//
			//	_dataCnt = 0;
			//}

			std::string str;
			int Temp_y=0;
			for( int num = 0; num < 30; num++ )
			{
				unsigned char ch = buf[ num ];
				str += ch;
				
				if( ch == ' ' )
				{
					int value = atoi(str.c_str());
					if( value > 0 )
					{
						int y = 300- value/3.4f - 50;
						dataList.push_back( PacketData(0,y) );
						str.clear();

						//
						int cnt = dataList.size();
						if( cnt > 2 )
						{
							int curValue = dataList[ cnt - 1 ]._y;
							int preValue = dataList[ cnt - 2 ]._y;

							//
							if( 130 <= preValue && 130 > curValue )
							{
								float hz = 1/(_overflowCnt / 110.f)*60;
								if( hz < 150 )
								{
									_hzList.push_back( hz );
									_overflowCnt = 0;

									//CString buf;
									//buf.Format(L"DEBUG : %f \n", hz);
									//OutputDebugString( (LPCWSTR)(buf) );
								}
							}

							//
							_overflowCnt++;
						}
					}
				}
			}
		}

		// 데이터 전송
		Network::GetInstance().ReqClientDataSend();

		// 1분에 한번 체크
		CTime t = CTime::GetCurrentTime();
		int curMin = t.GetMinute();

		// 갱신
		if( _lastMin != curMin )
		{
			//
			_lastMin = curMin;

			//
			float hz = 0.f;

			//
			int cnt = _hzList.size();
			for( int num = 0; num < cnt; num++ )
			{
				hz += _hzList[ num ];
			}

			float avHz;
			if( cnt > 0 )
			{
				avHz = hz / cnt;
			}
			else
			{
				avHz = 0;
			}

			_overflowCnt = 0;
			_hzList.clear();

			// 주기 데이터 전송(1분에 한번씩)
			SendAddUserData( avHz, 36.5 );

			//
			int userNo = Network::GetInstance()._myUserNo;
			Network::GetInstance().ReqGetUserDataSend( userNo );
		}
	}

	_mainFrm->GetMainDlg().GetView()->Refresh();

	return TRUE;
}

void CTestApp::SendAddUserData( int value, float temp )
{
	CTime t = CTime::GetCurrentTime();

	UserData userData; char buf[1024];
	userData._year = itoa( t.GetYear(), buf, 10 );

	if( t.GetMonth() < 10 )
	{
		userData._month = "0";
		userData._month += itoa( t.GetMonth(), buf, 10 );
	}
	else
	{
		userData._month = itoa( t.GetMonth(), buf, 10 );
	}

	if( t.GetDay() < 10 )
	{
		userData._day = "0";
		userData._day += itoa( t.GetDay(), buf, 10 );
	}
	else
	{
		userData._day = itoa( t.GetDay(), buf, 10 );
	}

	if( t.GetHour() < 10 )
	{
		userData._hour = "0";
		userData._hour += itoa( t.GetHour(), buf, 10 );
	}
	else
	{
		userData._hour = itoa( t.GetHour(), buf, 10 );
	}

	if( t.GetMinute() < 10 )
	{
		userData._min = "0";
		userData._min += itoa( t.GetMinute(), buf, 10 );
	}
	else
	{
		userData._min = itoa( t.GetMinute(), buf, 10 );
	}

	userData._value = itoa( value, buf, 10 );
	userData._temp = gcvt( temp, 10, buf );

	Network::GetInstance().ReqAddUserDataSend( userData );
}