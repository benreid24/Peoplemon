#ifndef MAPSTATE_HPP
#define MAPSTATE_HPP

#include "Gamestate.hpp"
#include "SFML.hpp"
#include "Menu/Menu.hpp"
#include "Resources/ResourcePool.hpp"
#include <vector>
#include <string>

/**
 * Structure to store data related to objects (towns and cities) on the map
 */
struct MapObject
{
    std::string displayName, referenceName, mapName,description; //reference for map visited map in world class, mapName for file to actually load
    int spId; //where to spawn them
    TextureReference image;

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

    TextureReference crossHairTxtr, townTxtr, backgroundTxtr, playerTxtr, sideBoxTxtr;
    sf::Sprite crossHair, town, background, player, sideBox;
	MenuText townName, townDesc;
	sf::Sprite townImage;

	/**
	 * Wraps the given string to fit in the description box
	 */
	std::string wordWrap(std::string str);

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
