#include "Game/NetworkTradestate.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
using namespace sf;
using namespace std;

NetworkTradestate::NetworkTradestate(Game* g, Network& n, RemotePlayer rp, Network::Mode m) : Gamestate(g,NULL), network(n), peer(rp), mode(m)
{
	if (mode==Network::Host)
		background.setImage("hostBgnd.png");
	else
		background.setImage("clientBgnd.png");
	window.setImage("networkTradeBgnd.png");
	//TODO - all the text crap

    choice.addChoice("Pick Peoplemon");
    choice.addChoice("Trade");
    choice.addChoice("Cancel");
    choice.addChoice("Disconnect");
    choice.setPosition(Vector2f(32,32));
    choice.setTextProps(Color::Black,26);
}

bool NetworkTradestate::execute()
{
	while (!finishFrame())
	{
		choice.update();

		if (choice.getChoice()=="Pick Peoplemon")
		{
			choice.reset();
			//sleep
		}
		if (choice.getChoice()=="Trade")
		{
			//go into wait state
		}
		if (choice.getChoice()=="Cancel");
		{
			//go back and await confirmation?
		}
		if (choice.getChoice()=="Disconnect")
		{
			//indicate to above code to exit all the way
			return false;
		}

		game->mainWindow.clear();
		background.draw(&game->mainWindow);
		window.draw(&game->mainWindow);
		choice.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(30));
	}

	return true;
}
