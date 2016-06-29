#ifndef DATAPACKET_HPP
#define DATAPACKET_HPP

#include "SFML.hpp"

/**
 * This is a wrapper class for sf::Packet. It interprets the type code in the packet so that application code doesn't have to
 *
 * \ingroup Network
 */
class DataPacket
{
public:
	/**
	 * All the possible types of packets that can be sent
	 */
	enum Type
	{
        PlayerInfo = 4,
        Peoplemon = 5
	};

private:
	Type type;
	sf::Packet data;

public:
	/**
	 * Interprets the type code and stores the data
	 *
	 * \param p The packet to handle
	 */
	DataPacket(sf::Packet p);

	/**
	 * Returns the type of data this packet contains
	 */
	Type getType();

	/**
	 * Returns a reference to the data itself
	 */
	sf::Packet& getData();
};

#endif // DATAPACKET_HPP
