#include "Battle/PlayerBattler.hpp"
#include "Game/BagState.hpp"
#include "Game/PeoplemonState.hpp"
#include "Game/Game.hpp"
#include "Game/BattleState.hpp"
using namespace std;
using namespace sf;

PlayerBattler::PlayerBattler(BattleState* b, Player* p, bool cR) : Battler(p->getCurrentPeoplemon())
{
    battle = b;
    mustChoose = true;
    moveChoice.setPosition(Vector2f(30,505));
    moveChoice.addChoice("Temp");
    moveChoice.setBackgroundColor(Color::White);
    moveChoice.setBorder(Color::Transparent,1);
    moveChoice.setTextProps(Color::Black,22);
    moveChoice.setAllowedCols(2);

    typeChoice.setPosition(Vector2f(525,505));
    typeChoice.addChoice("Move");
    typeChoice.addChoice("Item");
    typeChoice.addChoice("Switch");
    if (cR)
        typeChoice.addChoice("Run");
    typeChoice.setAllowedCols(2);
    typeChoice.setBackgroundColor(Color::White);
    typeChoice.setBorder(Color::Transparent,1);
    typeChoice.setTextProps(Color::Black,36);
}

PlayerBattler::~PlayerBattler()
{
    //dtor
}

Turn PlayerBattler::getTurn(PeoplemonRef op, Game* g)
{
	sleep(milliseconds(250));
    typeChoice.reset();
    moveChoice.reset();
    moveChoice.clear();
    for (int i = 0; i<4; ++i)
    {
        if (peoplemon->at(curPeoplemon).moves[i].id!=0)
            moveChoice.addChoice(g->moveList[peoplemon->at(curPeoplemon).moves[i].id].name);
    }
    moveChoice.addChoice("Back");

    while (true)
    {
        //forgive me for breaking standard and not using the finishFrame method, but I didn't want to have to do any major refactoring
        Event evt;
        while (g->mainWindow.pollEvent(evt))
        {
            if (evt.type==Event::Closed)
            {
                g->data.gameClosedFlag = true;
                Turn t;
                return t;
            }
        }

        typeChoice.update();

        if (typeChoice.getChoice()=="Move")
        {
            typeChoice.reset();
            moveChoice.reset();
            sleep(milliseconds(225));

            while (moveChoice.getChoice().size()==0)
            {
                //forgive me for breaking standard and not using the finishFrame method, but I didn't want to have to do any major refactoring
                Event evt;
                while (g->mainWindow.pollEvent(evt))
                {
                    if (evt.type==Event::Closed)
                    {
                        g->data.gameClosedFlag = true;
                        Turn t;
                        return t;
                    }
                }
                moveChoice.update();

                battle->renderBase();
                typeChoice.draw(&g->mainWindow);
                moveChoice.draw(&g->mainWindow);
                g->mainWindow.display();
                sleep(milliseconds(30));
            }

            if (moveChoice.getChoice()=="Back")
                sleep(milliseconds(225));
            else
            {
                for (int i = 0; i<4; ++i)
                {
                    if (g->moveList[peoplemon->at(curPeoplemon).moves[i].id].name==moveChoice.getChoice())
                    {
                        Turn t;
                        t.type = Turn::Move;
                        t.id = peoplemon->at(curPeoplemon).moves[i].id;
                        return t;
                    }
                }
            }
        }
        if (typeChoice.getChoice()=="Item")
        {
            BagState* s = new BagState(g,true,true);
            g->runStateUnderPriveldged(s,false);
            if (s->chosenItem()!=0)
            {
                Turn t;
                t.type = Turn::Item;
                t.id = s->chosenItem();
                delete s;
                return t;
            }
            delete s;
            typeChoice.reset();
            sleep(milliseconds(225));
        }
        if (typeChoice.getChoice()=="Switch")
        {
            Turn t;
            t.type = Turn::Switch;
            mustChoose = false;
            t.id = getSwitchPeoplemon(op,g);
            mustChoose = true;
            if (t.id!=-1 || g->data.gameClosedFlag)
                return t;
            typeChoice.reset();
            sleep(milliseconds(225));
        }
        if (typeChoice.getChoice()=="Run")
        {
            Turn t;
            t.type = Turn::Run;
            return t;
        }

        battle->renderBase();
        typeChoice.draw(&g->mainWindow);
        g->mainWindow.display();
        sleep(milliseconds(40));
    }
}

int PlayerBattler::getSwitchPeoplemon(PeoplemonRef op, Game* g)
{
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        if (peoplemon->at(i).curHp>0)
            goto alive;
    }
    return -1;
    alive:

    PeoplemonState* s = new PeoplemonState(g,mustChoose,peoplemon,"Send out?");
    g->runStateUnderPriveldged(s,false);
    int r = s->getChosenIndex();
    delete s;
    return r;
}
