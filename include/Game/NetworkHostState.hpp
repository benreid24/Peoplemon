#ifndef NETWORKHOSTSTATE_HPP
#define NETWORKHOSTSTATE_HPP

#include "Gamestate.hpp"
#include "Network/Network.hpp"
#include "Menu/Menu.hpp"
#include "Network/RemotePlayer.hpp"

/**
 * This state provides the UI and functionality for the host peer
 *
 * \ingroup Network
 * \ingroup Game
 */
class NetworkHostState : public Gamestate
{
	Network& network;
	RemotePlayer peer;

	MenuImage background;
	ChoiceBox gameType;
	MenuText prompt;

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
	 * \param p The information of the remote peer
	 */
	NetworkHostState(Game* g, Network& n, RemotePlayer p);
};

#endif // NETWORKCONNECTINGSTATE_HPP

