#pragma once

class ScreenSaver
{
public:
	ScreenSaver();
	~ScreenSaver();

	void Init();
	void Uninit();

	void StartScreenSaver( HWND hwnd );
	void KillScreenSaver();

	//
	static ScreenSaver & GetInstance() { return _instance; }

private:
	static ScreenSaver _instance;
};