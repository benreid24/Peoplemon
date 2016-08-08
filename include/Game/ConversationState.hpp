#ifndef COVERSATIONSTATE_HPP
#define COVERSATIONSTATE_HPP

#include "Gamestate.hpp"
#include <vector>
#include <string>

class Conversation;
class Character;

/**
 * Defines a state for when the player is in conversation with an NPC or Trainer in the over world
 *
 * \ingroup Game
 */
class ConversationState : public Gamestate
{
    Conversation* convo;
    Character* person;

    /**
     * Runs the state
     */
    bool execute(); //run conversation while updating and rendering the world in the background

    /**
     * Handles the data returned by the conversation object
     *
     * \param data The data returned by the conversation object
     */
    void handleData(std::vector<std::string> data);

    /**
     * Little helper function to find an element in a vector
     *
     * \param v The vector to search
     * \param c The element to search for
     * \return The index of where the element first occurs
     */
    int choiceIndex(std::vector<std::string> v, std::string c);

public:
    /**
     * Constructs the ConversationState from the given parameters
     *
     * \param g A pointer to the main Game object
     * \param c The Character the player is talking to
     * \param cv A pointer to the Conversation object to use
     * \param n The next state to run
     */
    ConversationState(Game* g, Character* c, Conversation* cv, Gamestate* n = nullptr); //lock player and character

    /**
     * Destructor, unlocks the player and their conversation partner
     */
    virtual ~ConversationState(); //unlock them
};

#endif // COVERSATIONSTATE_HPP
