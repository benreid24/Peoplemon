#include "Game/NetworkConnectingState.hpp"
#include "Game/NetworkClientState.hpp"
#include "Game/NetworkHostState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

NetworkConnectingState::NetworkConnectingState(Game* g) : Gamestate(g)
{
	end = false;
	background.setImage("connectingBgnd.png");
	prompt.setText("Would you like to host a game\n        or connect to someone else?");
	prompt.setProps(Color::Cyan,36);
	prompt.setPosition(Vector2f(50,50));
	clientPrompt.setText("How would you like to\nconnect?");
	clientPrompt.setProps(Color(170,20,20),36);
	clientPrompt.setPosition(Vector2f(400,300));
	conType.addChoice("Host");
	conType.addChoice("Client");
	conType.addChoice("Back");
	conType.setTextProps(Color::Cyan, 36);
	conType.setPosition(Vector2f(75,200));
	conMethod.addChoice("Scan LAN");
	conMethod.addChoice("Direct Connect");
	conMethod.addChoice("Back");
	conMethod.setTextProps(Color(170,20,20), 36);
	conMethod.setPosition(Vector2f(475,400));
}

bool NetworkConnectingState::execute()
{
    while (!finishFrame())
	{
		conType.update();

		if (conType.getChoice()=="Host")
		{
			sleep(milliseconds(250));
			if (waitClient())
				return true;
			if (end)
				return false;

			conType.reset();
			sleep(milliseconds(250));
		}
		if (conType.getChoice()=="Client")
		{
			sleep(milliseconds(250));
			conType.reset();

			while (true)
			{
				conMethod.update();
				if (finishFrame())
					return true;

				if (conMethod.getChoice()=="Scan LAN")
				{
					sleep(milliseconds(250));
					if (showHosts())
						return true;
					if (end)
						return false;

					conMethod.reset();
					sleep(milliseconds(250));
					break;
				}
				if (conMethod.getChoice()=="Direct Connect")
				{
					sleep(milliseconds(250));
					if (directConnect())
						return true;
					if (end)
						return false;

					conMethod.reset();
					sleep(milliseconds(250));
					break;
				}
				if (conMethod.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
				{
					sleep(milliseconds(250));
					conMethod.reset();
					break;
				}

				game->mainWindow.clear();
				background.draw(&game->mainWindow);
                conType.draw(&game->mainWindow);
                prompt.draw(&game->mainWindow);
                clientPrompt.draw(&game->mainWindow);
                conMethod.draw(&game->mainWindow);
                game->mainWindow.display();
                sleep(milliseconds(30));
			}
		}
		if (conType.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
			return false;

		game->mainWindow.clear();
        background.draw(&game->mainWindow);
        conType.draw(&game->mainWindow);
        prompt.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(30));
	}

	return true;
}

bool NetworkConnectingState::directConnect()
{
	ipEnter.reset();

	while (!finishFrame())
	{
		ipEnter.update();

        if (ipEnter.done())
		{
			cout << "Entered address: " << ipEnter.getIp().toString() << ":" << ipEnter.getPort() << endl;
			Network network(Network::Client);

			if (network.connect(ipEnter.getIp(),ipEnter.getPort()))
			{
				end = true;
				return game->runState(new NetworkClientState(game,network));
			}
			else
			{
				game->hud.displayMessage("Failed to connect!");
				while (!game->hud.messageFinished())
				{
					game->hud.update();
					if (finishFrame())
						return true;

					game->mainWindow.clear();
					background.draw(&game->mainWindow);
					ipEnter.draw(&game->mainWindow);
					game->hud.draw(&game->mainWindow);
					game->mainWindow.display();
					sleep(milliseconds(30));
				}
				ipEnter.reset();
				sleep(milliseconds(250));
			}

			return false;
		}

		game->mainWindow.clear();
		background.draw(&game->mainWindow);
		ipEnter.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(30));
	}

	return true;
}

bool NetworkConnectingState::showHosts()
{
	Network network(Network::Client);
	hostSelector.setNetwork(&network);
	hostSelector.reset();

	while (!finishFrame())
	{
		hostSelector.update();

		if (hostSelector.done())
		{
			game->hud.getChoice("Are you sure you want to connect to "+hostSelector.getSelectedHost().name+"?", vector<string>({"Yes","No"}),false);

			while (game->hud.getChoice().size()==0)
			{
				game->hud.update();
				if (finishFrame())
					return true;

				game->mainWindow.clear();
				background.draw(&game->mainWindow);
				hostSelector.draw(&game->mainWindow);
				game->hud.draw(&game->mainWindow);
				game->mainWindow.display();
				sleep(milliseconds(30));
			}

			if (game->hud.getChoice()=="Yes")
			{
				if (network.connect(hostSelector.getSelectedHost().ip,hostSelector.getSelectedHost().port))
				{
					end = true;
					return game->runState(new NetworkClientState(game,network),true);
				}
				else
				{
					game->hud.displayMessage("Failed to connect!");
					while (!game->hud.messageFinished())
					{
						game->hud.update();
						if (finishFrame())
							return true;

						game->mainWindow.clear();
						background.draw(&game->mainWindow);
						hostSelector.draw(&game->mainWindow);
						game->hud.draw(&game->mainWindow);
						game->mainWindow.display();
						sleep(milliseconds(30));
					}
					hostSelector.reset();
					sleep(milliseconds(250));
				}
			}
			else
			{
				hostSelector.reset();
				sleep(milliseconds(250));
			}
		}

		game->mainWindow.clear();
		background.draw(&game->mainWindow);
		hostSelector.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(130));
	}

	return true;
}

bool NetworkConnectingState::waitClient()
{
	Network network(Network::Host, game->player.getName());
	MenuText text;
	text.setText("Started server on port: " +intToString(network.getServerPort())+"\nThe local ip address is: "+network.getLocalIp().toString()+"\n\n    Press the run key to cancel");
	text.setPosition(Vector2f(20,20));

	while (!finishFrame())
	{
		if (network.checkClientConnected())
		{
			game->hud.displayMessage("Client connected! TODO: Change this to their name");
			while (!game->hud.messageFinished())
			{
				game->hud.update();
				if (finishFrame())
					return true;

				game->mainWindow.clear();
				background.draw(&game->mainWindow);
				game->hud.draw(&game->mainWindow);
				game->mainWindow.display();
				sleep(milliseconds(30));
			}

			end = true;
			return game->runState(new NetworkHostState(game,network),true);
		}
		if (user.isInputActive(PlayerInput::Run))
		{
			sleep(milliseconds(250));
			return false;
		}

		game->mainWindow.clear();
		background.draw(&game->mainWindow);
		text.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(30));
	}

	return true;
}
