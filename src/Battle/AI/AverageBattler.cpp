#include "Battle/AI/AverageBattler.hpp"
#include "Globals.hpp"
using namespace std;

AverageBattler::AverageBattler(vector<PeoplemonRef>* pplmon, vector<int> itms) : AIBattler(pplmon,itms)
{
    weights.level = 1.5;
    weights.atkAdv = 2;
    weights.spAtkAdv = 2;
    weights.defAdv = 1.5;
    weights.spDefAdv = 1.5;
    weights.typeAdv = 2;
    weights.superMove = 2;
    weights.spdAdv = 1.3;

    ReactionPreference reg;
    reg.order[0] = ReactionPreference::DirectAttack;
    reg.order[1] = ReactionPreference::AttackingAilment;
    reg.order[2] = ReactionPreference::DefendingAilment;
    reg.order[3] = ReactionPreference::PassiveAttack;
    reg.order[4] = ReactionPreference::PassiveDefense;
    reg.order[5] = ReactionPreference::DirectDefense;
    reg.order[6] = ReactionPreference::Switch;
    reg.order[7] = ReactionPreference::HealingItem;
    reactionSorter.setDefault(reg);
}

AverageBattler::~AverageBattler()
{
    //dtor
}

ReactionPreference AverageBattler::getReaction(StateFlags state, Score score)
{
    reactionSorter.reset();
    if (state.atkAdv)
    {
        reactionSorter.setTop(ReactionPreference::PassiveDefense);
        reactionSorter.setTop(ReactionPreference::DirectAttack);
    }
    if (state.defAdv)
        reactionSorter.setTop(ReactionPreference::PassiveAttack);
    if (state.spdAdv)
        reactionSorter.setTop(ReactionPreference::DirectAttack);
    if (state.likelyToDie)
    {
        reactionSorter.setTop(ReactionPreference::DirectDefense);
        reactionSorter.setTop(ReactionPreference::HealingItem);
    }
    if (state.canKillOpp)
        reactionSorter.setTop(ReactionPreference::DirectAttack);
    if (state.switchAdv && Random(0,100)<50 && turnsSinceSwitch>=3)
        reactionSorter.setTop(ReactionPreference::Switch);
    return reactionSorter.getOrder();
}

int AverageBattler::getSwitchPeoplemon(PeoplemonRef op, Game* g)
{
    bool atLeastOneAlive = false;
    vector<double> scores;
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        scores.push_back(getScore(i,op,g).total);
        if (peoplemon->at(i).curHp==0)
            scores[scores.size()-1] = -1000;
        else
            atLeastOneAlive = true;
    }
    if (!atLeastOneAlive)
        return -1;

    int highestI = -1;
    double highestScore = 0;
    while (highestI==-1)
    {
        for (unsigned int i = 0; i<scores.size(); ++i)
        {
            if (scores[i]>highestScore && Random(0,100)<60)
            {
                highestScore = scores[i];
                highestI = i;
            }
        }
    }
    curPeoplemon = highestI;
    return highestI;
}
