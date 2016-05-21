#ifndef MENUTEXT_HPP
#define MENUTEXT_HPP

#include "SFML.hpp"
#include <string>

/**
 * Class used to easily manipulate and render text to the screen
 *
 * \ingroup Menu
 * \ingroup Media
 */
class MenuText
{
    sf::Text text;
    bool m;

public:
    /**
     * Creates an empty menu text object
     */
    MenuText();

    /**
     * Creates a menu text object from a string
     *
     * \param t The text to put in the object
     */
    MenuText(std::string t);

    /**
     * Sets the text object to use the secondary font instead of the primary
     */
    void useSecondaryFont();

    /**
     * Sets the text inside of the object
     *
     * \param t The text to put inside the object
     */
    void setText(std::string t);

    /**
     * Sets the position of the object on th screen
     *
     * \param pos The position to render the text at
     */
    void setPosition(sf::Vector2f pos);

    /**
     * Set the rendering properties of the text
     *
     * \param col The color of the text
     * \param size The size of the text
     */
    void setProps(sf::Color col, int size = 26);

    /**
     * Renders the text to the given windo
     *
     * \param window The window to render the text to
     */
    void draw(sf::RenderWindow* window);
};

#endif // MENUTEXT_HPP
