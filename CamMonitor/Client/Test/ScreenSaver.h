#pragma once

class ScreenSaver
{
public:
	ScreenSaver();
	~ScreenSaver();

	void Init();
	void Uninit();

	void StartScreenSaver();
	void KillScreenSaver();
	BOOL IsScreenSaverRunning();

	//
	static ScreenSaver & GetInstance() { return _instance; }

private:
	void ReadRegVal(char *reg_path, char *key, char *val, int val_buf_size );
	bool WriteRegVal(char *reg_path, char *key, char *val);

private:
	static ScreenSaver _instance;
};