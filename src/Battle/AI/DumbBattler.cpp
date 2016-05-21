#include "Battle/AI/DumbBattler.hpp"
#include "Globals.hpp"
using namespace std;

DumbBattler::DumbBattler(vector<PeoplemonRef>* pplmon, vector<int> itms) : AIBattler(pplmon,itms)
{
    weights.level = 1;
    weights.atkAdv = 1;
    weights.spAtkAdv = 1;
    weights.defAdv = 1;
    weights.spDefAdv = 1;
    weights.typeAdv = 1;
    weights.superMove = 1;
    weights.spdAdv = 1;

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

DumbBattler::~DumbBattler()
{
    //dtor
}

ReactionPreference DumbBattler::getReaction(StateFlags state, Score score)
{
    reactionSorter.reset();
    if (state.defAdv)
    {
        reactionSorter.setTop(ReactionPreference::PassiveAttack);
        reactionSorter.setTop(ReactionPreference::DirectAttack);
    }
    if (state.atkAdv)
    {
        reactionSorter.setTop(ReactionPreference::PassiveDefense);
        reactionSorter.setTop(ReactionPreference::DirectDefense);
    }
    if (state.spdAdv)
        reactionSorter.setTop(ReactionPreference::PassiveAttack);
    if (state.likelyToDie)
    {
        if (Random(0,100)<40)
            reactionSorter.setTop(ReactionPreference::HealingItem);
        else
            reactionSorter.setTop(ReactionPreference::DirectAttack);
    }
    if (state.canKillOpp)
    {
        reactionSorter.setTop(ReactionPreference::DirectAttack);
        reactionSorter.setTop(ReactionPreference::DirectDefense);
        reactionSorter.setTop(ReactionPreference::PassiveAttack);
    }
    return reactionSorter.getOrder();
}

int DumbBattler::getSwitchPeoplemon(PeoplemonRef op, Game* g)
{
    bool atLeastOneAlive = false;
    vector<double> scores;
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        scores.push_back(getScore(i,op,g).total);
        if (peoplemon->at(i).curHp==0)
            scores[scores.size()-1] = -1004;
        else
            atLeastOneAlive = true;
    }
    if (!atLeastOneAlive)
        return -1;

    int lowestI = 10000;
    double lowestScore = -10000;
    for (unsigned int i = 0; i<scores.size(); ++i)
    {
        if (scores[i]<lowestScore && scores[i]>-1000)
        {
            lowestScore = scores[i];
            lowestI = i;
        }
    }
    curPeoplemon = lowestI;
    return lowestI;
}
