#ifndef NETWORKCONNECTINGSTATE_HPP
#define NETWORKCONNECTINGSTATE_HPP

#include "Gamestate.hpp"
#include "Menu/Menu.hpp"
#include "Menu/IpEnter.hpp"
#include "Menu/HostSelector.hpp"

/**
 * Gamestate for connecting to a peer. This provides the UI for connecting, creates the Network object and then invokes NetworkState
 *
 * \ingroup Network
 * \ingroup Game
 */
class NetworkConnectingState : public Gamestate
{
	MenuImage background;
    ChoiceBox conType;
    IpEnter ipEnter;
    HostSelector hostSelector;

	/**
	 * Runs the state
	 */
	bool execute();

public:
	/**
	 * Creates UI elements and initializes all data
	 *
	 * \param g A pointer to the main Game object
	 */
	NetworkConnectingState(Game* g);
};

#endif // NETWORKCONNECTINGSTATE_HPP
