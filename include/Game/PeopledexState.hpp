#ifndef PEOPLEDEXSTATE_HPP
#define PEOPLEDEXSTATE_HPP

#include "Gamestate.hpp"
#include "Menu/Menu.hpp"
#include "Menu/PeopledexList.hpp"

/**
 * Gamestate for the PeopleDex
 *
 * \ingroup Game
 */
class PeopledexState : public Gamestate
{
    PeopledexList peoplemonList;
    MenuImage background, peoplemonPic, pplSeen, pplOwned;
    MenuText peoplemonName, peoplemonDesc, pplSeenT, pplOwnedT;

    /**
     * Wraps a string to fit in a certain area
     *
     * \param str The string to wrap
     * \return The wrapped string
     */
    std::string wordWrap(std::string str);

    /**
     * Updates the UI elements containing peoplemon information to reflect the currently selected peoplemon
     */
    void update();

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
    PeopledexState(Game* g);
};

#endif // PEOPLEDEXSTATE_HPP
