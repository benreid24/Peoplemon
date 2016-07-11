#ifndef WORLD_HPP
#define WORLD_HPP

#include "Util/Vector2d.hpp"
#include "SFML.hpp"
#include "Object.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Weather.hpp"
#include "Media/Playlist.hpp"
#include "Tileset.hpp"
#include "Media/Animation.hpp"
#include "Scripts/Script Interpreter.hpp"
#include "Util/File.hpp"
#include <memory>

class Game;
class Trainer;
class Npc;

/**
 * \defgroup World
 * \brief All classes related to the world are in this module
 */

/**
 * Structure to store a tile in the map
 *
 * \ingroup World
 */
struct Tile
{
    /**
     * Creates an empty tile
     */
    Tile()
    {
        nonZero = false;
        isAnim = false;
    }
    bool isAnim, nonZero, delA;
    Animation* anim;
    sf::Sprite spr;
};

/**
 * Structure to store a light in the map
 *
 * \ingroup World
 */
struct Light
{
    sf::Vector2f position;
    int radius;
};

/**
 * Structure to store a script in the map
 *
 * \ingroup World
 */
struct MapEvent
{
    std::shared_ptr<Script> script;
    sf::Vector2i position;
    sf::Vector2i size;
    int maxRuns, trigger; //0 = on load, 1 = step in range, 2 = step out of range, 3 = step in or out of range, 4 = in range
    int runs;
};

/**
 * This structure stores the wild peoplemon and area that they can appear for each map
 */
struct PeoplemonZone
{
	sf::IntRect area;
	std::vector<WildPeoplemon> peoplemon;
};

/**
 * This class is responsible for loading, updating and rendering the world. It also owns and updates all entities in the world, as well as handles the weather and scripted play
 *
 * \ingroup World
 */
class World
{
    Game* game;
    std::string name;
    sf::Vector2i size;
    Tileset tileset;
    std::vector<Animation> anims;
    std::vector<Vector2D<Tile> > layers;
    std::vector<Vector2D<std::pair<int,Tile*> > > ySortedTiles;
    Vector2D<int> collisions, catchables, charCols; //using int b/c stupid specialization of vector<bool> conflicting with 2d vector
    std::vector<Object*> objects, objDelQueue, objAddQueue;
    std::vector<std::vector<Object*> > ySortedObjects;
    sf::Mutex objLock;
    std::vector<Light> lights;
    std::vector<MapEvent> events;
    sf::Vector2f camPos;
    sf::Vector2i camPosTiles;

    std::string lastMap, pcMap, curMap;
    sf::Vector2f lastPos;
    int lastDir, pcSpawn;

    int firstYSortLayer, firstTopLayer;
    int ambientLightOverride, currentLighting;
    sf::RenderTexture lightTxtr;
    sf::Sprite lightSpr;
    sf::VertexArray light;

    Weather weather;
    std::shared_ptr<Script> unloadScript;

    std::vector<PeoplemonZone> zones;

    /**
     * When it is determined that a wild peoplemon will attack the player, this function determines which peoplemon will do it and returns that peoplemon
     */
    std::vector<PeoplemonRef> getWildPeoplemon();

    /**
     * Deletes the entire map from memory
     */
    void clear();

    /**
     * Adds the given map to the list of visited maps
     *
     * \param m The name of the place visited
     */
    void addVisitedMap(std::string m);

    std::vector<std::string> visitedMaps;
    std::map<std::string, std::vector<int> > pickedUpItems;
    std::vector<std::string> beatenTrainers;
    std::vector<std::string> talkedToNpcs;

public:
    /**
     * Initializes all the internal world data
     *
     * \param g A pointer to the main Game object
     */
    World(Game* g);

    /**
     * Oh no!
     */
    ~World();

    /**
     * Returns the name of the currently loaded map
     */
	std::string getName();

	/**
	 * Returns the size of the currently loaded map in tiles
	 */
	sf::Vector2i getSize();

    /**
     * Saves all the world data to the given file. This is for game saves
     *
     * \param saveTo The file to save to
     */
    void saveGame(File* saveTo);

    /**
     * Loads all the world data from the given file. This is for game saves
     *
     * \param loadFrom The file to load from
     */
    void loadGame(File* loadFrom);

    /**
     * Loads the map from the given file
     *
     * \param file The name of the map to load. Use "LastMap" to load the previous map
     * \param spId The id of the spawn to put the player at. Use 0 to put the player where they were in the previous map
     * \param trans Whether or not to do a smooth transition
     */
    void load(std::string file, int spId = 0, bool trans = true);

    /**
     * Sets the current weather. See Weather::Type for legal values to pass
     *
     * \param t The type of weather
     */
    void setWeather(int t);

    /**
     * Tells whether or not the given map has been visited
     *
     * \param m The name of the map to check
     * \return Whether or not the map has been visited
     */
    bool mapVisited(std::string m);

    /**
     * Updates the world and all of the entities within it
     */
    void update();

    /**
     * Calculates the new ambient light level, taking time of day, overrides and weather into account
     */
	void calculateLighting();

