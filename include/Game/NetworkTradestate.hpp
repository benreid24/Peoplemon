#ifndef NETWORKTRADESTATE_HPP
#define NETWORKTRADESTATE_HPP

#include "Gamestate.hpp"
#include "Network/Network.hpp"
#include "Network/RemotePlayer.hpp"

/**
 * Gamestate for trading Peoplemon over the network. Used by both clients and hosts
 *
 * \ingroup Game
 * \ingroup Network
 */
class NetworkTradestate : public Gamestate
{
	Network& network;
	RemotePlayer peer;
	Network::Mode mode;

	/**
	 * Runs the state
	 */
	bool execute();

public:
	/**
	 * Constructs the state
	 *
	 * \param g A pointer to the main Game object
	 * \param n A reference to the Network object
	 * \param rp The RemotePlayer
	 * \param m The mode to run in (client or host)
	 */
	NetworkTradestate(Game* g, Network& n, RemotePlayer rp, Network::Mode m);
};

#endif // NETWORKTRADESTATE_HPP
