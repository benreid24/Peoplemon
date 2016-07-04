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
	MenuText prompt, clientPrompt;
    ChoiceBox conType, conMethod;
    IpEnter ipEnter;
    HostSelector hostSelector;
    bool end; //flag to indicate if functions should terminate after submenus are ran or not

    /**
     * Menu stuff for connecting to hosts on the local network
     *
     * \return True if the game should close, false otherwise
     */
	bool showHosts();

	/**
	 * Menu stuff for connecting directly to a host
	 *
	 * \return True if the game should close, false otherwise
	 */
	bool directConnect();

	/**
     * Menu stuff for waiting for a client to connect
     *
     * \return True if the game should close, false otherwise
     */
	bool waitClient();

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
