#include "Network/DataPacket.hpp"
using namespace sf;
using namespace std;

DataPacket::DataPacket()
{
	type = Empty;
}

DataPacket::DataPacket(sf::Packet p)
{
	Uint16 code;
	p >> code;
	type = Type(code);
	data = p;

	if (type==ActionChoice)
	{
		Uint8 t;
		data >> t;
		choice = Choice(t);
	}
	else if (type==ActionConfirmation)
	{
		Uint8 t;
		data >> t;
		confirmation = Confirmation(t);
	}
}

DataPacket::Type DataPacket::getType()
{
	return type;
}

Packet& DataPacket::getData()
{
	return data;
}
