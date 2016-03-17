#include "SFML.hpp"
#include "Resources/ResourcePool.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Menu/Menu.hpp"
#include "Game/MainMenuState.hpp"
#include "Globals.hpp"
#include <iostream>
#include <memory>

#ifdef Linux
#include <dirent.h>
#include <X11/Xlib.h>
#endif

#ifdef Windows
#include <direct.h>
#endif

#include <cstdlib>
using namespace sf;
using namespace std;

void scriptInserter(Game* g)
{
    sleep(milliseconds(2000));
    while (g->mainWindow.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
			string f;
			cout << "Script: ";
			cin.clear();
			cin.sync();
			getline(cin,f);
			g->scriptEnvironment.runScript(shared_ptr<Script>(new Script(f)));
        }
        sleep(milliseconds(100));
    }
}

int main(int argC, const char* argV[])
{
	#ifdef Windows
	cout << "Creating directory: " << string(string(getenv("APPDATA"))+"/Peoplemon") << endl;
	mkdir(string(string(getenv("APPDATA"))+"/Peoplemon").c_str());
	#endif // Windows
	//TODO - paths for saves on other systems
    #ifdef Linux
    if (!XInitThreads())
        return 1;
    #endif

	srand(time(0));
    Properties::PrimaryMenuFont.loadFromFile(Properties::FontPath+"Dream.ttf");
    Properties::SecondaryMenuFont.loadFromFile(Properties::FontPath+"Bondani.ttf");
    Properties::ConversationFont.loadFromFile(Properties::FontPath+"Acens.ttf");
    Properties::BattleFont.loadFromFile(Properties::FontPath+"Pokemon.ttf");

    Game *game = new Game();
    Thread debugRunner(&scriptInserter,game);
    debugRunner.launch();
    cout << "Maximum texture size on this machine is: " << Texture::getMaximumSize() << endl;
    Joystick::update();
    game->start();
    debugRunner.terminate();
    imagePool.clearAll();
    audioPool.clearAll();
    animPool.clearAll();
    game->mainWindow.close();
    delete game;

    return 0;
}
