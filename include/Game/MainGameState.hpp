#ifndef MAINGAMESTATE_HPP
#define MAINGAMESTATE_HPP

#include "Gamestate.hpp"

/**
 * Defines the main state of the game, which is when the player is walking around outside of all menus and battle
 *
 * \ingroup Game
 */
class MainGameState : public Gamestate
{
    /**
     * Runs the state
     */
    bool execute();

    /**
     * Interprets and handles all flags set by scripts and lower functions
     *
     * \return Whether or not the game should close
     */
    bool handleFlags();

    int pTime;

public:
    /**
     * Constructs the state from the given data
     *
     * \param g A pointer to the main Game object
     * \param n The next state to run
     */
    MainGameState(Game* g, Gamestate* n = NULL);

    /**
     * Destructor, does nothing
     */
    virtual ~MainGameState();
};

#endif // MAINGAMESTATE_HPP
