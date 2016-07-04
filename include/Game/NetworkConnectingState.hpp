#ifndef NETWORKCONNECTINGSTATE_HPP
#define NETWORKCONNECTINGSTATE_HPP

#include "Gamestate.hpp"
#include "Menu/Menu.hpp"
#include "Menu/IpEnter.hpp"
#include "Menu/HostSelector.hpp"
#include "Network/RemotePlayer.hpp"

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
     * Sends all information over the network and then sends the transmission complete signal
     *
     * \param n The Network to transfer over
     */
	void transferData(Network& n);

	/**
	 * Receives all information over the network and stops when the transmission complete signal is received
	 *
	 * \param n The Network to receive from
	 * \return A RemotePlayer object containing information of the remote peer
	 */
	RemotePlayer receiveData(Network& n);

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
