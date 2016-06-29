#include "Game/NetworkConnectingState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

NetworkConnectingState::NetworkConnectingState(Game* g) : Gamestate(g)
{
	//
}

bool NetworkConnectingState::execute()
{
	return false;
}
