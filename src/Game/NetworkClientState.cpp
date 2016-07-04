#include "Game/NetworkClientState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

NetworkClientState::NetworkClientState(Game* g, Network& n, RemotePlayer p) : Gamestate(g), network(n), peer(p)
{
	//
}

bool NetworkClientState::execute()
{
	return false;
}
