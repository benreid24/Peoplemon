#ifndef PRIORITIZER_HPP
#define PRIORITIZER_HPP

#include "ReactionPreference.hpp"

/**
 * Utility class used by the AI personalities to conveniently order their preferred reactions
 *
 * \ingroup Battle
 * \ingroup AI
 */
class Prioritizer
{
    ReactionPreference order, defaultOrder;

public:
    /**
     * Initializes the order to the default: Direct Attack, AttackingAilment, Defending Ailment, Passive Attack, Passive Defense, Direct Defense, Healing Item, Switch
     */
    Prioritizer();

    /**
     * Sets the default order that the prioritizer should reset to
     *
     * \param d The default order of reactions
     */
    void setDefault(ReactionPreference d);

    /**
     * Resets the internal order to the default order
     */
    void reset();

    /**
     * Sets the new top option by removing it from the order and inserting it at the top, shifting down other options when necessary
     *
     * \param r The option to make the top priority
     */
    void setTop(ReactionPreference::ReactionType r);

    /**
     * Returns the current order of options sorted by priority
     */
    ReactionPreference getOrder();
};

#endif // PRIORITIZER_HPP
