#include "Game/NicknameState.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
using namespace sf;
using namespace std;

NicknameState::NicknameState(Game* g, PeoplemonRef* ppl, Gamestate* n) : Gamestate(g,n)
{
	peoplemon = ppl;
	background.setImage("nicknameBgnd.png");
	pplPic.setImage(Properties::PeoplemonImagePath+intToString(ppl->id)+".png",false);
	keyboard.setInputLimit(32);

	oldHudPos = game->hud.getPosition();
	oldWrapWidth = game->hud.getWrapWidth();
	game->hud.displayMessage("Give "+ppl->name+" a nickname!");
}

NicknameState::~NicknameState()
{
    game->hud.rePosition(oldHudPos,oldWrapWidth);
    game->hud.displayMessage("");
}

bool NicknameState::execute()
{
	while (!finishFrame())
	{
		keyboard.update();
        game->hud.update();

		if (keyboard.isFinished())
		{
			game->hud.displayMessage("The "+peoplemon->name+" was named "+keyboard.getText()+"!");
			while (!game->hud.messageFinished())
			{
				game->hud.update();
				if (finishFrame())
					return true;

				game->mainWindow.clear();
				background.draw(&game->mainWindow);
				pplPic.draw(&game->mainWindow);
				keyboard.draw(&game->mainWindow);
				game->mainWindow.display();

				sleep(milliseconds(30));
			}
			peoplemon->name = keyboard.getText();
			return false;
		}

		game->mainWindow.clear();
		background.draw(&game->mainWindow);
		pplPic.draw(&game->mainWindow);
        keyboard.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(30));
	}
	return true;
}
