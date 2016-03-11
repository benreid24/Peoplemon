#include "Game/MainMenuState.hpp"
#include "Game/MainGameState.hpp"
#include "Game/OptionsMenuState.hpp"
#include "Game/NewGamestate.hpp"
#include "Game/LoadGamestate.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "SFML.hpp"
using namespace std;
using namespace sf;

MainMenuState::MainMenuState(Game* g, Gamestate* n) : Gamestate(g,n), background("mainBGND.png")
{
    choices.addChoice("New Game");
    choices.addChoice("Load Game");
    choices.addChoice("Quit");
    choices.setPosition(Vector2f(200, 200));
    choices.setTextProps(Color::Black, 36);
    menu.add(&background);
    menu.add(&choices);
}

MainMenuState::~MainMenuState()
{
    //dtor
}

bool MainMenuState::execute()
{
    cout << "Main menu state\n";

    while (!finishFrame())
    {
        choices.update();

        if (choices.getChoice()=="New Game")
        {
            cout << "New Game\n";
            return game->runState(new NewGamestate(game));
        }
        else if (choices.getChoice()=="Load Game")
        {
            cout << "Load Game\n";
            if (game->runState(new LoadGamestate(game,NULL)))
                return true;
            sleep(milliseconds(200));
            choices.reset();
        }
        else if (choices.getChoice()=="Quit")
        {
            cout << "Quit\n";
            return true;
        }

        game->mainWindow.clear();
        menu.draw(&game->mainWindow);
        game->mainWindow.display();
        sleep(milliseconds(30));
    }

    return true;
}
