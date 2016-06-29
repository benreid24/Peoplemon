#ifndef NETWORKHOSTSTATE_HPP
#define NETWORKHOSTSTATE_HPP

#include "Gamestate.hpp"
#include "Network/Network.hpp"

/**
 * This state provides the UI and functionality for the host peer
 *
 * \ingroup Network
 * \ingroup Game
 */
class NetworkHostState : public Gamestate
{
	Network& network;

	/**
	 * Runs the state
	 */
	bool execute();

public:
	/**
	 * Initializes the UI
	 *
	 * \param n A reference to the connected Network object
	 */
	NetworkHostState(Network& n);
};

#endif // NETWORKCONNECTINGSTATE_HPP

