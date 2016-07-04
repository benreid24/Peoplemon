#include "Game/NetworkClientState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

NetworkClientState::NetworkClientState(Game* g, Network& n, RemotePlayer p) : Gamestate(g), network(n), peer(p)
{
	background.setImage("clientBgnd.png");
	prompt.setText("Waiting for host to decide what to do\nPress the run button to disconnect if things\nget weird");
	prompt.setProps(Color(160,10,10),36);
	prompt.setPosition(Vector2f(100,220));
}

bool NetworkClientState::execute()
{
	while (!finishFrame())
	{
		DataPacket p = network.pollPacket();
		if (p.getType()==DataPacket::ActionChoice)
		{
			//confirm
			cout << "Received choice!\n";
		}
		else if (p.getType()!=DataPacket::Empty)
			cout << "WARNING: Received errant packet when waiting for ActionChoice\n";

		game->mainWindow.clear();
		background.draw(&game->mainWindow);
		prompt.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(100));
	}

	return true;
}
