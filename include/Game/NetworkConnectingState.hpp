#ifndef NETWORKCONNECTINGSTATE_HPP
#define NETWORKCONNECTINGSTATE_HPP

#include "Gamestate.hpp"

/**
 * Gamestate for connecting to a peer. This provides the UI for connecting, creates the Network object and then invokes NetworkState
 *
 * \ingroup Network
 * \ingroup Game
 */
class NetworkConnectingState : public Gamestate
{
	/**
	 * Runs the state
	 */
	bool execute();

public:
	/**
	 * Creates UI elements and initializes all data
	 */
	NetworkConnectingState();
};

#endif // NETWORKCONNECTINGSTATE_HPP
