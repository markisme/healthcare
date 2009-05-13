#pragma once

class Config
{
public:
	Config();
	~Config();

	//
	static Config & GetInstance() { return _instance; }

public:
	BOOL _isWebcamMode;
	BOOL _isACPowerMode;
	BOOL _isUSBMouseMode;
	BOOL _isScreenSaverMode;

private:
	static Config _instance;
};
