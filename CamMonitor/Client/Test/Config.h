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

private:
	static Config _instance;
};
