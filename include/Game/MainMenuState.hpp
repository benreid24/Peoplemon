#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "Gamestate.hpp"
#include "Menu/Menu.hpp"

/**
 * Defines a state for the main menu
 *
 * \ingroup Game
 */
class MainMenuState : public Gamestate
{
    Menu menu;
    ChoiceBox choices;
    MenuImage background;

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
    MainMenuState(Game* g, Gamestate* n = NULL);

    /**
     * Destructor, does nothing
     */
    virtual ~MainMenuState();
};

#endif // MAINMENUSTATE_HPP
