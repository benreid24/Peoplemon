#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include "MenuText.hpp"
#include "MenuImage.hpp"
#include "ChoiceBox.hpp"
#include "ScrollChoiceBox.hpp"

/**
 * \defgroup Menu
 * \brief All classes related to menu's and user interfacing are in this module
 */

/**
 * Container class to easily render batches of GUI elements
 *
 * \ingroup Menu
 */
class Menu
{
    std::vector<MenuText*> text;
    std::vector<MenuImage*> images;
    std::vector<ChoiceBox*> choices;
    std::vector<ScrollChoiceBox*> scrolls;

public:
    /**
     * Constructs an empty menu
     */
    Menu();

    /**
     * Adds a MenuText element to the menu
     *
     * \param text A pointer to the MenuText object to add
     */
    void add(MenuText* text);

    /**
     * Adds a MenuImage element to the menu
     *
     * \param img A pointer to the MenuImage object to add
     */
    void add(MenuImage* img);

    /**
     * Adds a ChoiceBox object to the menu
     *
     * \param c A pointer to the ChoiceBox object to add
     */
    void add(ChoiceBox* c);

    /**
     * Adds a ScrollChoiceBox element to the menu
     *
     * \param s A pointer to the ScrollChoiceBox to add
     */
    void add(ScrollChoiceBox* s);

    /**
     * Renders all of the menu elements to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);
};

#endif // MENU_HPP
