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
	 * This function packs objects to be sent over the network. Any object that is sent must have a specialization
	 * of this function defined for it. If it is called on an object with no specialization then a disconnect signal
	 * is returned instead
	 *
	 * \param obj The object to pack
	 * \return A packet containing the packed object to send
	 */
	template<typename T>
	sf::Packet pack(T obj)
	{
		std::cout << "WARNING: pack() called on object with no specialization defined!\n";
		sf::Packet p;
		p << sf::Uint16(2);
		return p;
	}

	template<>
	sf::Packet pack(PeoplemonRef& obj);

	template<>
	sf::Packet pack(Turn& obj);

	template<>
	sf::Packet pack(Player& obj);

	/**
	 * This function unpacks objects received over the network. Any object that is unpacked must have a specialization
	 * of this function defined for it. If it is called on an object with no specialization then this function returns
	 * false
	 *
	 * \param obj A reference to the object to unpack
	 * \return True if the object was successfully unpacked, false otherwise
	 */
	template<typename T>
	bool unpack(DataPacket& dp, T& obj)
	{
		std::cout << "WARNING: unpack() called on object with no specialization defined!\n";
		return false;
	}

	template<>
	bool unpack(DataPacket& dp, PlayerInfo& obj);

	template<>
	bool unpack(DataPacket& dp, PeoplemonRef& obj);

	template<>
	bool unpack(DataPacket& dp, Turn& obj);
}

#endif // PACKING_HPP
