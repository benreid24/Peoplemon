#ifndef PEOPLEDEXLIST_HPP
#define PEOPLEDEXLIST_HPP

#include "Menu/Menu.hpp"

class Game;

/**
 * Helper class for PeopledexState. This creates and controls the GUI for scrolling through all the peoplemon in the Peopledex
 *
 * \ingroup Menu
 */
class PeopledexList
{
    std::vector<std::pair<int,std::string> > pplList; //id, name
    int curPos;
    bool done;

    MenuImage upArrow, downArrow, unSelBox, selBox;
    MenuText pplName;

public:
    /**
     * Creates and initializes all the internal GUI elements
     */
    PeopledexList(Game* g);

    /**
     * Gathers input from the player and updates the internal state
     */
    void update();

    /**
     * Renders the Peopledex to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);

    /**
     * Returns the id of the currently selected peoplemon
     *
     * \return Theid of the currently selected peoplemon
     */
    int getCurPeoplemon();

    /**
     * Tells whether or not the player has indicated that they want to exit the peopledex
     *
     * \return True if the player wants to exit, false otherwise
     */
    bool isFinished();
};

#endif // PEOPLEDEXLIST_HPP
