#ifndef SCREENKEYBOARD_HPP
#define SCREENKEYBOARD_HPP

#include "ChoiceBox.hpp"
#include "MenuText.hpp"
#include "MenuImage.hpp"

/**
 * This class is used to create an on screen keyboard that can be used to get textual input from the player
 *
 * \ingroup Menu
 */
class ScreenKeyboard
{
    std::string typedText;
    MenuImage background;
    MenuText text;
    ChoiceBox keys;
    bool finished, allowEmpty;
    int lastTime, charLim;
    std::string lastInput;

public:
    /**
     * Creates the keyboard with an optional starting string to put in the input box
     *
     * \param init The string to put in the input box
     */
    ScreenKeyboard(std::string init = "", bool allowEmpty = false);

    /**
     * Sets a character limit on what can be inputted
     *
     * \param l The maximum amount of characters allowed
     */
    void setInputLimit(int l);

    /**
     * Gets user input and updates the input box
     */
    void update();

    /**
     * Returns the current content of the input box
     *
     * \return The current text as inputted by the user
     */
    std::string getText();

    /**
     * Tells whether or not the player finished their input
     *
     * \return Whether or not the player finished inputting the string
     */
    bool isFinished();

    /**
     * Renders the keyboard to the given window
     *
     * \param window The window to render the keyboard to
     */
    void draw(sf::RenderWindow* window);
};

#endif // SCREENKEYBOARD_HPP
