#include "Game/NetworkTradestate.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
using namespace sf;
using namespace std;

NetworkTradestate::NetworkTradestate(Game* g, Network& n, RemotePlayer rp, Network::Mode m) : Gamestate(g,NULL), network(n), peer(rp), mode(m)
{
	//ctor
}

bool NetworkTradestate::execute()
{
	return false;
}
