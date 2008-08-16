#pragma once

struct PacketData
{
	PacketData( int x = 0, int y = 0 ) : _x( x ), _y( y ) {};
	~PacketData() {};

	int _x;
	int _y;
	int _temp;
};