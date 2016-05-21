#include "Game/PausedState.hpp"
#include "Game/PeopledexState.hpp"
#include "Game/PeoplemonState.hpp"
#include "Game/BagState.hpp"
#include "Game/MapState.hpp"
#include "Game/OptionsMenuState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

PausedState::PausedState(Game* g, Gamestate* n) : Gamestate(g,n)
{
    g->world.lockAllPeople();
    gameClock.pause();
}

PausedState::~PausedState()
{
    game->world.unlockAllPeople();
    gameClock.unPause();
}

bool PausedState::execute()
{
    int lTime = gameClock.getTimeStamp()+500;
    ChoiceBox choices;

    choices.setTextProps(Color::Black,26);
    choices.setPosition(Vector2f(680,90));
    choices.addChoice("Resume");
    choices.addChoice("Peopledex");
    choices.addChoice("Peoplemon");
    choices.addChoice("Bag");
    choices.addChoice("Map");
    choices.addChoice("Save");
    choices.addChoice("Settings");
    choices.addChoice("Quit");
    choices.setBackgroundColor(Color::White);
    choices.setBorder(Color::Black,4);

    int fpsEnsure = gameClock.getTimeStamp();

    while (!finishFrame())
    {
        choices.update();
        game->world.update();

        if (choices.getChoice()=="Resume" || ((user.isInputActive(PlayerInput::Pause) || user.isInputActive(PlayerInput::Run)) && gameClock.getTimeStamp()>lTime) || !game->data.pauseGameFlag)
            return false;
        if (choices.getChoice()=="Peopledex")
        {
            if (game->runState(new PeopledexState(game)))
                return true;
            sleep(milliseconds(200));
            choices.reset();
        }
        if (choices.getChoice()=="Peoplemon")
        {
            choices.reset();
            if (game->runState(new PeoplemonState(game, false, game->player.getCurrentPeoplemon())))
                return true;
        }
        if (choices.getChoice()=="Bag")
        {
            choices.reset();
            if (game->runState(new BagState(game, false, false)))
                return true;
        }
        if (choices.getChoice()=="Map")
        {
            choices.reset();
            if (game->runState(new MapState(game,game->player.canFly())))
                return true;
        }
        if (choices.getChoice()=="Save")
        {
            choices.reset();
            game->save();
            MenuImage bgnd;
            bgnd.setImage("gamesave.png");
            game->hud.displayMessage(game->player.getName()+" saved the game!", "gamesave.wav");
            while (!game->hud.messageFinished())
            {
                game->hud.update();
                if (finishFrame())
                    return true;

                game->mainWindow.clear();
                bgnd.draw(&game->mainWindow);
                game->hud.draw(&game->mainWindow);
                game->mainWindow.display();

                sleep(milliseconds(30));
            }
            return false;
        }
        else if (choices.getChoice()=="Settings")
        {
            if (game->runState(new OptionsMenuState(game)))
                return true;
            sleep(milliseconds(200));
            choices.reset();
        }
        if (choices.getChoice()=="Quit")
            return true;

        game->mainWindow.clear();
        game->world.draw(&game->mainWindow);
        choices.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(16-(gameClock.getTimeStamp()-fpsEnsure)));
        fpsEnsure = gameClock.getTimeStamp();
    }

    return true;
}
