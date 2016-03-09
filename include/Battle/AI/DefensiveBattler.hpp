#ifndef DEFENSIVEBATTLER_HPP
#define DEFENSIVEBATTLER_HPP

#include "AIBattler.hpp"

/**
 * Defines an AI type that plays defensively. It favors defensive moves and healing items in all situations
 *
 * \ingroup AI
 * \ingroup Battle
 */
class DefensiveBattler : public AIBattler
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
    DefensiveBattler(std::vector<PeoplemonRef>* pplmon, std::vector<int> itms);

    /**
     * Destructor. Does nothing
     */
    virtual ~DefensiveBattler();
};

#endif // DEFENSIVEBATTLER_HPP
