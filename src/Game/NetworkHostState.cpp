#include "Game/NetworkHostState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

NetworkHostState::NetworkHostState(Game* g, Network& n, RemotePlayer p) : Gamestate(g), network(n), peer(p)
{
    background.setImage("hostBgnd.png");
    prompt.setText("Choose a mode");
    prompt.setProps(Color::Cyan,36);
    //continue
}

bool NetworkHostState::execute()
{
	return false;
}
