#ifndef TRAINERSPOTTEDPLAYERSTATE_HPP
#define TRAINERSPOTTEDPLAYERSTATE_HPP

#include "Gamestate.hpp"

class Trainer;
class BattleState;
class ConversationState;

/**
 * Gamestate for when a Trainer has spotted the player and is walking to them
 *
 * \ingroup Game
 */
class TrainerSpottedPlayerState : public Gamestate
{
    Trainer* trainer;
    BattleState* battle;
    ConversationState *preConv, *postConv;

    /**
     * Runs the state
     */
    bool execute();

public:
    /**
     * Constructs the state from the given data
     *
     * \param g A pointer to the main Game object
     * \param t A pointer to the trainer who has spotted the player
     * \param n The next state to run
     */
    TrainerSpottedPlayerState(Game* g, Trainer* t, BattleState* b, ConversationState* before, ConversationState* after, Gamestate* n = NULL);

    /**
     * Unlocks the player and trainer
     */
    ~TrainerSpottedPlayerState();
};

#endif // TRAINERSPOTTEDPLAYERSTATE_HPP
