#include "Battle/AI/SuicidalBattler.hpp"
using namespace std;

SuicidalBattler::SuicidalBattler(vector<PeoplemonRef>* pplmon, vector<int> itms) : AIBattler(pplmon,itms)
{
    weights.level = 0;
    weights.atkAdv = 3;
    weights.spAtkAdv = 3;
    weights.defAdv = 0;
    weights.spDefAdv = 0;
    weights.typeAdv = 0;
    weights.superMove = 0;
    weights.spdAdv = 0;

    ReactionPreference reg;
    reg.order[0] = ReactionPreference::DirectAttack;
    reg.order[1] = ReactionPreference::PassiveAttack;
    reg.order[2] = ReactionPreference::AttackingAilment;
    reg.order[3] = ReactionPreference::DefendingAilment;
    reg.order[4] = ReactionPreference::PassiveDefense;
    reg.order[5] = ReactionPreference::DirectDefense;
    reg.order[6] = ReactionPreference::Switch;
    reg.order[7] = ReactionPreference::HealingItem;
    reactionSorter.setDefault(reg);
}

SuicidalBattler::~SuicidalBattler()
{
    //dtor
}

ReactionPreference SuicidalBattler::getReaction(StateFlags state, Score score)
{
    return reactionSorter.getOrder();
}
