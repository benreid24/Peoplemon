#ifndef AIBATTLER_HPP
#define AIBATTLER_HPP

#include "Battle/Battler.hpp"
#include "Score.hpp"
#include "StateFlags.hpp"
#include "ReactionPreference.hpp"
#include "Prioritizer.hpp"

/**
 * \defgroup AI
 * \brief All the battling AI's are contained within this module
 */

/**
 * Base class for all the AI types. It defines some common functionality between them and provides an interface for the
 * different types of AI to use to implement their own unique behaviors
 *
 * \ingroup AI
 * \ingroup Battle
 */
class AIBattler : public Battler
{
protected:
    Weights weights; //inited by subclasses
    std::vector<int> items;
    Prioritizer reactionSorter;
    int turnsSinceSwitch;

    /**
     * Generates a score for the given peoplemon against the given opponent
     *
     * \param ppl The index of the peoplemon to score in the peoplemon vector
     * \param op The peoplemon to score against
     * \param g A pointer to the main Game object
     * \return A Score struct containing score information
     */
    Score getScore(int ppl, PeoplemonRef op, Game* g);

    /**
     * Constructs a set of flags based on a score and opponent
     *
     * \param s The Score struct of the current battle state
     * \param op The opponent to construct flags with respect to
     * \param g A pointer to the main Game object
     * \return A StateFlags struct containing state information
     */
    StateFlags getState(Score s, PeoplemonRef op, Game* g);

    /**
     * Returns a ReactionPreference struct indicating how the AI would prefer to react
     *
     * \param state Current battle state
     * \param score Score for the current battle situation
     * \return The preferred reaction of the AI
     */
    virtual ReactionPreference getReaction(StateFlags state, Score score);

public:
    /**
     * Default constructor. Initializes internal behavior data
     *
     * \param pplmon Pointer to a vector containing the peoplemon for the AI to use
     * \param itms The items the AI can use
     */
    AIBattler(std::vector<PeoplemonRef>* pplmon, std::vector<int> itms);

    /**
     * Destructor. Does nothing
     */
    virtual ~AIBattler();

     /**
     * Returns what action the AI would take against the given opponent
     *
     * \param op The opponent to take the turn against
     * \return The action that the AI is going to take
     */
    virtual Turn getTurn(PeoplemonRef op, Game* g);

    /**
     * Returns the new index of the peoplemon to switch to. Will be overridden for different personalities
     *
     * \param op The opponent's peoplemon
     * \param g A pointer to the main Game object
     * \return The index of the new active peoplemon, or -1 if all are dead
     */
    virtual int getSwitchPeoplemon(PeoplemonRef op, Game* g);
};

#endif // AIBATTLER_HPP
