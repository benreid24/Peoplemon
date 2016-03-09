#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <cstdlib>

class Game;

/**
 * Base class to define a gamestate
 *
 * \ingroup Game
 */
class Gamestate
{
protected:
    Game* game;
    Gamestate* next;

    /**
     * Runs the state
     */
    virtual bool execute()=0;

public:
    /**
     * Construct the state from the Game object and the next state to run
     *
     * \param g A pointer to the main Game object
     * \param n The next state to run
     */
    Gamestate(Game* g, Gamestate* n = NULL);

    /**
     * This is only here for polymorphism to be happy
     */
    virtual ~Gamestate();

    /**
     * Runs the state and runs the next state if one was specified
     *
     * \return Whether or not the game should close
     */
    bool run();

    /**
     * Handles all window events
     *
     * \return Whether or not the window should close
     */
    bool finishFrame();
};

#endif // GAMESTATE_HPP
