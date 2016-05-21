#ifndef CHOICEBOX_HPP
#define CHOICEBOX_HPP

#include "SFML.hpp"
#include <vector>
#include <string>

/**
 * This class is for generically creating a choice box with a little arrow to allow the player to
 * choose whichever option they please
 *
 * \ingroup Menu
 */
class ChoiceBox
{
    std::vector<std::string> choices;
    int curC, cCols; //collumns of choices
    bool choiceChosen; //i.e if enter was pressed
    int lastTime, speed;

    sf::Text renderText; //change pos/text for each choice
    sf::Vector2f position;
    sf::RectangleShape box;
    sf::ConvexShape pointer;
    int rowHeight, colWidth; //set to 0 to indicate that they need to be updated

    /**
     * Returns the size of the choice text at the given index
     *
     * \param c The index of the choice to size up
     * \return The size of the choice in a Vector2i
     */
    sf::Vector2i getChoiceSize(int c);

    /**
     * Returns the position of the given choice based on the maximum allowed number of rows and columns
     *
     * \param c The index of the choice to position
     * \return The position where the choice should be
     */
    sf::Vector2f getChoicePos(int c); //return position of choice based on allowed number of rows and cols and text size

public:
    /**
     * Creates an empty choice box
     */
    ChoiceBox();

    /**
     * Sets the choice box to use the secondary menu font in place of the primary one
     */
    void useSecondaryFont();

    /**
     * Sets the desired position of the choice box on the screen
     *
     * \param pos The desired on screen position
     */
    void setPosition(sf::Vector2f pos);

    /**
     * Sets the background color of the box
     *
     * \param col The color to render in the background
     */
    void setBackgroundColor(sf::Color col);

    /**
     * Sets the properties of the border surrounding the box
     *
     * \param col The color of the border
     * \param thick The thickness of the border, in pixels
     */
    void setBorder(sf::Color col, int thick = 3);

    /**
     * Sets the properties of the text used to display the choices. This function must not be called
     * until addChoice is called at least once!
     *
     * \param col The color of the text
     * \param size The font size of the text
     */
    void setTextProps(sf::Color col, int size = 26);

    /**
     * This function adds the given choice to the list of choices
     *
     * \param c The choice to add
     */
    void addChoice(std::string c);

    /**
     * Moves the little arrow to the given choice
     *
     * \param i Index of the choice to make the current selection
     */
    void setCurChoice(int i);

    /**
     * Sets the little arrow to the given choice
     *
     * \param c The choice to make the current selection
     */
    void setCurChocie(std::string c);

    /**
     * Set the width of the choice box. The default is 1. Choices will be added to fill the entire row
     * up to the given width, then will go onto the next row. This function must not be called unless
     * addChoice is called at least once!
     *
     * \param cols The number of columns to use
     */
    void setAllowedCols(int cols);

    /**
     * Set the delay to enforce in between arrow movements
     *
     * \param t The time, in milliseconds, that must elapse before the arrow can move again
     */
    void setSpeed(int t);

    /**
     * Returns the choice chosen by the player if they made a choice, or an empty string otherwise
     *
     * \return The chosen choice by the player
     */
    std::string getChoice(); //returns "" if not chosen yet, else returns the choice

    /**
     * Returns the Currently selected choice
     *
     * \return The currently selected choice
     */
    std::string getCurrentChoice();

    /**
     * Returns the size of the box containing the choices
     */
	sf::Vector2f getSize();

    /**
     * Sets the little pointer to the given choice
     *
     * \param c The choice to set the pointer to
     */
    void setCurrentChoice(std::string c);

    /**
     * Sets the choice box to act as if the player hadn't made a choice, even if they had previously
     */
    void reset();

    /**
     * Clears out all of the choices inside the choice box
     */
    void clear();

    /**
     * Gathers user input and updates the state of the box
     */
    void update();

    /**
     * Renders the box to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);
};

#endif // CHOICEBOX_HPP
