#ifndef NPC_HPP
#define NPC_HPP

#include "Character.hpp"
#include "Conversation.hpp"

/**
 * Class to represent NPC's that walk around in the over world and talk to the player
 *
 * \ingroup Entities
 */
class Npc : public Character
{
    Conversation convo;
    bool wasTalkedTo;
    int lastTime;

public:
    /**
     * Creates the NPC from the given file and sets whether or not they have already spoken to the player
     *
     * \param file The file to load the NPC from
     * \param talked Whether or not the NPC has previously spoken to the player
     */
    Npc(std::string file, bool talked); //init sprite and data and stuff

    /**
     * vtable
     */
    virtual ~Npc() = default;

    /**
     * Updates the NPC, making them walk around and perform their behavior
     *
     * \param game A pointer to the main Game object
     */
    void update(Game* game); //activate behavior, update anim if moving, respond to player if interacted? maybe do that elseware

    /**
     * Starts a conversation with the player if possible
     *
     * \param game A pointer to the main Game object
     */
    void interact(Game* game);

    /**
     * Returns whether or not the NPC has spoken to the player
     */
    bool talkedTo();

    /**
     * Sets the NPC so that they've been talked to
     */
	void setTalked();

    /**
     * Returns a string to identify the object for debug purposes
     */
    std::string getIdentifier();
};

#endif // NPC_HPP
