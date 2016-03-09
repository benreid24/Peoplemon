#include "Game/SharedData.hpp"

SharedData::SharedData()
{
	fullscreen = false;
    gameMuted = false;
    pauseGameFlag = false;
    saveGameFlag = false;
    loadMapFlag = false;
    whiteoutFlag = false;
    playCreditsFlag = false;
    openStorageSystemFlag = false;
    openStoreFlag = false;
    pauseGameFlag = false;
    gameClosedFlag = false;
    saveGameFlag = false;
    interactFlag = false;
    repelStepsLeft = 0;
    nextSpawnId = 0;
    nextWeather = -1;
}
