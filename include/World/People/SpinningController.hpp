#ifndef SPINNINGCONTROLLER_HPP
#define SPINNINGCONTROLLER_HPP

#include "Controller.hpp"
#include "Util/File.hpp"

/**
 * Controller class to just spin the Character in place
 *
 * \ingroup Entities
 */
class SpinningController : public Controller
{
    int motion, lastTime;

public:
    /**
     * Loads the spin direction from the given file
     *
     * \param c A pointer to the Character to be controlled
     * \param input The file to load from
     */
    SpinningController(Character* c, File* input);

    /**
     * Creates the controller with the given direction of spinning
     *
     * \param c The Character to be controlled
     * \param b The direction to spin in
     */
    SpinningController(Character* c, int b);

    /**
     * Enacts the behavior on the Character
     *
     * \param g A pointer to the main Game object
     */
    void act(Game* g);
};

#endif // SPINNINGCONTROLLER_HPP
