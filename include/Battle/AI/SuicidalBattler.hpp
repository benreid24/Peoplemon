#ifndef SUICIDALBATTLER_HPP
#define SUICIDALBATTLER_HPP

#include "AIBattler.hpp"

/**
 * Defines an AI type that will intentionally make moves it knows to be harmful to itself
 *
 * \ingroup AI
 * \ingroup Battle
 */
class SuicidalBattler : public AIBattler
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
    SuicidalBattler(std::vector<PeoplemonRef>* pplmon, std::vector<int> itms);

    /**
     * Destructor. Does nothing
     */
    virtual ~SuicidalBattler();

    /**
     * Returns the new index of the peoplemon to switch to
     *
     * \param op The opponent's peoplemon
     * \param g A pointer to the main Game object
     * \return The index of the new active peoplemon, or -1 if all are dead
     */
    int getSwitchPeoplemon(PeoplemonRef op, Game* g);
};

#endif // SUICIDALBATTLER_HPP
