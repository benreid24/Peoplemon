#ifndef STANDINGCONTROLLER_HPP
#define STANDINGCONTROLLER_HPP

#include "Controller.hpp"

/**
 * Controller to have the character simply stand in place
 *
 * \ingroup Entities
 */
class StandingController : public Controller
{
    //does nothing, character just stands in place
public:
    /**
     * Links the controller to its owner for literally no reason
     *
     * \param c The Character to control
     */
    StandingController(Character* c) : Controller(c) {}

    /**
     * Does nothing
     */
    void act(Game* g){}
};

#endif // STANDINGCONTROLLER_HPP
