#pragma once

enum MessageType
{
	S2CH_SUCCESS_CONNECTED = 200,
	CH2S_LOGIN,
	S2CH_LOGIN_RES,
	H2S_GET_USERINFO_LIST,
	S2H_GET_USERINFO_LIST_RES,
	CH2S_GET_USERDATA_LIST,
	S2CH_GET_USERDATA_LIST_RES,
	C2S_CLIENT_DATA,
	S2H_CLIENT_DATA_RES,
	C2S_ADD_USERDATA,
};