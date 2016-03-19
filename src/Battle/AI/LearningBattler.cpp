#include "Battle/AI/LearningBattler.hpp"
#include "Globals.hpp"
using namespace std;

LearningBattler::LearningBattler(vector<PeoplemonRef>* pplmon, vector<int> itms) : AIBattler(pplmon,itms)
{
    weights.level = 2;
    weights.atkAdv = 2.25;
    weights.spAtkAdv = 2.25;
    weights.defAdv = 2.25;
    weights.spDefAdv = 2.25;
    weights.typeAdv = 4;
    weights.superMove = 3;
    weights.spdAdv = 2.5;
    weights.healthAdv = 2;
    weights.ailmentAdv = 2.5;

    ReactionPreference reg;
    reg.order[0] = ReactionPreference::DirectAttack;
    reg.order[1] = ReactionPreference::AttackingAilment;
    reg.order[2] = ReactionPreference::PassiveDefense;
    reg.order[3] = ReactionPreference::PassiveAttack;
    reg.order[4] = ReactionPreference::DefendingAilment;
    reg.order[5] = ReactionPreference::DirectDefense;
    reg.order[6] = ReactionPreference::HealingItem;
    reg.order[7] = ReactionPreference::Switch;
    reactionSorter.setDefault(reg);
}

LearningBattler::~LearningBattler()
{
    //dtor
}

ReactionPreference LearningBattler::getReaction(StateFlags state, Score score)
{
    reactionSorter.reset();
    if (state.spdAdv) //try and block damage or get in a quick attack
    {
        if (state.defAdv)
        {
            if (Random(0,100)<60)
                reactionSorter.setTop(ReactionPreference::DirectDefense);
            else
                reactionSorter.setTop(ReactionPreference::DirectAttack);
        }
        if (state.atkAdv)
            reactionSorter.setTop(ReactionPreference::AttackingAilment);
    }

    if (score.cHealth>=0.8) //take time to create advantages
    {
        if (state.defAdv)
            reactionSorter.setTop(ReactionPreference::PassiveDefense);
        if (state.atkAdv)
            reactionSorter.setTop(ReactionPreference::PassiveAttack);
        if (state.atkDisAdv)
            reactionSorter.setTop(ReactionPreference::PassiveAttack);
        if (state.defDisAdv)
            reactionSorter.setTop(ReactionPreference::PassiveDefense);
    }

    if (state.switchAdv && turnsSinceSwitch>=1)
        reactionSorter.setTop(ReactionPreference::Switch);

    if (state.canKillOpp && state.likelyToDie)
    {
        if (state.spdAdv)
            reactionSorter.setTop(ReactionPreference::DirectAttack);
        else
        {
            reactionSorter.setTop(ReactionPreference::DefendingAilment);
            reactionSorter.setTop(ReactionPreference::DirectDefense);
            reactionSorter.setTop(ReactionPreference::HealingItem);
        }
    }
    else if (state.likelyToDie)
    {
        reactionSorter.setTop(ReactionPreference::DefendingAilment);
        reactionSorter.setTop(ReactionPreference::DirectDefense);
        reactionSorter.setTop(ReactionPreference::HealingItem);
    }
    else if (state.canKillOpp)
        reactionSorter.setTop(ReactionPreference::DirectAttack);

    return reactionSorter.getOrder();
}

int LearningBattler::getSwitchPeoplemon(PeoplemonRef op, Game* g)
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

    int highestI = 0;
    double highestScore = 0;
    for (unsigned int i = 0; i<scores.size(); ++i)
    {
        if (scores[i]>highestScore)
        {
            highestScore = scores[i];
            highestI = i;
        }
    }
    curPeoplemon = highestI;
    return highestI;
}
