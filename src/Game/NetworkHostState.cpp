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
    prompt.setPosition(Vector2f(100,50));
    gameType.addChoice("Trade");
    gameType.addChoice("Battle");
    gameType.addChoice("Disconnect");
    gameType.setPosition(sf::Vector2f(130,130));
    gameType.setTextProps(Color(0,240,240),30);
}

bool NetworkHostState::execute()
{
	while (!finishFrame())
	{
		gameType.update();

		if (gameType.getChoice()=="Trade")
		{
			gameType.reset();
            network.sendSignal(DataPacket::Trade);
            if (waitConfirmation(Trade))
				return true;
			sleep(milliseconds(250));
		}
		if (gameType.getChoice()=="Battle")
		{
			gameType.reset();
			network.sendSignal(DataPacket::Battle);
            if (waitConfirmation(Battle))
				return true;
			sleep(milliseconds(250));
		}

		game->mainWindow.clear();
		background.draw(&game->mainWindow);
		prompt.draw(&game->mainWindow);
		gameType.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(30));
	}
	return true;
}

bool NetworkHostState::waitConfirmation(int mode)
{
	game->hud.setAlwaysShow(true);
	game->hud.displayMessage("Waiting for "+peer.info.name+" to confirm...");

	while (!finishFrame())
	{
		game->hud.update();
		DataPacket dp = network.pollPacket();

		if (dp.getType()!=DataPacket::Empty)
		{
			if (dp.getType()==DataPacket::ActionConfirmation)
			{
				if (dp.confirmation==DataPacket::Yes)
				{
					game->hud.setAlwaysShow(false);
					cout << "YES!\n";
					return false;
				}
				else
				{
					game->hud.setAlwaysShow(false);
					cout << "NO!\n";
					return false;
				}
			}
			else
				cout << "WARNING: Errant packet received while waiting for confirmation!\n";
		}

		game->mainWindow.clear();
		background.draw(&game->mainWindow);
		prompt.draw(&game->mainWindow);
		gameType.draw(&game->mainWindow);
		game->hud.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(30));
	}

	return true;
}
