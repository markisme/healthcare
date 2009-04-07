#include "mmsystem.h"

class SoundMixer
{
public:
	SoundMixer();
	~SoundMixer();

	BOOL Init();
	void Uninit();

	void ViewFromCamera();

	BOOL SetMute(BOOL bMode);
	BOOL SetVolumn(INT nValue);

private:
	HMIXER m_hMixer;
	DWORD m_dwControlID;  
	DWORD m_dwLineID;
	DWORD m_dwChannels;
	DWORD m_VolumneID; 
	MMRESULT m_nStatus;
};