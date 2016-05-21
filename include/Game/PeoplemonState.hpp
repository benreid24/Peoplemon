#ifndef PEOPLEMONSTATE_HPP
#define PEOPLEMONSTATE_HPP

#include "Gamestate.hpp"
#include "Peoplemon/Peoplemon.hpp"

/**
 * Gamestate for the player to view their peoplemon
 *
 * \ingroup Game
 */
class PeoplemonState : public Gamestate
{
    bool shouldReturnChoice;
    int chosenIndex;
    int itemId;
    std::vector<PeoplemonRef>* peoplemon;
    std::string choicePrompt;

    /**
     * Runs the state
     */
    bool execute();

public:
    /**
     * Constructs the state from the given data
     *
     * \param g A pointer to the main Game object
     * \param getChoice Whether or not the player must choose a peoplemon
     * \param ppl A pointer to the peoplemon vector to use
     * \param cPrompt What to ask the player when the choose a peoplemon if getChoice is true
     * \param item The id of an item to use on the chosen peoplemon
     */
    PeoplemonState(Game* g, bool getChoice, std::vector<PeoplemonRef>* ppl, std::string cPrompt = "", int item = -1); //set getChoice to true for picking peoplemon during battle or using items, false when accessing from pause menu

    /**
     * Tells what peoplemon the player chose
     *
     * \return The index of the chosen peoplemon, or -1 if none was chosen
     */
    int getChosenIndex();
};

#endif // PEOPLEMONSTATE_HPP
