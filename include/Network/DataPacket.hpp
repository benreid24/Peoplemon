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
		Empty = 0,
		Disconnect = 2,
		TransmissionComplete = 3,
		ActionChoice = 4,
		ActionConfirmation = 5,
        PlayerInfo = 6,
        Peoplemon = 7,
        Turn = 8,
        TradeChoice = 9,
        TradeReady = 10
	};

	/**
	 * If the DataPacket contains an ActionChoice then this enum will contain the choice itself
     */
	enum Choice
	{
		Trade = 1,
		Battle = 2
	}choice;

	/**
	 * If the DataPacket contains an ActionConfirmation then this enum will contain the confirmation itself
	 */
	enum Confirmation
	{
		No = 0,
		Yes = 1
	}confirmation;

private:
	Type type;
	sf::Packet data;

public:
	/**
	 * Creates an empty DataPacket
	 */
	DataPacket();

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

	/**
	 * Template function to easily read out specific data type from the packet
	 */
	template<typename T>
	T read()
	{
		T tmp;
		data >> tmp;
		return tmp;
	}
};

#endif // DATAPACKET_HPP
