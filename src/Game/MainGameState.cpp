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
        bool ret = game->runState(new PausedState(game,NULL));
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
    if (game->data.chooseStarterFlag)
    {
        //this might be done with scripting
    }
    if (game->data.openStorageSystemFlag)
    {
        game->data.openStorageSystemFlag = false;
        StorageSystem ss(game,game->player.getStoredPeoplemon());
        if (ss.run())
            return true;
		sleep(milliseconds(225));
    }
    if (game->data.openStoreFlag)
    {
        game->data.openStoreFlag = false;
        return game->runState(new StoreState(game,game->data.storePrompt, game->data.storeError, game->data.storeItems));
    }
    if (game->data.playCreditsFlag)
    {
        game->data.playCreditsFlag = false;
        return game->runState(new CreditsState(game));
    }
    if (game->data.whiteoutFlag)
    {
        game->data.whiteoutFlag = false;
        game->world.whiteout();
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
	}
	if (game->data.nextBattlePplmon.size()>0)
	{
		vector<PeoplemonRef> pplmon;
		PeoplemonRef ppl;
		ppl.load(game,Properties::WildPeoplemonPath+game->data.nextBattlePplmon);
		pplmon.push_back(ppl);
		game->data.nextBattlePplmon = "";
		return game->runState(new BattleState(game,createBattler(game->data.nextBattleAi,&pplmon,vector<int>()),"WILD "+ppl.name,"",0,true,game->data.nextBattleMusic,game->data.nextBattleBgnd));
	}

    return false;
}
