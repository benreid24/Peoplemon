#ifndef DELETEMOVESTATE_HPP
#define DELETEMOVESTATE_HPP

#include "Gamestate.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Resources/ResourcePool.hpp"
#include "Menu/Menu.hpp"

/**
 * This gamestate allows the player to choose which move their Peoplemon should forget
 *
 * \ingroup Game
 */
class DeleteMoveState : public Gamestate
{
    PeoplemonRef* ppl;
    int moves[5];
    std::map<std::string,int> revLookup;
    bool taught;

    MenuImage bgnd;
    MenuText power, acc, type, desc;
    ChoiceBox choices;
    Menu menu;

    /**
     * Runs the state
     */
    bool execute();

    /**
     * Wraps the given string to fit in the description box
     *
     * \param str The string to wrap
     * \return The wrapped string
     */
    std::string wordWrap(std::string str);

public:
    /**
     * Creates the gamestate from the given parameters
     *
     * \param g A pointer to the main Game object
     * \param p A pointer to the Peoplemon who is learning the new move
     * \param m The id of the move being learned
     */
    DeleteMoveState(Game* g, PeoplemonRef* p, int m);

    /**
     * Indicates whether or not the move was taught
     *
     * \return True if the Peoplemon learned the move, false otherwise
     */
	bool moveLearned();
};

#endif // DELETEMOVESTAT_HPP
