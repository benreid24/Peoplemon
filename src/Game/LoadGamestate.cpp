#include "Game/LoadGamestate.hpp"
#include "Game/MainGameState.hpp"
#include "Menu/Menu.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
#include "Menu/ChoiceBox.hpp"
#include "Properties.hpp"
#include <cstdlib>
using namespace std;
using namespace sf;

LoadGamestate::LoadGamestate(Game* g, Gamestate* n) : Gamestate(g,n)
{
    //ctor
}

bool LoadGamestate::execute()
{
    vector<string> saves = getDirectory(Properties::GameSavePath, "sav");

    MenuImage background;
    ChoiceBox choices;
    for (unsigned int i = 0; i<saves.size(); ++i)
    {
        choices.addChoice(File::getBaseName(saves[i]));
    }
    choices.addChoice("Back");

    background.setImage("loadGameBgnd.png");
    choices.setBackgroundColor(Color(130,130,130));
    choices.setBorder(Color::Black,4);
    choices.setTextProps(Color::Black,30);
    choices.setPosition(Vector2f(200,280));

    sleep(milliseconds(200));

    while (!finishFrame())
    {
        choices.update();

        if (choices.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
            return false;
        else if (choices.getChoice().size())
        {
        	sleep(milliseconds(350));
        	ChoiceBox action;
        	action.addChoice("Load");
        	action.addChoice("Delete");
        	action.addChoice("Back");
        	action.setBackgroundColor(Color(130,130,130));
        	action.setBorder(Color::Black,4);
        	action.setTextProps(Color::Black,30);
        	unsigned int h = 0;
        	for (; h<saves.size(); ++h)
			{
				if (File::getBaseName(saves[h])==choices.getChoice())
					break;
			}
			action.setPosition(Vector2f(200+choices.getSize().x+50,280+30*h));

			while (true)
			{
				if (finishFrame())
					return true;

				action.update();
				if (action.getChoice()=="Load")
				{
					int a = 0;
					RectangleShape cover(Vector2f(Properties::ScreenWidth,Properties::ScreenHeight));
					cover.setFillColor(Color::Transparent);
					while (a!=255)
					{
						cover.setFillColor(Color(0,0,0,a));
						a += 2;
						if (a>255)
							a = 255;

						game->mainWindow.draw(cover);
						game->mainWindow.display();
						sleep(milliseconds(15));
					}
					game->load(choices.getChoice());
					return game->runState(new MainGameState(game));
				}
				else if (action.getChoice()=="Delete")
				{
					remove(saves[h].c_str());
					cout << "Deleting: " << saves[h].c_str() << endl;
					saves.erase(saves.begin()+h);
					choices.clear();
					for (unsigned int i = 0; i<saves.size(); ++i)
					{
						choices.addChoice(File::getBaseName(saves[i]));
					}
					choices.addChoice("Back");
					choices.setTextProps(Color::Black,30);
					break;
				}
				else if (action.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
					break;

				background.draw(&game->mainWindow);
				choices.draw(&game->mainWindow);
				action.draw(&game->mainWindow);
				game->mainWindow.display();
				sleep(milliseconds(30));
			}

			sleep(milliseconds(350));
			choices.reset();
        }

        background.draw(&game->mainWindow);
        choices.draw(&game->mainWindow);
        game->mainWindow.display();
        sleep(milliseconds(30));
    }

    return true;
}
