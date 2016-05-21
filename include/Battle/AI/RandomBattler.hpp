#ifndef RANDOMBATTLER_HPP
#define RANDOMBATTLER_HPP

#include "AIBattler.hpp"

/**
 * Defines an AI type that will simply make random moves every turn, with healing and switching always being the last choices
 *
 * \ingroup AI
 * \ingroup Battle
 */
class RandomBattler : public AIBattler
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
    RandomBattler(std::vector<PeoplemonRef>* pplmon, std::vector<int> itms);

    /**
     * Destructor. Does nothing
     */
    virtual ~RandomBattler();

    /**
     * Returns the new index of the peoplemon to switch to
     *
     * \param op The opponent's peoplemon
     * \param g A pointer to the main Game object
     * \return The index of the new active peoplemon, or -1 if all are dead
     */
    int getSwitchPeoplemon(PeoplemonRef op, Game* g);
};

#endif // RANDOMBATTLER_HPP
