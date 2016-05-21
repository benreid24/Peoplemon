#ifndef WANDERINGCONTROLLER_HPP
#define WANDERINGCONTROLLER_HPP

#include "Controller.hpp"
#include "Util/File.hpp"

/**
 * This controller makes the Character wander within a certain radius, favoring wandering towards where they spawned to keep them in their zone. Also
 * will switch randomly between other controllers to vary the behavior
 *
 * \ingroup Entities
 */
class WanderingController : public Controller
{
    int radius; //0 for infinite

    /**
     * Defines the current controller being used
     */
    enum State
    {
        Still = 0,
        Spinning = 1,
        Path = 2,
        Wander = 3
    }state;

    Controller* subBehavior;
    int timeOfNextState, timeOfNextStep;

    bool initialized;
    sf::Vector2i startPos;

    /**
     * Generates a path randomly that stays within the radius and doesn't pass through walls
     *
     * \param g A pointer to the main Game object
     * \return The path as a vector of int's, each representing the direction to step in at that point in the path
     */
    std::vector<int> generatePath(Game* g);

    /**
     * Returns a random direction to move in that favors moving towards the center of the wander radius while making sure that they don't go outside of it
     *
     * \param pos The current position of the Character
     * \return The direction to move in
     */
    int bestDirFromPos(sf::Vector2i pos);

    /**
     * Helper function that returns the square magnitude of the given vector
     *
     * \param v The vector whose magnitude should be calculated
     * \return The magnitude of the vector squared
     */
    int magnitudeSquared(sf::Vector2i v);

public:
    /**
     * Creates the wandering controller by loading its parameters from the given file
     *
     * \param c The Character that should be controlled
     * \param file The file to load from
     */
    WanderingController(Character* c, File* file);

    /**
     * Enacts the behavior on the Character
     *
     * \param g A pointer to the main Game object
     */
    void act(Game* g);
};

#endif // WANDERINGCONTROLLER_HPP
