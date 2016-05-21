#ifndef BAGSTATE_HPP
#define BAGSTATE_HPP

#include "Gamestate.hpp"
#include <vector>
#include <string>

/**
 * Gamestate that implements the UI for when the player is viewing their bag
 *
 * \ingroup Game
 */
class BagState : public Gamestate
{
    bool inBattle;
    bool getChoice;
    int choice;
    std::string retMes;

    /**
     * Runs the state
     */
    bool execute();

    /**
     * Gets the player's choice of peoplemon to use an item on
     *
     * \param id The id of the item being used
     * \return The index of the peoplemon to operate on, or -1 if none was chosen. -100 is returned if the game should close
     */
    int getPeoplemon(int id);

    /**
     * Returns a wrapped version of the given string
     *
     * \param str The string to wrap
     * \return The same string but with newlines inserted to make it fit
     */
	std::string wordWrap(std::string str);

	/**
	 * Displays a message to the screen, handles the updating of HUD
	 *
	 * \param m The message to show
	 * \return True if the game should close, false otherwise
	 */
	bool displayMessage(std::string m);

public:
    /**
     * Create the state
     *
     * \param g Pointer to the main Game object
     * \param inB Whether or not the player is currently in battle
     * \param getC Whether or not the player is simply viewing the bag or accessing it to use an item
     */
    BagState(Game* g, bool inB, bool getC); //returns after one item if in battle

    /**
     * Returns the id of the chosen item to use
     *
     * \return The id of the chosen item or 0 if no item was chosen
     */
    int chosenItem();

    /**
     * Returns the output message of the result of using the item
     */
    std::string getMessage();
};

#endif // BAGSTATE_HPP
