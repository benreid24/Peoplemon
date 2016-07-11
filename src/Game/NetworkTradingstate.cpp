#include "Game/NetworkTradestate.hpp"
#include "Game/PeoplemonState.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
using namespace sf;
using namespace std;

NetworkTradestate::NetworkTradestate(Game* g, Network& n, RemotePlayer rp, Network::Mode m) : Gamestate(g,NULL), network(n), peer(rp), mode(m)
{
	selMade = false;
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
    choice.setPosition(Vector2f(20,310));
    choice.setTextProps(Color::Black,26);
}

bool NetworkTradestate::execute()
{
	while (!finishFrame())
	{
		choice.update();
		updateNetwork();

		if (choice.getChoice()=="Pick Peoplemon")
		{
			sleep(milliseconds(250));
			choice.reset();
			PeoplemonState* state = new PeoplemonState(game, true, game->player.getCurrentPeoplemon(), "Trade this one?");
			if (game->runState(state,false))
				return true;
            updateLocal(game->player.getCurrentPeoplemon()->at(state->getChosenIndex()));
            localSelection = state->getChosenIndex();
		}
		if (choice.getChoice()=="Trade" && selMade)
		{
			sleep(milliseconds(250));
			choice.reset();
			game->hud.displayMessage("Waiting for "+peer.info.name+" to choose who to trade... Press the Run button to cancel");
			game->hud.setAlwaysShow(true);
			while (!game->hud.allTextShown() || !peerSelMade)
			{
				updateNetwork();
				game->hud.update();
				if (finishFrame())
					return true;

				game->mainWindow.clear();
				background.draw(&game->mainWindow);
				window.draw(&game->mainWindow);
				choice.draw(&game->mainWindow);
				game->mainWindow.display();
				sleep(milliseconds(30));
			}
			cout << "Show trade anim and swap peoplemon. Save game\n";
		}
		else
		{
            choice.reset();
            sleep(milliseconds(250));
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

void NetworkTradestate::updateLocal(PeoplemonRef p)
{
	//do that
}

void NetworkTradestate::updatePeer(PeoplemonRef p)
{
	//do that
}

void NetworkTradestate::updateNetwork()
{
	DataPacket dp = network.pollPacket();
	switch (dp.getType())
	{
	case DataPacket::TradeChoice:
		peerSelection = dp.read<Uint8>();
		break;
	case DataPacket::TradeReady:
		peerSelMade = dp.read<Uint8>()==1;
		break;
	default:
		break;
	}
}
