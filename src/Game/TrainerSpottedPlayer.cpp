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

TrainerSpottedPlayerState::TrainerSpottedPlayerState(Game* g, Trainer* t) : Gamestate(g,NULL)
{
    trainer = t;
    g->player.setLock(true,true);
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

	Vector2i destPos = game->player.getMapPos();
	if (nTDir==0)
		destPos.y++;
	else if (nTDir==1)
		destPos.x--;
	else if (nTDir==2)
		destPos.y--;
	else
		destPos.x++;

    int ensureFps = gameClock.getTimeStamp();
    int rTime = ensureFps;
    while (game->player.getPosition().x!=game->player.getMapPos().x*32 || game->player.getPosition().y!=game->player.getMapPos().y*32)
	{
		ensureFps = gameClock.getTimeStamp();

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
	game->player.forceStop();

    while (trainer->getMapPos()!=destPos)
    {
        ensureFps = gameClock.getTimeStamp();
        trainer->move(game,nTDir);
        if (game->player.getDir()!=nPDir)
			game->player.move(game,nPDir,false,true,false);

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

    return false;
}
