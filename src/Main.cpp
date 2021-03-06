#include "SFML.hpp"
#include "Resources/ResourcePool.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Menu/Menu.hpp"
#include "Game/MainMenuState.hpp"
#include "Globals.hpp"
#include <iostream>
#include <memory>
#include <cstdlib>

#ifdef Linux
#include <dirent.h>
#include <sys/stat.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

#ifdef Windows
#include <direct.h>
#endif

#ifdef OSX
#include <dirent.h>
#include <sys/stat.h>
#endif

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
			g->scriptEnvironment.runScript(new Script(f));
        }
        sleep(milliseconds(100));
    }
}

int main(int argC, const char* argV[])
{
	#ifndef DEBUG
   // cout.setstate(ios_base::failbit);
    //cerr.setstate(ios_base::failbit);
    #endif // DEBUG

	#ifdef Windows
	cout << "Creating directory: " << string(string(getenv("APPDATA"))+"/Peoplemon") << endl;
	mkdir(string(string(getenv("APPDATA"))+"/Peoplemon").c_str());
	#endif // Windows

    #ifdef OSX
    cout << "Creating directory: " << "/Users/"+string(getenv("USER"))+"/Library/Application Support/";
    mkdir(string("/Users/"+string(getenv("USER"))+"/Library/Application Support/Peoplemon").c_str(),ACCESSPERMS);
    #endif

    #ifdef Linux
    if (!XInitThreads())
        return 1;
    struct passwd *pw = getpwuid(getuid());
    string homedir = pw->pw_dir;
    cout << "Creating directory: "+homedir+"/.config/Peoplemon\n";
    cout << Properties::GameSavePath << endl;
    umask(777);
    mkdir(string(homedir+"/.config").c_str(),ACCESSPERMS);
    mkdir(string(homedir+"/.config/Peoplemon").c_str(),ACCESSPERMS);
    system(string("chmod 777 "+string(homedir+"/.config/Peoplemon")).c_str());
    #endif

	srand(time(NULL));
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
