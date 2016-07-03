#ifndef PACKING_HPP
#define PACKING_HPP

#include "SFML.hpp"
#include "DataPacket.hpp"
#include <iostream>

/**
 * Namespace to try and keep the global namespace cleaner
 *
 * \ingroup Network
 */
namespace Packing
{
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
}

#endif // PACKING_HPP
