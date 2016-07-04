#include "Game/NetworkHostState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

NetworkHostState::NetworkHostState(Game* g, Network& n, RemotePlayer p) : Gamestate(g), network(n), peer(p)
{
	//
}

bool NetworkHostState::execute()
{
	return false;
}
