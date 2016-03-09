#ifndef PAUSEDSTATE_HPP
#define PAUSEDSTATE_HPP

#include "Gamestate.hpp"

/**
 * Gamestate for the pause menu in the game
 *
 * \ingroup Game
 */
class PausedState : public Gamestate
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
    PausedState(Game* g, Gamestate* n = NULL);

    /**
     * Destructor, does nothing
     */
    ~PausedState();
};

#endif // PAUSEDSTATE_HPP
