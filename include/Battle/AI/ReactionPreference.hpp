#ifndef REACTIONPREFERENCE_HPP
#define REACTIONPREFERENCE_HPP

/**
 * This structure is for an AI to indicate what type of actions it prefers in its given situation
 *
 * \ingroup AI
 * \ingroup Battle
 */
struct ReactionPreference
{
    /**
     * The type of reaction
     */
    enum ReactionType
    {
        DirectAttack = 0,
        PassiveAttack = 1,
        DirectDefense = 2,
        PassiveDefense = 3,
        AttackingAilment = 4,
        DefendingAilment = 5,
        HealingItem = 6,
        Switch = 7
    };

    ReactionType order[8];
};

#endif // REACTIONPREFERENCE_HPPP
