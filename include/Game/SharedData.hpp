#ifndef SHAREDDATA_HPP
#define SHAREDDATA_HPP

#include <vector>
#include <string>

/**
 * Structure to store flags for inter-thread and inter-routine communication
 *
 * \ingroup Game
 */
struct SharedData
{
	bool fullscreen;
    bool gameMuted;
    bool loadMapFlag;
    bool whiteoutFlag;
    bool openStoreFlag;
    bool chooseStarterFlag;
    bool playCreditsFlag;
    bool interactFlag;
    bool openStorageSystemFlag;
    bool saveGameFlag;
    bool pauseGameFlag;
    bool gameClosedFlag; //used by Npc/Trainer interact functions so they can create/run states then indicate to main state if should close

    std::string nextMapName;
    int nextSpawnId;
    int nextWeather;

    std::string nextBattlePplmon, nextBattleMusic, nextBattleBgnd;
    int nextBattleAi;

    std::string storePrompt, storeError;
    std::vector<std::pair<int,int> > storeItems;

    //TODO - data here for items like repel to manipulate
    //for example
    int repelStepsLeft;

    /**
     * Initializes all flags to false
     */
    SharedData();
};

#endif // SHAREDDATA_HPP
