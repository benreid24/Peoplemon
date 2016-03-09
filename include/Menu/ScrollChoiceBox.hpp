#ifndef SCROLLCHOICEBOX_HPP
#define SCROLLCHOICEBOX_HPP

#include <vector>
#include <string>
#include "SFML.hpp"

/**
 * This class is identical to the ChoiceBox class but it allows only some of the choices to be visible at once
 *
 * \ingroup Menu
 */
class ScrollChoiceBox
{
    std::vector<std::string> choices;
    int topChoice, pointerChoice, lastTime, choicesVisible; //top choice is the index of the first choice being rendered. pointerChoice is the index relative to topChoice
    bool choiceChosen;

    sf::Vector2f position;
    sf::RectangleShape box;
    sf::ConvexShape pointer;
    sf::Text renderText;
    sf::Vector2i textSize; //(maxWidth, text height)

    /**
     * Returns the position of the choice at the given index
     *
     * \param i The index of the choice to position
     */
    sf::Vector2f getChoicePos(int i); //raw index

public:
    /**
     * Initializes the ScrollChoiceBox to be empty
     */
    ScrollChoiceBox();

    /**
     * Sets the position of the box
     *
     * \param pos The position
     */
    void setPosition(sf::Vector2f pos);

    /**
     * Sets the fonts of the choices to be th secondary menu font
     */
    void useSecondaryFont();

    /**
     * Sets the amount of choices allowed to be visible at once
     *
     * \param v The amount of choices shown at once
     */
    void setVisibleChoices(int v);

    /**
     * Sets the properties of the text the choices are rendered with
     *
     * \param col The color of the text
     * \param size The size of the text
     */
    void setTextProps(sf::Color col, int size = 18);

    /**
     * Sets the background color of the box
     *
     * \param col The background color of the box
     */
    void setBackgroundColor(sf::Color col);

    /**
     * Sets the border of the box
     *
     * \param col The color of the box
     * \param t The thickness of the border in pixels
     */
    void setBorder(sf::Color col, int t = 3);

    /**
     * Adds a choice to the list of choices
     *
     * \param c The choice to add
     */
    void addChoice(std::string c);

    /**
     * Sets the currently selected choice to the given index
     *
     * \param i Index of the choice to select
     */
    void setChoice(int i);

    /**
     * Sets the currently selected choice to the given choice
     *
     * \param c The choice to select
     */
    void setChoice(std::string c);

    /**
     * Returns the selected choice
     *
     * \return The choice selected by the user, or empty if no selection has been made
     */
    std::string getChoice();

    /**
     * Returns the choice presently "selected" by the player
     *
     * \return The choice that the arrow is currently on
     */
    std::string getCurrentChoice();

    /**
     * Deletes all the choices in the box
     */
    void clear();

    /**
     * Gathers user input and updates the box
     */
    void update();

    /**
     * Resets the box so that it is as if the player had never made a choice
     */
    void reset();

    /**
     * Renders the box to the given window
     *
     * \param window The window to render the box to
     */
    void draw(sf::RenderWindow* window);
};

#endif // SCROLLCHOICEBOX_HPP
