#ifndef PATHDINDER_HPP
#define PATHFINDER_HPP

#include "SFML.hpp"

class Game;

/**
 * A helper class used to generate paths for Characters to navigate the world
 *
 * \ingroup Entities
 */
class PathFinder
{
	Game* game;
	sf::Vector2i start, dest;
	int startDir;
    //something to find

    /**
     * Returns the direction to go to get from one location to the next
     *
     * \param cur The current location
     * \param next The location to move to
     * \return The direction to move in
     */
    int getDir(sf::Vector2i cur, sf::Vector2i next);

public:
	/**
	 * Creates the PathFinder object from the given data
	 *
	 * \param g A pointer to the main Game object
	 * \param s The starting point
	 * \param d The ending point
	 * \param The starting direction that the Character is facing
	 */
	PathFinder(Game* g, sf::Vector2i s, sf::Vector2i d, int sD);

	/**
	 * Returns the path as a sequence of inputs to apply to the Character
	 */
	std::vector<int> getPath();
};

#endif // PATHDINDER_HPP
