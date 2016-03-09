#include "Game/TrainerSpottedPlayerState.hpp"
#include "Battle/AI/RandomBattler.hpp"
#include "Game/Game.hpp"
#include "Game/ConversationState.hpp"
#include "Game/BattleState.hpp"
#include "World/People/Trainer.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
using namespace std;
using namespace sf;

TrainerSpottedPlayerState::TrainerSpottedPlayerState(Game* g, Trainer* t, BattleState* b, ConversationState* before, ConversationState* after, Gamestate* n) : Gamestate(g,n)
{
    battle = b;
    trainer = t;
    preConv = before;
    postConv = after;
    t->setLock(true,true);
    g->player.setLock(true,true);
    g->player.forceStop();
}

TrainerSpottedPlayerState::~TrainerSpottedPlayerState()
{
    trainer->resetLock();
    game->player.resetLock();
}

bool TrainerSpottedPlayerState::execute()
{
    int nTDir = 0;
    int nPDir = 0;
    if (trainer->getMapPos().x!=game->player.getMapPos().x)
    {
        if (trainer->getMapPos().x>game->player.getMapPos().x)
            nTDir = 3;
        else
            nTDir = 1;
    }
    else
    {
        if (trainer->getMapPos().y>game->player.getMapPos().y)
            nTDir = 0;
        else
            nTDir = 2;
    }
    nPDir = nTDir+2;
    if (nPDir>3)
        nPDir -= 4;

    int ensureFps = gameClock.getTimeStamp();
    int rTime = ensureFps;
    while (trainer->getMapPos().x-game->player.getMapPos().x+trainer->getMapPos().y-game->player.getMapPos().y>1)
    {
        ensureFps = gameClock.getTimeStamp();
        cout << trainer->move(game,nTDir) << endl;

        if (finishFrame())
            return true;

        game->world.update();
        game->hud.update();

		if (gameClock.getTimeStamp()-rTime>=16)
        {
        	game->mainWindow.clear();
			game->world.draw(&game->mainWindow);
			game->hud.draw(&game->mainWindow);
			game->mainWindow.display();
			rTime = gameClock.getTimeStamp();
        }

        sleep(milliseconds(8-(gameClock.getTimeStamp()-ensureFps)));
    }

    if (game->runState(preConv,false))
        return true;
    if (game->runState(battle,false))
        return true;
    if (!game->data.gameClosedFlag && battle->playerWon())
    {
        if (game->runState(postConv,false))
            return true;
    }

    return false;
}
