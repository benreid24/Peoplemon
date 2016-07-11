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

	localLevel.setPosition(Vector2f(60,29));
	localPic.setPosition(Vector2f(70,41));
	localName.setPosition(Vector2f(69,193));
	localHp.setPosition(Vector2f(240,127));
	localAtk.setPosition(Vector2f(240,152));
	localDef.setPosition(Vector2f(315,127));
	localSpAtk.setPosition(Vector2f(240,177));
	localSpDef.setPosition(Vector2f(315,152));
	localSpd.setPosition(Vector2f(315,176));
	localAbility.setPosition(Vector2f(226,66));
	localItem.setPosition(Vector2f(226,32));
	localMoves[0].setPosition(Vector2f(404,50));
	localMoves[1].setPosition(Vector2f(404,90));
	localMoves[2].setPosition(Vector2f(404,130));
	localMoves[3].setPosition(Vector2f(404,170));

	localLevel.setProps(Color::Black,20);
	localName.setProps(Color::Black,20);
	localHp.setProps(Color::Black,20);
	localAtk.setProps(Color::Black,20);
	localDef.setProps(Color::Black,20);
	localSpAtk.setProps(Color::Black,20);
	localSpDef.setProps(Color::Black,20);
	localSpd.setProps(Color::Black,20);
	localAbility.setProps(Color::Black,20);
	localItem.setProps(Color::Black,20);
	localMoves[0].setProps(Color::Black,20);
	localMoves[1].setProps(Color::Black,20);
	localMoves[2].setProps(Color::Black,20);
	localMoves[3].setProps(Color::Black,20);

	menu.add(&localLevel);
	menu.add(&localPic);
	menu.add(&localName);
	menu.add(&localHp);
	menu.add(&localAtk);
	menu.add(&localDef);
	menu.add(&localSpAtk);
	menu.add(&localSpDef);
	menu.add(&localSpd);
	menu.add(&localAbility);
	menu.add(&localItem);
	menu.add(&localMoves[0]);
	menu.add(&localMoves[1]);
	menu.add(&localMoves[2]);
	menu.add(&localMoves[3]);

	peerLevel.setPosition(Vector2f(591,264));
	peerPic.setPosition(Vector2f(600,275));
	peerName.setPosition(Vector2f(598,429));
	peerHp.setPosition(Vector2f(440,336));
	peerAtk.setPosition(Vector2f(440,366));
	peerDef.setPosition(Vector2f(515,366));
	peerSpAtk.setPosition(Vector2f(440,416));
	peerSpDef.setPosition(Vector2f(515,392));
	peerSpd.setPosition(Vector2f(515,416));
	peerAbility.setPosition(Vector2f(426,300));
	peerItem.setPosition(Vector2f(426,270));
	peerMoves[0].setPosition(Vector2f(240,285));
	peerMoves[1].setPosition(Vector2f(240,325));
	peerMoves[2].setPosition(Vector2f(240,365));
	peerMoves[3].setPosition(Vector2f(240,405));

	peerLevel.setProps(Color::Black,20);
	peerName.setProps(Color::Black,20);
	peerHp.setProps(Color::Black,20);
	peerAtk.setProps(Color::Black,20);
	peerDef.setProps(Color::Black,20);
	peerSpAtk.setProps(Color::Black,20);
	peerSpDef.setProps(Color::Black,20);
	peerSpd.setProps(Color::Black,20);
	peerAbility.setProps(Color::Black,20);
	peerItem.setProps(Color::Black,20);
	peerMoves[0].setProps(Color::Black,20);
	peerMoves[1].setProps(Color::Black,20);
	peerMoves[2].setProps(Color::Black,20);
	peerMoves[3].setProps(Color::Black,20);

	menu.add(&peerLevel);
	menu.add(&peerPic);
	menu.add(&peerName);
	menu.add(&peerHp);
	menu.add(&peerAtk);
	menu.add(&peerDef);
	menu.add(&peerSpAtk);
	menu.add(&peerSpDef);
	menu.add(&peerSpd);
	menu.add(&peerAbility);
	menu.add(&peerItem);
	menu.add(&peerMoves[0]);
	menu.add(&peerMoves[1]);
	menu.add(&peerMoves[2]);
	menu.add(&peerMoves[3]);

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
            selMade = true;
            network.sendTradeIndex(localSelection);
            sleep(milliseconds(250));
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
				menu.draw(&game->mainWindow);
				game->mainWindow.display();
				sleep(milliseconds(30));
			}
			cout << "Show trade anim and swap peoplemon. Save game\n";
		}
		else if (choice.getChoice()=="Trade")
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
		menu.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(30));
	}

	return true;
}

void NetworkTradestate::updateLocal(PeoplemonRef p)
{
	map<int,Peoplemon>& m = game->peoplemonList;
	localLevel.setText(intToString(p.level));
	localPic.setImage(Properties::PeoplemonImagePath+intToString(p.id)+".png",false,Vector2f(120,120));
	localName.setText(p.name+"/"+m[p.id].name);
	localAbility.setText(Peoplemon::abilityTexts[m[p.id].specialAbilityId].first);
	if (p.holdItem!=0)
		localItem.setText(game->itemList[p.holdItem].name);
	else
		localItem.setText("No item");
	localHp.setText(intToString(p.stats.hp));
	localAtk.setText(intToString(p.stats.atk));
	localDef.setText(intToString(p.stats.def));
	localSpAtk.setText(intToString(p.stats.spAtk));
	localSpDef.setText(intToString(p.stats.spDef));
	localSpd.setText(intToString(p.stats.spd));
	for (int i = 0; i<4; ++i)
	{
		if (p.moves[i].id!=0)
			localMoves[i].setText(game->moveList[p.moves[i].id].name);
	}
}

void NetworkTradestate::updatePeer(PeoplemonRef p)
{
	map<int,Peoplemon>& m = game->peoplemonList;
	peerLevel.setText(intToString(p.level));
	peerPic.setImage(Properties::PeoplemonImagePath+intToString(p.id)+".png",false,Vector2f(120,120));
	peerName.setText(p.name+"/"+m[p.id].name);
	peerAbility.setText(Peoplemon::abilityTexts[m[p.id].specialAbilityId].first);
	if (p.holdItem!=0)
		peerItem.setText(game->itemList[p.holdItem].name);
	else
		peerItem.setText("No item");
	peerHp.setText(intToString(p.stats.hp));
	peerAtk.setText(intToString(p.stats.atk));
	peerDef.setText(intToString(p.stats.def));
	peerSpAtk.setText(intToString(p.stats.spAtk));
	peerSpDef.setText(intToString(p.stats.spDef));
	peerSpd.setText(intToString(p.stats.spd));
	for (int i = 0; i<4; ++i)
	{
		if (p.moves[i].id!=0)
			peerMoves[i].setText(game->moveList[p.moves[i].id].name);
	}
}

void NetworkTradestate::updateNetwork()
{
	DataPacket dp = network.pollPacket();
	switch (dp.getType())
	{
	case DataPacket::TradeChoice:
		peerSelection = dp.read<Uint8>();
		updatePeer(peer.peoplemon[peerSelection]);
		break;
	case DataPacket::TradeReady:
		peerSelMade = dp.read<Uint8>()==1;
		break;
	default:
		break;
	}
}
