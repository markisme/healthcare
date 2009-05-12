#include "stdafx.h"
#include "ScreenSaver.h"

ScreenSaver ScreenSaver::_instance;

BOOL CALLBACK KillScreenSaverFunc(HWND hWnd, LPARAM lParam);

ScreenSaver::ScreenSaver()
{
}

ScreenSaver::~ScreenSaver()
{
}

void ScreenSaver::Init()
{
	// 문자 스크린 세이버 설정
	WriteRegVal( "Control Panel\\Desktop", "ScreenSaveActive", "1" );
	WriteRegVal( "Control Panel\\Desktop", "SCRNSAVE.EXE", "C:\\WINDOWS\\system32\\ssmarque.scr" );

	// 스크린 세이버 세부 설정
	WriteRegVal( "Control Panel\\Screen Saver.Marquee", "BackgroundColor", "192 192 192" );
	WriteRegVal( "Control Panel\\Screen Saver.Marquee", "Mode", "1" );
	WriteRegVal( "Control Panel\\Screen Saver.Marquee", "Size", "26" );
	WriteRegVal( "Control Panel\\Screen Saver.Marquee", "Speed", "2" );
	WriteRegVal( "Control Panel\\Screen Saver.Marquee", "Text", "도난 경보기 작동 중.." );
	WriteRegVal( "Control Panel\\Screen Saver.Marquee", "TextColor", "255 0 0" );
	
	// 스크린세이버 모드 실행으로 설정
	::SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,TRUE,0,0);
}

void ScreenSaver::Uninit()
{
}

void ScreenSaver::StartScreenSaver( HWND hwnd )
{
	hwnd = GetDesktopWindow();
	if( IsScreenSaverRunning() == FALSE )
	{
		SendMessage(hwnd,WM_SYSCOMMAND, SC_SCREENSAVE,NULL);
	}
}

BOOL ScreenSaver::IsScreenSaverRunning()
{
	HDESK hDesktop = OpenDesktop( TEXT("screen-saver"), 0, FALSE, DESKTOP_READOBJECTS | DESKTOP_WRITEOBJECTS ); 

	if( hDesktop != 0 )
	{
		CloseDesktop( hDesktop );
		return TRUE;
	}

	return FALSE;
}

void ScreenSaver::KillScreenSaver()
{
	HDESK hDesktop = OpenDesktop( TEXT("screen-saver"), 0, FALSE, DESKTOP_READOBJECTS | DESKTOP_WRITEOBJECTS ); 

	if( hDesktop != 0 )
	{
		EnumDesktopWindows( hDesktop, (WNDENUMPROC)KillScreenSaverFunc , 0 );
		CloseDesktop( hDesktop );
		return;
	}
	else
	{
		::PostMessage (::GetForegroundWindow(), WM_CLOSE, 0L, 0L);
	}
}

BOOL CALLBACK KillScreenSaverFunc(HWND hWnd, LPARAM lParam)
{
	if( IsWindowVisible( hWnd ) )
	{
		::PostMessage( hWnd, WM_CLOSE, 0, 0 );
	}
	return true;
}

void ScreenSaver::ReadRegVal(char *reg_path, char *key, char *val, int val_buf_size )
{
    HKEY phk=0;
    DWORD nEC = REG_BINARY;
    DWORD size = val_buf_size;
    
    memset(val,0,val_buf_size);
    
	// 레지스트리 값 읽기
    RegOpenKey(HKEY_CURRENT_USER,reg_path,&phk); 
    RegQueryValueEx(phk, key, NULL,&nEC, (LPBYTE)val, &size );
	
    RegCloseKey(phk);
}

bool ScreenSaver::WriteRegVal(char *reg_path, char *key, char *val)
{
    HKEY phk=0;
    long nRet;

	// 레지스트리 값 저장
    RegCreateKey(HKEY_CURRENT_USER, reg_path, &phk);
    nRet = RegSetValueEx(phk, key, 0, REG_SZ, (LPBYTE)val, strlen(val));

    RegCloseKey(phk);

	if(ERROR_SUCCESS == nRet)
	{
		return true;
	}

    return false;
}