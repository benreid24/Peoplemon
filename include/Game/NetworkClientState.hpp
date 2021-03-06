#ifndef NETWORKCLIENTSTATE_HPP
#define NETWORKCLIENTSTATE_HPP

#include "Gamestate.hpp"
#include "Network/Network.hpp"
#include "Network/RemotePlayer.hpp"
#include "Menu/Menu.hpp"

/**
 * This state provides the UI and functionality for the client peer
 *
 * \ingroup Network
 * \ingroup Game
 */
class NetworkClientState : public Gamestate
{
	Network& network;
	RemotePlayer peer;

	MenuImage background;
	MenuText prompt;

	const int Trade = 0, Battle = 1;

	/**
	 * UI for the client to confirm a choice by the host
	 *
	 * \param mode the Choice to confirm and the mode to enter if the user says yes
	 * \return True if the game should close, false otherwise
	 */
	bool confirmChoice(int mode);

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
	 * \param p Information of the remote peer
	 */
	NetworkClientState(Game* g, Network& n, RemotePlayer p);
};

#endif // NETWORKCONNECTINGSTATE_HPP


