#ifndef CONVERSATION_HPP
#define CONVERSATION_HPP

#include <vector>
#include <string>

class Player;
class Character;
class ScriptEnvironment;
class Game;

/**
 * Structure to conveniently store all data related to a single "step" or line in a Conversation
 *
 * \ingroup Entities
 */
struct ConvoLine
{
    char code;
    std::string say, line;
    int d1,d2; //in case of code 'c' d1 reperesents start in choice array, d2 represents end. Range is [d1,d2)
};

/**
 * Class to load and execute conversations with NPC's and trainers
 */
class Conversation
{
    std::vector<ConvoLine> lines;
    std::vector<std::pair<std::string,std::string> > choices; //say and goto line
    int cLine;

    /**
     * Sets the current step of the conversation to the jump point with the given name
     *
     * \param l The name of the jump point to go to
     */
    void setLine(std::string l);

public:
    /**
     * Initializes the Conversation to be empty
     */
    Conversation();

    /**
     * Loads the conversation from the given file
     *
     * \param file The file to load from
     */
    Conversation(std::string file);

    /**
     * Loads the conversation from the given file
     *
     * \param file The file to load from
     */
    void load(std::string file);

    /**
     * Resets the Conversation to the first step
     */
    void reset();

    /**
     * Updates the current state of the Conversation to the next step, or the appropriate step based on the player choice made
     *
     * \param game A pointer to the main Game object
     * \param player A pointer to the Player object
     * \param person A pointer to the Character the player is talking to
     * \param env A pointer to the ScriptEnvironment that the Conversation should save its data in
     * \param choice The index of the choice the player made
     * \return A vector of strings with the first index being the "line" to say, and all the indexes after being the choices the player can choose from to respond, if any
     */
    std::vector<std::string> update(Game* game, Player* player, Character* person, ScriptEnvironment* env, int choice = -1); //returns vector of size one for regular line, or more if choice. if choice, vector[0] = say, rest = choices
    //if choice!=-1 then use lines[cLine] to get action before updating cLine
};

#endif // CONVERSATION_HPP
