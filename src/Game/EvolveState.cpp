#include "Game/EvolveState.hpp"
#include "Properties.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
using namespace sf;
using namespace std;

EvolveState::EvolveState(Game* g, PeoplemonRef* p, int nextId) : Gamestate(g,nullptr)
{
    ppl = p;
    evolved = false;
    nextPeoplemonId = nextId;
    if (nextId==-1)
        nextPeoplemonId = game->peoplemonList[ppl->id].evolveNewId;

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
    ppl->id = nextPeoplemonId;
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

int EvolveState::getEvolveId(Game* game, PeoplemonRef ppl) {
    switch (ppl.id) {
    case 82:
        if (ppl.level>=25 && game->world.getWeather().isThunder())
            return 83;
        return -1;

    case 43:
        if (ppl.level>=30) {
            if (gameClock.getClockTime().hour>=14 && gameClock.getClockTime().hour<16)
                return 44;
            else if (gameClock.getClockTime().hour>=3 && gameClock.getClockTime().hour<14)
                return 46;
            else
                return 45;
        }
        return -1;

    case 30:
        if (ppl.level>=30) {
            if (gameClock.getClockTime().hour>=20 || gameClock.getClockTime().hour<8)
                return 47;
            else
                return 107;
        }
        return -1;

    case 104:
        if (ppl.level>=30 && game->world.getWeather().isRain())
            return 106;
        return -1;

    case 70:
        if (ppl.level>=30 && game->world.getWeather().isSandstorm())
            return 71;
        return -1;

    case 48:
        if (ppl.level>=32 && game->world.getWeather().isFoggy())
            return 49;
        return -1;

    case 27:
        if (ppl.level>=32) {
            if (game->world.getWeather().isSunny())
                return 29;
            return 28;
        }
        return -1;

    case 67:
        if (ppl.level>=32 && game->world.getWeather().isSnowy())
            return 68;
        return -1;

    case 99:
        if (ppl.level>=35) {
            if (game->player.hasItem(62)>=3) {
                game->player.takeItem(62, 3);
                return 100;
            }
        }
        return -1;

    case 88:
        if (ppl.level>=35 && game->world.getWeather().isSunny())
            return 89;
        return -1

    default:
        if (ppl.level >= game->peoplemonList[ppl.id].evolveLevel)
            return game->peoplemonList[ppl.id].evolveNewId;
        return -1;
    }
}
