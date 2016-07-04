#include "Game/EvolveState.hpp"
#include "Properties.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
using namespace sf;
using namespace std;

EvolveState::EvolveState(Game* g, PeoplemonRef* p) : Gamestate(g,NULL)
{
    ppl = p;
    evolved = false;

    evolveText.setPosition(Vector2f(90,500));
    evolveText.setText("What the poop? "+p->name+" is evolving!!!\nPress the pause button to cancel");
    bgndTxtr = imagePool.loadResource(Properties::BattleImagePath+"evolveBgnd.png");
    oldTxtr = imagePool.loadResource(Properties::PeoplemonImagePath+intToString(game->peoplemonList[p->id].id)+".png");
    newTxtr = imagePool.loadResource(Properties::PeoplemonImagePath+intToString(game->peoplemonList[p->id].evolveNewId)+".png");

    sparkles.setSource(animPool.loadResource(Properties::MiscAnimationPath+"sparkles.anim"));
    sparkles.setPosition(Vector2f(800,600));
    bgnd.setTexture(*bgndTxtr);
    oldPpl.setTexture(*oldTxtr);
    newPpl.setTexture(*newTxtr);
    oldPpl.setColor(Color::White);
    newPpl.setColor(Color::White);

    oldPpl.setOrigin(oldTxtr->getSize().x/2,oldTxtr->getSize().y/2);
    newPpl.setOrigin(newTxtr->getSize().x/2,newTxtr->getSize().y/2);
    oldPpl.setPosition(Properties::ScreenWidth/2,Properties::ScreenHeight/2);
    newPpl.setPosition(Properties::ScreenWidth/2,Properties::ScreenHeight/2);
}

bool EvolveState::execute()
{
    int scale = 100, slp = 5;
    int zeroFrames = 0;
    bool increasing = false;

    sparkles.play();
    while (!sparkles.finished())
    {
        if (finishFrame())
            return true;

        if (increasing)
        {
            scale++;
            if (scale>=100)
            {
                increasing = false;
                slp--;
            }
        }
        else
        {
            scale--;
            if (scale<=0)
            {
                increasing = true;
                if (slp<=0)
                {
                    zeroFrames++;
                    if (zeroFrames>=7)
                        break;
                }
            }
        }
        if (user.isInputActive(PlayerInput::Pause))
        {
            evolveText.setText("The evolution was canceled!");
            goto no;
        }
        oldPpl.setScale(float(scale)/100,float(scale)/100);
        newPpl.setScale(float(100-scale)/100,float(100-scale)/100);
        sparkles.update();

        game->mainWindow.clear();
        game->mainWindow.draw(bgnd);
        evolveText.draw(&game->mainWindow);
        sparkles.draw(&game->mainWindow);
        game->mainWindow.draw(oldPpl);
        game->mainWindow.draw(newPpl);
        game->mainWindow.display();

        sleep(milliseconds(slp));
    }

    evolved = true;
    ppl->id = game->peoplemonList[ppl->id].evolveNewId;
    evolveText.setText(ppl->name+" evolved!");
    no:

    while (!user.isInputActive(PlayerInput::Interact))
    {
        if (finishFrame())
            return true;

		game->mainWindow.clear();
        game->mainWindow.draw(bgnd);
        game->mainWindow.draw(newPpl);
        evolveText.draw(&game->mainWindow);
        game->mainWindow.display();
        sleep(milliseconds(30));
    }
    sleep(milliseconds(225));

    return false;
}

bool EvolveState::evolutionSuccessful()
{
    return evolved;
}
