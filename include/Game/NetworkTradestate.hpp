#ifndef NETWORKTRADESTATE_HPP
#define NETWORKTRADESTATE_HPP

#include "Gamestate.hpp"
#include "Network/Network.hpp"
#include "Network/RemotePlayer.hpp"
#include "Menu/Menu.hpp"

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
	int localSelection, peerSelection;
	bool selMade, peerSelMade;

	MenuImage background, window;
	MenuImage localPeoplemon, peerPeoplemon;
	MenuText localName, localLevel, localHp, localAtk, localDef, localSpAtk, localSpDef, localSpd, localAbility, localItem, localMoves[4];
	MenuText peerName, peerLevel, peerHp, peerAtk, peerDef, peerSpAtk, peerSpDef, peerSpd, peerAbility, peerItem, peerMoves[4];
	ChoiceBox choice;

	/**
	  * Updates all of the UI elements for the local peoplemon
	  *
	  * \param p The Peoplemon to update for
	  */
	void updateLocal(PeoplemonRef p);

	/**
	  * Updates all of the UI elements for the peer peoplemon
	  *
	  * \param p The Peoplemon to update for
	  */
	void updatePeer(PeoplemonRef p);

	/**
	 * Receives packets from the Network and handles them
	 */
	void updateNetwork();

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
