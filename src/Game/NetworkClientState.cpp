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
			if (p.choice==DataPacket::Battle)
			{
				game->hud.getChoice(peer.info.name+" wants to battle! Is this OK?", vector<string>({"Yes","No"}),false);
				while (game->hud.getChoice().size()==0)
				{
					game->hud.update();
					if (finishFrame())
						return true;

					game->mainWindow.clear();
					background.draw(&game->mainWindow);
					prompt.draw(&game->mainWindow);
					game->hud.draw(&game->mainWindow);
					game->mainWindow.display();
					sleep(milliseconds(30));
				}

				if (game->hud.getChoice()=="Yes")
				{
					network.sendSignal(DataPacket::Yes);
					cout << "Battle!\n";
					return false;
				}
				else
					network.sendSignal(DataPacket::No);
			}
		}
		else if (p.getType()!=DataPacket::Empty)
			cout << "WARNING: Received errant packet when waiting for ActionChoice\n";

		game->mainWindow.clear();
		background.draw(&game->mainWindow);
		prompt.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(30));
	}

	return true;
}
