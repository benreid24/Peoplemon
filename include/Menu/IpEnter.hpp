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
	MenuImage background, numberHighlight, buttonHighlight;
	MenuText instructions;
	MenuText digits[17]; //3x4 for ip + 5 for port
	int address[17]; //ip and port
	int curSpot, lastTime;
	bool finished;

	const std::string numLookup[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	const int boxWidth = 45;

public:
	/**
	 * Creates the IpEnter menu object
	 */
	IpEnter();

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
	 * Resets the internal state so that the user may enter another address
	 */
	void reset();

	/**
	 * Returns a formatted ip address that can be put into the Network object
	 *
	 * \return An IpAddress object that can be connected to
	 */
	sf::IpAddress getIp();

	/**
	 * Returns the port that the user entered
	 *
	 * \return The port to connect to
	 */
	int getPort();

	/**
	 * Renders the IpEnter to the screen
	 *
	 * \param window A pointer to the window to render to
	 */
	void draw(sf::RenderWindow* window);
};

#endif // IPENTER_HPP
