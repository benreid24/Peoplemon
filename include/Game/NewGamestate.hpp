#ifndef NEWGAMESTATE_HPP
#define NEWGAMESTATE_HPP

#include "Gamestate.hpp"

/**
 * Defines a state for when the player creates a new game. The introduction is done here
 *
 * \ingroup Game
 */
class NewGamestate : public Gamestate
{
    /**
     * Runs the state
     */
    bool execute();

public:
    /**
     * Constructs the state from the given data
     *
     * \param g A pointer to the main Game object
     */
    NewGamestate(Game* g);
};


#endif // NEWGAMESTATE_HPP
