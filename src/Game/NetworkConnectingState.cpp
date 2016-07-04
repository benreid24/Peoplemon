#include "Game/NetworkConnectingState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

NetworkConnectingState::NetworkConnectingState(Game* g) : Gamestate(g)
{
	end = false;
	background.setImage("connectingBgnd.png");
	prompt.setText("Would you like to host a game or connect to someone else?");
	prompt.setProps(Color::Cyan,36);
	prompt.setPosition(Vector2f(50,50));
	conType.setTextProps(Color::Cyan, 30);
	conType.setBackgroundColor(Color(220,30,30));
	conType.setBorder(Color(180,30,30),3);
	conType.addChoice("Host");
	conType.addChoice("Client");
	conType.addChoice("Back");
}

bool NetworkConnectingState::execute()
{
    while (finishFrame())
	{
		conType.update();

		if (conType.getChoice()=="Host")
		{
			//
		}
		if (conType.getChoice()=="Client")
		{
			//
		}
		if (conType.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
			return false;

		game->mainWindow.clear();
        //
	}

	return false;
}
