#ifndef NETWORKCLIENTSTATE_HPP
#define NETWORKCLIENTSTATE_HPP

#include "Gamestate.hpp"
#include "Network/Network.hpp"

/**
 * This state provides the UI and functionality for the client peer
 *
 * \ingroup Network
 * \ingroup Game
 */
class NetworkClientState : public Gamestate
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
	 * \param g A pointer to the main Game object
	 * \param n A reference to the connected Network object
	 */
	NetworkClientState(Game* g, Network& n);
};

#endif // NETWORKCONNECTINGSTATE_HPP


