#ifndef BAGSECTIONSELECTOR_HPP
#define BAGSECTIONSELECTOR_HPP

#include "Menu.hpp"

class Game;

/**
 * This class is a helper class for BagState. It defines UI code for the player to select which
 * section of their bag to view
 *
 * \ingroup Menu
 */
class BagSectionSelector
{
    MenuText curSection;
    int index;
    std::string sections[3];

public:
    /**
     * Initializes the UI elements
     */
    BagSectionSelector();

    /**
     * Updates the UI based on user input
     */
    void update();

    /**
     * Renders the UI to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);

    /**
     * Returns the name of the current bag section selected by the player
     *
     * \return Either "Items", "Key Items" or "TM's"
     */
    std::string getCurrentSection();
};

#endif // BAGSECTIONSELECTOR_HPP
