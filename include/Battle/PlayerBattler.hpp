#ifndef PLAYERBATTLER_HPP
#define PLAYERBATTLER_HPP

#include "Battler.hpp"
#include "Menu/Menu.hpp"

class Player;
class BattleState;

/**
 * Subclass of Battler to add functionality for interfacing with the human player through UI elements
 *
 * \ingroup Battle
 */
class PlayerBattler : public Battler
{
    BattleState* battle;
    ChoiceBox typeChoice, moveChoice; //TODO - this will update/render these in getTurn and send turn to network if network.connectionGood()
    bool mustChoose, canRun;

public:
    /**
     * Constructs the object from a pointer to the Player object
     *
     * \param b A pointer to the owner state
     * \param p A pointer to the Player object
     * \param cR Whether or not the player can run from the battle
     */
    PlayerBattler(BattleState* b, Player* p, bool cR);

    /**
     * Destructor. Does nothing
     */
    ~PlayerBattler();

    /**
     * Returns the action to take on their turn. Determined from Player input
     *
     * \param op The opponents current peoplemon
     * \param g A pointer to the Game object
     * \return A Turn struct indicating what to do
     */
    Turn getTurn(PeoplemonRef op, Game* g);

    /**
     * Returns the new index of the peoplemon to switch to. Determined from player input
     *
     * \param op The opponents peoplemon
     * \param g A pointer to the main Game object
     *
     * \return The index of the new active peoplemon
     */
    int getSwitchPeoplemon(PeoplemonRef op, Game* g);
};

#endif // PLAYERBATTLER_HPP
