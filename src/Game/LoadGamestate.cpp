#include "Game/LoadGamestate.hpp"
#include "Game/MainGamestate.hpp"
#include "Menu/Menu.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
#include "Menu/ChoiceBox.hpp"
#include "Properties.hpp"
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
    choices.setBackgroundColor(Color::White);
    choices.setBorder(Color::Black,3);
    choices.setTextProps(Color::Black,22);
    choices.setPosition(Vector2f(300,300));

    sleep(milliseconds(200));

    while (!finishFrame())
    {
        choices.update();

        if (choices.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
            return false;
        else if (choices.getChoice().size())
        {
            game->load(choices.getChoice());
            return game->runState(new MainGameState(game));
        }

        background.draw(&game->mainWindow);
        choices.draw(&game->mainWindow);
        game->mainWindow.display();
        sleep(milliseconds(30));
    }

    return true;
}
