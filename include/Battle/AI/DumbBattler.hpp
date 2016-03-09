#ifndef DUMBBATTLER_HPP
#define DUMBBATTLER_HPP

#include "AIBattler.hpp"

/**
 * Defines an AI type that plays poorly on purpose. It will make strategical errors intentionally
 *
 * \ingroup AI
 * \ingroup Battle
 */
class DumbBattler : public AIBattler
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
    DumbBattler(std::vector<PeoplemonRef>* pplmon, std::vector<int> itms);

    /**
     * Destructor. Does nothing
     */
    virtual ~DumbBattler();

    /**
     * Returns the new index of the peoplemon to switch to
     *
     * \param op The opponent's peoplemon
     * \param g A pointer to the main Game object
     * \return The index of the new active peoplemon, or -1 if all are dead
     */
    int getSwitchPeoplemon(PeoplemonRef op, Game* g);
};

#endif // DUMBBATTLER_HPP
