#ifndef IPENTER_HPP
#define IPENTER_HPP

#include "SFML.hpp"
#include "Menu/MenuImage.hpp"
#include "Menu/MenuText.hpp"

/**
 * Menu class for getting an ip and port from the user to connect to
 *
 * \ingroup Menu
 */
class IpEnter
{
	MenuImage background, goButton, numberHighlight, buttonHighlight;
	MenuText instructions;
	MenuText digits[14]; //3x4 for ip + 5 for port

public:
	/**
	 * Creates the IpEnter object at the given position
	 *
	 * \param pos The position to render to
	 */
	IpEnter(sf::Vector2f pos);

	/**
	 * Updates the object
	 */
	void update();

	/**
	 * Tells whether or not the user has finished entering the address they want
	 *
	 * \return True if the user is done, false otherwise
	 */
	bool done();

	/**
	 * Returns a formatted ip address that can be put into the Network object
	 *
	 * \return An IpAddress object that can be connected to
	 */
	sf::IpAddress getIp();
};

#endif // IPENTER_HPP
