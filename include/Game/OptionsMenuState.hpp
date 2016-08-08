#ifndef OPTIONSMENUSTATE_HPP
#define OPTIONSMENUSTATE_HPP

#include "Gamestate.hpp"

/**
 * Gamestate for the options menu
 *
 * \ingroup Game
 */
class OptionsMenuState : public Gamestate
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
    OptionsMenuState(Game* g, Gamestate* n = nullptr);
};

#endif // OPTIONSMENUSTATE_HPP
