#ifndef GAME_HPP
#define GAME_HPP

#include "SFML.hpp"
#include "Gamestate.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Peoplemon/Move.hpp"
#include "Resources/ResourcePool.hpp"
#include "World/World.hpp"
#include "World/People/Player.hpp"
#include "Scripts/Script Environment.hpp"
#include "Media/SoundEngine.hpp"
#include "Menu/HUD.hpp"
#include "SharedData.hpp"

/**
 * This is for storing data in the item database
 */
struct ItemDBEntry
{
    std::string name, description;
    int price;
};

/**
 * \defgroup Game
 * \brief All class related to game states are in this module
 */

/**
 * Master class to store all global game data and handle the initialization of all common data
 *
 * \ingroup Game
 */
class Game
{
public:
    /**
     * Loads all the data from database files and initializes all common game data
     */
    Game();

    /**
     * This doesn't really serve a purpose
     */
    ~Game();

    /**
     * Starts the game!
     */
    void start();

    /**
     * Runs the given Gamestate and deletes it when done
     *
     * \param s The state to run
     * \param delWhenDone Whether or not to delete the state when it is finished
     * \return Whether or not the game should close
     */
    bool runState(Gamestate* s, bool delWhenDone = true);

    /**
     * Runs the given Gamestate and deletes it when done if necessary. Also sets the appropriate flags for the MainGameState rather than returning the result
     *
     * \param s The state to run
     * \param delWhenDone Whether or not to delete the state when it is finished
     */
    void runStateUnderPriveldged(Gamestate* s, bool delWhenDone = true);

    /**
     * Saves the game to the appropriate file
     */
    void save();

    /**
     * Loads the game from the given file
     *
     * \param name The name of the save file to load, without path or extension
     */
    void load(std::string name);

    sf::RenderWindow mainWindow;
    ScriptEnvironment scriptEnvironment;

	SoundEngine soundEngine;
    Player player;
    World world;
    HUD hud;
    Playlist music;
    SharedData data;
    bool inFocus;

    std::map<int,Peoplemon> peoplemonList;
    std::map<int,Move> moveList;
    std::map<int,ItemDBEntry> itemList;
    std::string ailmentList[7];
    std::string typeList[19];
};

#endif // GAME_HPP
