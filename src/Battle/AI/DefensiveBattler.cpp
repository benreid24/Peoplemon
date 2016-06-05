#include "Battle/AI/DefensiveBattler.hpp"
#include "Globals.hpp"
using namespace std;

DefensiveBattler::DefensiveBattler(vector<PeoplemonRef>* pplmon, vector<int> itms) : AIBattler(pplmon,itms)
{
    weights.level = 1.5;
    weights.atkAdv = 0.5;
    weights.spAtkAdv = 0.5;
    weights.defAdv = 3;
    weights.spDefAdv = 3;
    weights.typeAdv = 2;
    weights.superMove = 1.25;
    weights.spdAdv = 2;

    ReactionPreference reg;
    reg.order[0] = ReactionPreference::DefendingAilment;
    reg.order[1] = ReactionPreference::PassiveDefense;
    reg.order[2] = ReactionPreference::DirectAttack;
    reg.order[3] = ReactionPreference::AttackingAilment;
    reg.order[4] = ReactionPreference::PassiveAttack;
    reg.order[5] = ReactionPreference::DirectDefense;
    reg.order[6] = ReactionPreference::HealingItem;
    reg.order[7] = ReactionPreference::Switch;
    reactionSorter.setDefault(reg);
}

DefensiveBattler::~DefensiveBattler()
{
    //dtor
}

ReactionPreference DefensiveBattler::getReaction(StateFlags state, Score score)
{
    reactionSorter.reset();
    if (state.likelyToDie)
    {
        reactionSorter.setTop(ReactionPreference::DirectDefense);
        reactionSorter.setTop(ReactionPreference::HealingItem);
    }
    else if (state.defDisAdv)
        reactionSorter.setTop(ReactionPreference::PassiveDefense);
    else
    {
        if (Random(0,100)<50)
            reactionSorter.setTop(ReactionPreference::PassiveDefense);
        if (Random(0,100)<40)
            reactionSorter.setTop(ReactionPreference::DirectAttack);
        if (Random(0,100)<10)
            reactionSorter.setTop(ReactionPreference::DirectDefense);
    }
    return reactionSorter.getOrder();
}
