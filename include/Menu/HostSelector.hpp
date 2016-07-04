#ifndef HOSTSELECTOR_HPP
#define HOSTSELECTOR_HPP

#include <vector>
#include "SFML.hpp"
#include "Menu/MenuImage.hpp"
#include "Menu/MenuText.hpp"
#include "Network/Network.hpp"

/**
 * Menu item for detecting and updating the list of hosts on the local network and allowing the player to choose one
 *
 * \ingroup Network
 * \ingroup Menu
 */
class HostSelector
{
	Network* network;
	std::vector<std::pair<int,HostSettings> > hosts; //int is miss count. Miss gets reset if the host appears again in broadcast, host is removed from menu if misses exceeds 10
	MenuImage box, selBox;
	MenuText name, ip, port, title, tip;
	unsigned int curHost;
	int lastTime;
	bool finished;

	/**
	 * Updates the list of hosts and removes ones that have been gone for too long. Will also update the player's cursor to stay consistent as the hosts change
	 */
	void updateHosts();

public:
	/**
	 * Creates all UI elements
	 */
	HostSelector();

	/**
	 * Sets the internal network reference
	 *
	 * \param n The Network object to scan for hosts
	 */
	void setNetwork(Network* n);

	/**
	 * Updates the list of hosts and handles player input
	 */
	void update();

	/**
	 * Indicates if the player chose a host or not
	 *
	 * \return True if the player chose a host, false otherwise
	 */
	bool done();

	/**
	 * Resets the internal state so the player can choose another host
	 */
	void reset();

	/**
	 * Returns information for the selected host. This WILL crash if done() hasn't returned true
	 *
	 * \return A HostSettings object containing all the information needed to connect to the remote host
	 */
	HostSettings getSelectedHost();

	/**
	 * Draws the HostSelector to the screen
	 *
	 * \param window The window to render to
	 */
	void draw(sf::RenderWindow* window);
};

#endif // HOSTSELECTOR_HPP
