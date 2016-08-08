#ifndef SHAREDDATA_HPP
#define SHAREDDATA_HPP

#include <vector>
#include <string>

class Gamestate;

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
    bool interactFlag;
    bool saveGameFlag;
    bool pauseGameFlag;
    bool gameClosedFlag; //used by Npc/Trainer interact functions so they can create/run states then indicate to main state if should close

    std::string nextMapName;
    int nextSpawnId;
    int nextWeather;

    Gamestate* nextState;

    int repelStepsLeft;

    /**
     * Initializes all flags to false
     */
    SharedData();
};

#endif // SHAREDDATA_HPP