    /**
     * Renders the world to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);

    /**
     * Sets the camera position based on the players position while making sure that nothing off-map gets rendered
     *
     * \param playerPos The position of the player in world pixels
     */
    void setRenderPosition(sf::Vector2f playerPos); //called by player class each time it moves

    /**
     * This function is used by the player when they move onto a tile. It calls the other moveOnToTile function, while also checking scripts to run and deciding whether or not the player should be attacked
     *
     * \param playerPos The current tile position of the player
     * \param lastPos The last position the player was in
     */
    void moveOntoTile(sf::Vector2i playerPos, sf::Vector2i lastPos);

    /**
     * Checks and plays animations that may have been activated. This function is used by all entities that move
     *
     * \param pos The position of the entity
     */
    void moveOntoTile(sf::Vector2i pos); //version used by other characters and scripts

    /**
     * This function updates an entities location in the rendering order. Only call it when an entity moves, it's not super lightweight
     *
     * \param o The object to relocate
     * \param oldY The previous y tile position
     * \param newY The new y tile position
     */
    void updateObjectRenderLocation(Object* o, int oldY, int newY);

    /**
     * Returns the position of the camera in world pixels
     */
    sf::Vector2f getCamera();

    /**
     * Modifies the collision map to set the given space to be open or not
     *
     * \param position The space to modify
     * \param occupied Whether or not the given space is being used
     */
    void setSpaceOccupied(sf::Vector2i position, bool occupied); //used by trainers/npcs to modify collision layer

    /**
     * Returns whether or not the given space can be moved into
     *
     * \param position The space to check
     * \param oldPos The old (current) position of the entity trying to move
     * \return Whether or not the space can be moved into
     */
    bool spaceFree(sf::Vector2i position, sf::Vector2i oldPos);

    /**
     * Special version of spaceFree for PathFinder. Only returns true for spaces that can be moved onto from all edges
     *
     * \param position The space to check
     * \return Whether or not the space can be moved into
     */
    bool spaceFree(sf::Vector2i position);

    /**
     * Returns the first entity that would be "seen" by an entity at the given direction facing the given direction
     *
     * \param pos The position of the "looking" entity
     * \param dir The direction the entity is facing
     * \param range How many tiles to check
     * \return The first entity that is "seen" or NULL if none in range
     */
    Object* getFirstObject(sf::Vector2i pos, int dir, int range);

    /**
     * Adds the given object to the world
     *
     * \param o The object to add
     */
    void addObject(Object* o);

    /**
     * Removes the given object from the world
     *
     * \param object The object to remove
     */
    void removeObject(Object* object);

    /**
     * Sets the given item to a status where it won't be loaded again when the map is loaded
     *
     * \param id The map unique id of the item
     */
    void setItemPickedUp(int id);

    /**
     * Sets the trainer with the given name to a state where they will no longer battle the player, ever
     *
     * \param name The name of the trainer who was beaten
     */
    void setTrainerBeaten(std::string name);

    /**
     * Sets the NPC with the given name to a state where they won't give the player duplicate items unless explicitly allowed to do so
     *
     * \param name Th name of the NPC
     */
    void setNpcTalkedTo(std::string name);

    /**
     * Tells whether or not the trainer or NPC with the given name has been talked to
     *
     * \param The name of the person to check
     * \return True if they have been talked to already, false if otherwise
     */
	bool checkTalkedTo(std::string name);

    /**
     * Returns a pointer to the trainer with the given name
     *
     * \param nm The name of the trainer to search for
     * \return A pointer to the trainer, or NULL if not found
     */
    Trainer* getTrainer(std::string nm);

    /**
     * Returns a pointer to the npc with the given name
     *
     * \param nm The name of the npc to search for
     * \return A pointer to the npc, or NULL if not found
     */
    Npc* getNPC(std::string nm);

    /**
     * Locks all of the entities in the world. Used when the game is paused
     */
    void lockAllPeople();

    /**
     * Unlocks all of the entities in the world, used when the game is unpaused
     */
    void unlockAllPeople();

    /**
     * Stops all movement animations in Character's
     */
	void stopAnimations();

    /**
     * Adds a light to the world
     *
     * \param x The x position of the light in world pixels
     * \param y The y position of the light in world pixels
     * \param r The radius of the light in pixels
     */
    void addLight(int x, int y, int r);

    /**
     * Removes the light at the given position from the world
     *
     * \param x The x position of the light to remove
     * \param y The y position of the light to remove
     */
    void removeLight(int x, int y);

    /**
     * Removes all lights from the world
     */
    void removeAllLights();

    /**
     * Sets the ambient lighting to the given value, rather than letting it be determined by the time of day. Use when inside
     *
     * \param o The light level [0,255]. Use 255 to let time control the ambient light level
     */
    void setLightingOverride(int o);

    /**
     * Edits the tile at the given position to the new id
     *
     * \param x The x position of the tile
     * \param y The y position of the tile
     * \param layer The layer of the tile
     * \param nId The new id to set the tile to
     */
	void editTile(int x, int y, int layer, int nId);

    /**
     * This loads the map and spawn of the last visited PC center
     */
    void whiteout();
};

#endif // WORLD_HPP
