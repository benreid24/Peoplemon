#ifndef PEOPLEMONSELECTOR_HPP
#define PEOPLEMONSELECTOR_HPP

#include "Menu.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include <vector>

class Game;

/**
 * Base class for the PeoplemonSelector class to be able to use mutliple types of buttons generically
 *
 * \ingroup Menu
 */
class PeoplemonButtonBase
{
    bool isActive;

protected:
    Menu elements;
    MenuImage pic, activePic;

public:
    /**
     * Does nothing
     */
    PeoplemonButtonBase();

    /**
     * Here just for polymorphism to be happy
     */
    virtual ~PeoplemonButtonBase();

    /**
     * Sets whether or not the button should be highlighted
     *
     * \param a Whether or not the button is active
     */
    void makeActive(bool a); //highlight for when selected

    /**
     * Renders the button to the given window
     *
     * \param window The window to render the button to
     */
    virtual void draw(sf::RenderWindow* window);
};

/**
 * Defines the button used to represent a peoplemon in the party
 *
 * \ingroup Menu
 */
class PeoplemonButtonMain : public PeoplemonButtonBase
{
	MenuImage pplmon;
    MenuText name, level, hp, item;
    sf::RectangleShape hpBar;

public:
    /**
     * Constructs the button from the given data
     *
     * \param pos The position of the button on the screen
     * \param ppl The peoplemon to use to populate the button
     * \param g A pointer to the main Game object
     */
    PeoplemonButtonMain(sf::Vector2f pos, PeoplemonRef ppl, Game* g);

    /**
     * Cleans up internal data
     */
    virtual ~PeoplemonButtonMain();

    /**
     * Repopulates the button with a new peoplemon
     *
     * \param ppl The peoplemon to update the button with
     * \param g A pointer to the main Game object
     */
    void update(PeoplemonRef ppl, Game* g);

    /**
     * Renders the button to the given screen
     *
     * \param window The window to render the button to
     */
    void draw(sf::RenderWindow* window);
};

/**
 * This class is for the cancel button in the selection menu
 *
 * \ingroup Menu
 */
class PeoplemonButtonCancel : public PeoplemonButtonBase
{
public:
    /**
     * Creates the cancel button with the appropriate graphics in the appropriate position
     */
    PeoplemonButtonCancel(); //position is known

    /**
     * Cleans up internal resources
     */
    virtual ~PeoplemonButtonCancel();
};

/**
 * This is a helper class for PeoplemonState. It creates and controls the menu for the player to select and edit their peoplemon in the party
 *
 * \ingroup Menu
 */
class PeoplemonSelector
{
    std::vector<PeoplemonButtonBase*> buttons;
    int curPointer, secondPointer;
    bool getSecondary;
    bool enterPressed;
    bool aCancel;

public:
    /**
     * Initializes the menu with the given party of peoplemon
     *
     * \param ppl The party of peoplemon to initialize the menu with
     * \param g A pointer to the main Game object
     * \param allowCancel Whether or not to allow the player to cancel
     */
    PeoplemonSelector(std::vector<PeoplemonRef>* ppl, Game* g, bool allowCancel = true);

    /**
     * Cleans up internal resources
     */
    ~PeoplemonSelector();

    /**
     * Sets up the menu to represent the given party of peoplemon
     *
     * \param ppl The party of peoplemon to initialize the menu with
     * \param g A pointer to the main Game object
     */
    void sync(std::vector<PeoplemonRef>* ppl, Game* g);

    /**
     * Updates the menu using player input
     */
    void update(); //updates pointer, checks for selection

    /**
     * Sets whether or not the player can make a second peoplemon selection. This is for rearranging the order of the peoplemon in the party
     *
     * \param s True if a second selection can be made, false otherwise
     */
    void setSecondSelection(bool s);

    /**
     * Returns the index of the primary peoplemon selection in the vector representing the party
     *
     * \return Index of the first selected peoplemon
     */
    int primarySelection();

    /**
     * Returns the index of the secondary selection peoplemon in the vector representing the party
     *
     * \return The index of the second selected peoplemon
     */
    int secondarySelection();

    /**
     * Tells whether or not the player has selected a peoplemon, either primary or secondary
     *
     * \return True if the player has selected a peoplemon, false otherwise
     */
    bool selectionMade();

    /**
     * Renders the menu to the given window
     *
     * \param window The window to render the menu to
     */
    void draw(sf::RenderWindow* window);
};

#endif // PEOPLEMONBUTTON_HPP
