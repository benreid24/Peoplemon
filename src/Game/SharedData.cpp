#include "Game/SharedData.hpp"

SharedData::SharedData()
{
	fullscreen = false;
    gameMuted = false;
    pauseGameFlag = false;
    saveGameFlag = false;
    loadMapFlag = false;
    whiteoutFlag = false;
    pauseGameFlag = false;
    canSaveWhilePaused = true;
    gameClosedFlag = false;
    saveGameFlag = false;
    interactFlag = false;
    repelStepsLeft = 0;
    nextState = nullptr;
    lastBattleWon = false;
}
