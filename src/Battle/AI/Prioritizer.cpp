#include "Battle/AI/Prioritizer.hpp"

Prioritizer::Prioritizer()
{
    order.order[0] = ReactionPreference::DirectAttack;
    order.order[1] = ReactionPreference::AttackingAilment;
    order.order[2] = ReactionPreference::DefendingAilment;
    order.order[3] = ReactionPreference::PassiveAttack;
    order.order[4] = ReactionPreference::PassiveDefense;
    order.order[5] = ReactionPreference::DirectDefense;
    order.order[6] = ReactionPreference::HealingItem;
    order.order[7] = ReactionPreference::Switch;
    defaultOrder = order;
}

void Prioritizer::setTop(ReactionPreference::ReactionType r)
{
    for (int i = 0; i<8; ++i)
    {
        if (order.order[i]==r)
        {
            for (int j = i; j>0; --j)
            {
                order.order[j] = order.order[j-1];
            }
            order.order[0] = r;
            break;
        }
    }
}

ReactionPreference Prioritizer::getOrder()
{
    return order;
}

void Prioritizer::setDefault(ReactionPreference d)
{
    defaultOrder = d;
}

void Prioritizer::reset()
{
    order = defaultOrder;
}
