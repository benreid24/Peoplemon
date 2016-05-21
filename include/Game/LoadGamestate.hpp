#ifndef LOADGAMESTATE_HPP
#define LOADGAMESTATE_HPP

#include "Gamestate.hpp"

/**
 * Gamestate for choosing the file to load, handles the actual loading and transition to play as well
 *
 * \ingroup Game
 */
class LoadGamestate : public Gamestate
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
     * \param n The next state to run
     */
    LoadGamestate(Game* g, Gamestate* n);
};

#endif // LOADGAMESTATE_HPP
