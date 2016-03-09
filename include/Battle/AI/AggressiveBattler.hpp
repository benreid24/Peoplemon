#ifndef AGGRESSIVEBATTLER_HPP
#define AGGRESSIVEBATTLER_HPP

#include "AIBattler.hpp"

/**
 * This class defines the AI behavior for an aggressive AI
 *
 * \ingroup AI
 * \ingroup Battle
 */
class AggressiveBattler : public AIBattler
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
    AggressiveBattler(std::vector<PeoplemonRef>* pplmon, std::vector<int> itms);

    /**
     * Destructor. Does nothing
     */
    virtual ~AggressiveBattler();
};

#endif // AGGRESSIVEBATTLER_HPP
