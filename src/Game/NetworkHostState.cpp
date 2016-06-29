#include "Game/NetworkHostState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

NetworkHostState::NetworkHostState(Game* g, Network& n) : Gamestate(g), network(n)
{
	//
}

bool NetworkHostState::execute()
{
	return false;
}
