#include "Game/NewGamestate.hpp"
#include "Game/MainGameState.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include "Menu/ChoiceBox.hpp"
#include "Menu/ScreenKeyboard.hpp"
using namespace std;
using namespace sf;

NewGamestate::NewGamestate(Game* g) : Gamestate(g,NULL)
{
	g->music.stop();
    g->music.load("start.plst");
    g->music.play();
}

bool NewGamestate::execute()
{
    vector<string> messages({"Hello there!", "My name is Professor Prof", "You must be my new neighbor in our little town of HomeTown!",
                        "You're.... uh... I'm going to be completely honest, I have no idea what your name is!",
                        "Please, remind me","TEMP STRING TO BE OVERWRITTEN", "I'm also at least partially blind, maybe even fully blind. Are you a boy or a girl?",
                        "Oh... Oh I was not expecting that at all", "Well... uh... I should get going now", "See ya!"});
    int nameIndex = 4, sexIndex = 6, curIndex = 0;
    MenuImage background, professor;
    background.setImage("newGameBgnd.png");
    background.setOrigin(Vector2f(500,500));
    background.setPosition(Vector2f(400,300));
    professor.setImage("professor.png");
    int dir = 0;
    string playerName;

    game->hud.setAlwaysShow(true);
    game->hud.displayMessage(messages[curIndex]);

    while (!finishFrame())
    {
        game->hud.update();
        dir += 4;
        background.rotate(dir);
        if (game->hud.messageFinished())
        {
            if (curIndex==nameIndex)
            {
                sleep(milliseconds(350));
                ScreenKeyboard kBoard;
                kBoard.setInputLimit(25);

                while (!kBoard.isFinished())
                {
                    dir += 4;
                    background.rotate(dir);
                    kBoard.update();
                    if (finishFrame())
                        return true;

                    background.draw(&game->mainWindow);
                    professor.draw(&game->mainWindow);
                    game->hud.draw(&game->mainWindow);
                    kBoard.draw(&game->mainWindow);
                    game->mainWindow.display();
                    sleep(milliseconds(50));
                }
                playerName = kBoard.getText();
                messages[curIndex+1] = "Oh that's right! You're " + playerName + "!";
            }
            if (curIndex==sexIndex)
            {
                sleep(milliseconds(350));
                game->hud.getChoice(messages[curIndex], vector<string>({"Boy","Girl"}),false);

                while (!game->hud.getChoice().size())
                {
                    dir += 4;
                    background.rotate(dir);
                    game->hud.update();
                    if (finishFrame())
                        return true;

                    background.draw(&game->mainWindow);
                    professor.draw(&game->mainWindow);
                    game->hud.draw(&game->mainWindow);
                    game->mainWindow.display();
                    sleep(milliseconds(50));
                }
                bool isBoy = game->hud.getChoice()=="Boy";
                game->player.newGame(isBoy,playerName);
            }

            curIndex++;
            if (unsigned(curIndex)<messages.size())
                game->hud.displayMessage(messages[curIndex]);
            else
            {
                //TODO - init game to initial stuff
                game->hud.setAlwaysShow(false);
                game->world.load("roomDemo",1);
                gameClock.setClockTime(ClockTime(12,0));
                gameClock.unPause();
                return game->runState(new MainGameState(game));
            }
        }

        background.draw(&game->mainWindow);
        professor.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();
        sleep(milliseconds(30));
    }

    return true;
}
