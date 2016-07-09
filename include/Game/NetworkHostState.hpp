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

	const int Battle = 0;
	const int Trade = 1;

	/**
	 * Waits for the client confirmation and then enters the specified game mode
	 *
	 * \param mode The mode to enter if the client confirms
	 * \return True if the game should close, false otherwise
	 */
	bool waitConfirmation(int mode);

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

