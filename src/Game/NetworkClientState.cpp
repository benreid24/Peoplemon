#include "Game/NetworkClientState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

NetworkClientState::NetworkClientState(Game* g, Network& n) : Gamestate(g), network(n)
{
	//
}

bool NetworkClientState::execute()
{
	return false;
}
