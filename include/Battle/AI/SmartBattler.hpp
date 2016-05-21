#ifndef SMARTBATTLER_HPP
#define SMARTBATTLER_HPP

#include "AIBattler.hpp"

/**
 * Defines an AI type that will make the "best" move based on the current battle state
 *
 * \ingroup AI
 * \ingroup Battle
 */
class SmartBattler : public AIBattler
{
    int advTurns;

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
    SmartBattler(std::vector<PeoplemonRef>* pplmon, std::vector<int> itms);

    /**
     * Destructor. Does nothing
     */
    virtual ~SmartBattler();
};

#endif // SMARTBATTLER_HPP
