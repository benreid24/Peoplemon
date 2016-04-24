#ifndef TRAINER_HPP
#define TRAINER_HPP

#include "Character.hpp"
#include <vector>
#include "Peoplemon/Peoplemon.hpp"
#include "Conversation.hpp"

class Game;

/**
 * Class to represent trainers in the over world and store their peoplemon
 *
 * \ingroup Entities
 */
class Trainer : public Character
{
    Conversation preBattle, postBattle;
    std::string loserSay; //what to say in battle after losing
    std::string bMusic, bBgnd;
    std::vector<PeoplemonRef> peoplemon;
    std::vector<int> items;
    int prizeMoney;
    bool beaten, confrontingPlayer;
    int range;
    int aiType;
    int lastTime;

    /**
     * Starts a battle with the player by invoking the TrainerSpottedPlayer state
     */
    void startFight(Game* game);

public:
    /**
     * Creates the Trainer from the given data
     *
     * \param g A pointer to the main Game object
     * \param file The file to load the trainer from
     * \param lost Whether or not the trainer has already been beaten by the player
     */
    Trainer(Game* g, std::string file, bool lost);

    /**
     * Enacts the trainer's behavior and, if they haven't yet been beaten, checks to see if they should battle the player
     *
     * \param game A pointer to the main Game object
     */
    void update(Game* game);

    /**
     * Talks to the player, battles if not already beaten, then talks to the player again if they lose
     *
     * \param game A pointer to the main Game object
     */
    void interact(Game* game);

    /**
     * Returns whether or not the trainer has been beaten by the player yet
     */
    bool isDefeated();

    /**
     * Sets the trainer so that they've been beaten
     */
	void setBeaten();

    /**
     * Returns a string to identify the object for debug purposes
     */
    std::string getIdentifier();
};

#endif // TRAINER_HPP
