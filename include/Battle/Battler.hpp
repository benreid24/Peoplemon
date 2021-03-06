#ifndef BATTLER_HPP
#define BATTLER_HPP

#include <vector>
#include "Peoplemon/Peoplemon.hpp"
#include "Turn.hpp"

/**
 * \defgroup Battle
 * \brief All classes related to battling are in this module
 */

class Game;

/**
 * This structure stores state data used by battles for advanced move effects
 *
 * \ingroup Battle
 */
struct BattlerFlags
{
    bool isCharging, protectUsedLast;    int lastMoveUsed;

    int subHealth; //substitute
    bool ballIsUp, ballHandled, ballSet; //volleyball
    bool switchAfterMove; //baton pass
    bool endureThisTurn, enduredLastTurn; //ensure
    int spikesApplied; //spike (damaged when switching)
    bool healNextPeoplemon; //flag to heal next peoplemon at turn beginning 50%
    bool roarUsed; //roar. makes enemy switch
    bool move64Hit; //Move::FailOnMove64
    bool encoreHit; //true if next move can be encore'd
    int encoreTurnsLeft, encoreMoveId; //encore
    int deathTurnCounter; //DieIn3Turns
    bool koRevive; //ko's current and revives a random peoplemon
    int koReviveHp;

    /**
     * Initializes all the data to the default values
     */
    BattlerFlags();

    /**
     * Resets all the data specific to an individual peoplemon
     */
    void reset();
};

/**
 * Base class to define an interface of a "battler" to be used by BattleState. Will be overridden for the player, AI and network players
 *
 * \ingroup Battle
 */
class Battler
{
protected:
    std::vector<PeoplemonRef>* peoplemon;
    int curPeoplemon;
    int lastDamageDealt, lastDamageTaken;
    bool isWild;

public:
    /**
     * Construct the Battler from a pointer to its peoplemon
     *
     * \param game A pointer to the main Game object
     * \param peoplemon Pointer to a vector containing the peoplemon the battler should use
     */
    Battler(std::vector<PeoplemonRef>* peoplemon);

    /**
     * Destructor. Does nothing
     */
    virtual ~Battler();

    /**
     * Returns the index of the current peoplemon in use
     *
     * \return The index of the current peoplemon
     */
    int getCurrentPeoplemon(); //Battle function will store a vector of peoplemon for each battler and will store its own index of the current peoplemon

	/**
	 * Sets the current Peoplemon. This is used by the Engage ability and Trap ailment to revert back if the switch couldn't occur
	 *
	 * \param c The index of the Peoplemon to switch to
	 */
	void setCurrentPeoplemon(int c);

    /**
     * Returns the action to take on their turn. Will be overridden by subclasses for network/UI/AI functionality
     *
     * \param op The opponents current peoplemon
     * \param g A pointer to the Game object
     * \return A Turn struct indicating what to do
     */
    virtual Turn getTurn(PeoplemonRef op, Game* g); //gets the turn from the AI or network

    /**
     * Returns the new index of the peoplemon to switch to. Will be overridden for UI/network/AI functionality
     *
     * \param op The opponent's peoplemon
     * \param g A pointer to the main Game object
     * \return The index of the new active peoplemon, or -1 if all are dead
     */
    virtual int getSwitchPeoplemon(PeoplemonRef op, Game* g);

    /**
     * Returns a random fainted Peoplemon or -1 if all are alive
     *
     * \param updateCurrent Whether or not to change the current Peoplemon
     * \return The index of the fainted Peoplemon
     */
    int getRandomFaintedPeoplemon(bool updateCurrent);

    /**
     * Returns a pointer to the vector of the battler's peoplemon
     *
     * \return A pointer to the vector of peoplemon
     */
    std::vector<PeoplemonRef>* getPeoplemon();

    /**
     * Tells whether or not the battler is still good for battle. This function always returns true, except for network battlers.
     * In the case of network battlers this function returns true only if the connection is still good
     *
     * \return Whether or not the battler is able to continue battle
     */
    virtual bool statusIsGood(); //used by the network battler to indicate if battle should cease

    /**
     * Recalculates all of the stats of the peoplemon this Battler owns
     *
     * \param g A pointer to the main Game object
     * \param resetAbilities Whether or not to reset the current ability of each Peoplemon. Do at battle start
     */
    void recalcStats(Game* g, bool resetAbilities = false);

    /**
     * Heals all the ailments of the Battler's party
     *
     * \param a Whether or not to also heal "active" ailments
     */
    void healAils(bool a);

    /**
     * Zeros all stat stages for all the peoplemon in the Battler's party
     */
    void zeroStages();

    /**
     * Tells the battler how much damage their last move dealt. For use primarily by AI
     *
     * \param d The amount of damage last dealt
     */
    void setLastDamageDealt(int d);

    /**
     * Tells the battler how much damage they took the last time they were hit. For use primarily by AI
     *
     * \param d The amount of damage last taken
     */
    void setLastDamageTaken(int d);

    /**
     * Tell the Battler whether or not it is wild. The default is to not be wild
     *
     * \param wild Whether or not the Battler is wild
     */
	void setIsWild(bool wild);

	/**
	 * Returns the power mutliplier for FamilyPower:
	 * Damage *1.5 for each other unique Peoplemon with a move with this effect and *1.5 if the user has the ability Total Bro
	 */
    double getBroPower();

    /**
     * Returns the number of non fainted Peoplemon in the party
     */
    int getAliveCount();

    /**
     * Deducts PP from the move used
     */
    void deductPP(int moveId);

    BattlerFlags state;
};

Battler* createBattler(int aiType, std::vector<PeoplemonRef>* pplmon, std::vector<int> items);

#endif // BATTLER_HPP
