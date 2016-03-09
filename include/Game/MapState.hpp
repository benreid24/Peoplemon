#ifndef MAPSTATE_HPP
#define MAPSTATE_HPP

#include "Gamestate.hpp"
#include "SFML.hpp"
#include "Resources/ResourcePool.hpp"
#include <vector>
#include <string>

/**
 * Structure to store data related to objects (towns and cities) on the map
 */
struct MapObject
{
    std::string displayName, referenceName, mapName; //reference for map visited map in world class, mapName for file to actually load
    int spId; //where to spawn them

    sf::Vector2i pos; //in the map window
    bool visited; //how to display
};

/**
 * Defines a state for when the player is viewing the map
 *
 * \ingroup Game
 */
class MapState : public Gamestate
{
    bool canFly;

    sf::Vector2i navPos;
    std::vector<MapObject> towns;

    TextureReference crossHairTxtr, townTxtr, backgroundTxtr;
    sf::Sprite crossHair, town, background; //TODO - get stuff for dialog box on the side of the screen and add code to populate it. also adjust rendering to fit it

    /**
     * Runs the state
     */
    bool execute();

public:
    /**
     * Constructs the state from the given data
     *
     * \param g A pointer to the main Game object
     * \param n The next state to run
     */
    MapState(Game* g, bool cF);
};

#endif // MAPSTATE_HPP
