#ifndef SCRIPTENVIRONMENT_HPP
#define SCRIPTENVIRONMENT_HPP

#include "SFML.hpp"
#include "Script Interpreter.hpp"
#include "Resources/ResourceTypes.hpp"
#include <list>
#include <map>
#include <memory>

class Game;
class File;
class ScriptEnvironment;

/**
 * Structure to store all data needed to run a script from the map
 *
 * \ingroup Scripting
 */
struct ScriptData
{
    ScriptEnvironment* owner;
    std::shared_ptr<sf::Thread> thread;
    ScriptReference script;
    bool finished;
};

/**
 * This class is responsible for running and updating all of the scripts ran from the math
 *
 * \ingroup Scripting
 */
class ScriptEnvironment
{
    Game* game;
    std::list<std::shared_ptr<ScriptData> > runningScripts;

    sf::Thread thread;
    sf::Mutex lock;
    bool stopping, stopped;

    /**
     * Thread function that will update and manage all running scripts
     */
    void update();

    /**
     * Terminates the updater thread and returns when the thread is finished
     */
	void stop();

public:
    /**
     * Initializes the ScriptEnvironment and starts the updater thread
     *
     * \param g A pointer to the main Game object
     */
    ScriptEnvironment(Game* g);

    /**
     * Terminates the updater thread and all running scripts
     */
    ~ScriptEnvironment();

    /**
     * Runs a script from the map. Will not run the script if it is already running in parallel
     *
     * \param scr A pointer to the Script to run
     * \param concurrent Whether or not the run the script concurrently. Default is false
     */
    void runScript(ScriptReference scr, bool concurrent = false);

    /**
     * Stops all of the currently running threads
     */
    void stopAll();

    /**
     * Returns a pointer to the main Game object
     */
    Game* getGame();

    /**
     * Saves the environment variables to the given file
     *
     * \param file The File object to save to
     */
    void save(File* file);

    /**
     * Loads the environment variables from the given file
     *
     * \param file The File object to load from
     */
    void load(File* file);

    std::map<std::string,int> intSaveEntries;
    std::map<std::string,std::string> stringSaveEntries;
};

#endif // SCRIPTENVIRONMENT_HPP
