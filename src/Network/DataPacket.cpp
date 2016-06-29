#include "Network/DataPacket.hpp"
using namespace sf;
using namespace std;

DataPacket::DataPacket(sf::Packet p)
{
	Uint16 code;
	p >> code;
	type = Type(code);
	data = p;
}

DataPacket::Type DataPacket::getType()
{
	return type;
}

Packet& DataPacket::getData()
{
	return data;
}
