#include "Battle/AI/AggressiveBattler.hpp"
#include "Globals.hpp"
using namespace std;

AggressiveBattler::AggressiveBattler(vector<PeoplemonRef>* pplmon, vector<int> itms) : AIBattler(pplmon,itms)
{
    weights.level = 1.5;
    weights.atkAdv = 3;
    weights.spAtkAdv = 3;
    weights.defAdv = 0.5;
    weights.spDefAdv = 0.5;
    weights.typeAdv = 2;
    weights.superMove = 2;
    weights.spdAdv = 2;

    ReactionPreference reg;
    reg.order[0] = ReactionPreference::DirectAttack;
    reg.order[1] = ReactionPreference::AttackingAilment;
    reg.order[2] = ReactionPreference::PassiveAttack;
    reg.order[3] = ReactionPreference::DefendingAilment;
    reg.order[4] = ReactionPreference::PassiveDefense;
    reg.order[5] = ReactionPreference::DirectDefense;
    reg.order[6] = ReactionPreference::Switch;
    reg.order[7] = ReactionPreference::HealingItem;
    reactionSorter.setDefault(reg);
}

AggressiveBattler::~AggressiveBattler()
{
    //dtor
}

ReactionPreference AggressiveBattler::getReaction(StateFlags state, Score score)
{
    reactionSorter.reset();

    if (state.canKillOpp)
        return reactionSorter.getOrder();
    if (state.likelyToDie)
    {
        if (Random(0,100)<80)
            reactionSorter.setTop(ReactionPreference::HealingItem);
    }

    if (state.switchAdv)
    {
        if (Random(0,100)<turnsSinceSwitch*20) //100% chance by 5 turns
            reactionSorter.setTop(ReactionPreference::Switch);
    }

    if (Random(0,100)<20)
        reactionSorter.setTop(ReactionPreference::PassiveAttack);

    return reactionSorter.getOrder();
}
