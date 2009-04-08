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
	// 스크린세이버 모드 실행
	//::SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,TRUE,0,0);
}

void ScreenSaver::Uninit()
{
}

void ScreenSaver::StartScreenSaver( HWND hwnd )
{
	SendMessage(hwnd,WM_SYSCOMMAND, SC_SCREENSAVE,NULL);
	//SendMessage(HWND_BROADCAST,WM_SYSCOMMAND, SC_SCREENSAVE,NULL);
}

void ScreenSaver::KillScreenSaver()
{
	HDESK hDesktop = OpenDesktop( TEXT("screen-saver"), 0, FALSE, DESKTOP_READOBJECTS | DESKTOP_WRITEOBJECTS ); 

	if( hDesktop != 0 )
	{
		EnumDesktopWindows( hDesktop, (WNDENUMPROC)KillScreenSaverFunc , 0 );
		CloseDesktop( hDesktop );
	}
	//else
	//{
	//	PostMessage( GetForegroundWindow(), WM_CLOSE, 0, 0 );
	//}
}

BOOL CALLBACK KillScreenSaverFunc(HWND hWnd, LPARAM lParam)
{
	if( IsWindowVisible( hWnd ) )
	{
		::PostMessage( hWnd, WM_CLOSE, 0, 0 );
	}
	return true;
}