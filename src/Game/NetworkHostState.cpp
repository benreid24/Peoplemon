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
    gameType.setPosition(sf::Vector2f(130,80));
    gameType.setTextProps(Color::Cyan,30);
}

bool NetworkHostState::execute()
{
	while (!finishFrame())
	{
		gameType.update();

		if (gameType.getChoice()=="Trade")
		{
			cout << "Trade!\n";
			gameType.reset();
			sleep(milliseconds(250));
		}
		if (gameType.getChoice()=="Battle")
		{
			cout << "Battle\n";
			gameType.reset();
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
