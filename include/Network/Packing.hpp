#ifndef PACKING_HPP
#define PACKING_HPP

#include "SFML.hpp"
#include "DataPacket.hpp"
#include <iostream>

class Player;
class Turn;
class PeoplemonRef;

/**
 * Namespace for Network helpers to try and keep the global namespace cleaner
 *
 * \ingroup Network
 */
namespace Packing
{
	/**
	 * Helper struct to contain player information that gets sent over the network
	 *
	 * \ingroup Network
	 * \ingroup Entities
	 */
	struct PlayerInfo
	{
		std::string name;
		int money, gender; //1 = boy, 2 = girl
		std::vector<int> items;
	};

	/**
	 * This function packs a PeoplemonRef to be sent over the network
	 *
	 * \param obj The PeoplemonRef to pack
	 * \return A packet containing the packed object to send
	 */
	sf::Packet pack(PeoplemonRef& obj);

	/**
	 * This function packs a Turn to be sent over the network
	 *
	 * \param obj The Turn to pack
	 * \return A packet containing the packed object to send
	 */
	sf::Packet pack(Turn& obj);

	/**
	 * This function packs the Player's information to be sent over the network
	 *
	 * \param obj The Player to pack
	 * \return A packet containing the packed object to send
	 */
	sf::Packet pack(Player& obj);

	/**
	 * This function unpacks a PlayerInfo received over the network
	 *
	 * \param obj A reference to the PlayerInfo to unpack
	 * \return True if the object was successfully unpacked, false otherwise
	 */
	bool unpack(DataPacket& dp, PlayerInfo& obj);

	/**
	 * This function unpacks a PeoplemonRef received over the network
	 *
	 * \param obj A reference to the PeoplemonRef to unpack
	 * \return True if the object was successfully unpacked, false otherwise
	 */
	bool unpack(DataPacket& dp, PeoplemonRef& obj);

	/**
	 * This function unpacks a Turn received over the network
	 *
	 * \param obj A reference to the Turn to unpack
	 * \return True if the object was successfully unpacked, false otherwise
	 */
	bool unpack(DataPacket& dp, Turn& obj);
}

#endif // PACKING_HPP
