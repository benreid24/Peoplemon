#include "Game/MainGameState.hpp"
#include "Game/PausedState.hpp"
#include "Game/CreditsState.hpp"
#include "Game/StorageSystem.hpp"
#include "Game/StoreState.hpp"
#include "Game/BattleState.hpp"
#include "Battle/Battler.hpp"
#include "SFML.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Game/NicknameState.hpp"
#include "Game/NetworkConnectingState.hpp"
using namespace sf;
using namespace std;

MainGameState::MainGameState(Game* g, Gamestate* n) : Gamestate(g,n)
{
    pTime = 0;
}

MainGameState::~MainGameState()
{
    //dtor
}

bool MainGameState::execute()
{
    int sTime = gameClock.getTimeStamp();
    int rTime = sTime;

    while (!finishFrame())
    {
    	sTime = gameClock.getTimeStamp();
        game->world.update();
        game->hud.update();

        if (handleFlags())
            return true;

		if (gameClock.getTimeStamp()-rTime>16)
        {
        	game->mainWindow.clear();
			game->world.draw(&game->mainWindow);
			game->hud.draw(&game->mainWindow);
			game->mainWindow.display();
			rTime = gameClock.getTimeStamp();
        }

		if (gameClock.getTimeStamp()-sTime<8)
			sleep(milliseconds(8-gameClock.getTimeStamp()+sTime));
    }

    return true;
}

bool MainGameState::handleFlags()
{
    if (game->data.gameClosedFlag)
        return true;
    if (game->data.pauseGameFlag && gameClock.getTimeStamp()>pTime)
    {
        bool ret = game->runState(new PausedState(game,nullptr));
        game->data.pauseGameFlag = false;
        pTime = gameClock.getTimeStamp()+500;
        return ret;
    }
    else if (game->data.pauseGameFlag)
        game->data.pauseGameFlag = false;
    if (game->data.loadMapFlag)
    {
        game->data.loadMapFlag = false;
        game->world.load(game->data.nextMapName, game->data.nextSpawnId);
    }
    if (game->data.nextWeather!=-1)
    {
        game->world.setWeather(game->data.nextWeather);
        game->data.nextWeather = -1;
    }
    if (game->data.interactFlag)
	{
		game->data.interactFlag = false;
		game->player.interact(game);
		game->world.resetMotionTimers();
	}
	if (game->data.nextState!=nullptr)
    {
    	bool ret = game->runState(game->data.nextState,true);
    	game->data.nextState = nullptr;
    	sleep(milliseconds(250));
    	return ret;
    }
    if (game->data.whiteoutFlag)
    {
        game->data.whiteoutFlag = false;
        game->world.whiteout();
    }

	//if (Keyboard::isKeyPressed(Keyboard::N))
	//	return game->runState(new NetworkConnectingState(game),true);

    return false;
}
