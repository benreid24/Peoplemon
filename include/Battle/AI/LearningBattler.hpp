#ifndef LEARNINGBATTLER_HPP
#define LEARNINGBATTLER_HPP

#include "AIBattler.hpp"

/**
 * Defines an AI that adapts to the player over time by selecting strategies from other AI types that perform well.
 * It can also make small tweaks to those strategies and compare the new performance to old and change over time to
 * be more dynamic
 *
 * \ingroup AI
 * \ingroup Battle
 */
class LearningBattler : public AIBattler
{
    /**
     * Returns a ReactionPreference struct indicating how the AI would prefer to react
     *
     * \param state Current battle state
     * \param score Score for the current battle situation
     * \return The preferred reaction of the AI
     */
    ReactionPreference getReaction(StateFlags state, Score score);

public:
    /**
     * Default constructor. Initializes internal behavior data
     *
     * \param pplmon Pointer to a vector containing the peoplemon for the AI to use
     * \param itms The items the AI can use
     */
    LearningBattler(std::vector<PeoplemonRef>* pplmon, std::vector<int> itms);

    /**
     * Destructor. Does nothing
     */
    virtual ~LearningBattler();

    /**
     * Does the same thing as AIBattler::getTurn but is overridden to add the adaptive functionality
     *
     * \param op The opponents current peoplemon
     * \return A Turn struct indicating what action to take
     */
    Turn getTurn(PeoplemonRef op); //override because this is more complex

    /**
     * Returns the new index of the peoplemon to switch to
     *
     * \param op The opponent's peoplemon
     * \param g A pointer to the main Game object
     * \return The index of the new active peoplemon, or -1 if all are dead
     */
    int getSwitchPeoplemon(PeoplemonRef op, Game* g);
};

#endif // LEARNINGBATTLER_HPP
