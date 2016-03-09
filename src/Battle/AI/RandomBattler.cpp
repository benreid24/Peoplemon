#include "Battle/AI/RandomBattler.hpp"
#include "Globals.hpp"
using namespace std;

RandomBattler::RandomBattler(vector<PeoplemonRef>* ppl, vector<int> itms) : AIBattler(ppl,itms)
{
    //ctor
}

RandomBattler::~RandomBattler()
{
    //dtor
}

ReactionPreference RandomBattler::getReaction(StateFlags state, Score score)
{
    ReactionPreference ret;
    vector<ReactionPreference::ReactionType> popcorn({ReactionPreference::DirectAttack,ReactionPreference::PassiveAttack,ReactionPreference::DirectDefense,ReactionPreference::PassiveDefense,ReactionPreference::AttackingAilment,ReactionPreference::DefendingAilment,ReactionPreference::HealingItem});

    for (int i = 0; i<7; ++i)
    {
        int j = Random(0,popcorn.size()-1);
        ret.order[i] = popcorn[j];
        popcorn.erase(popcorn.begin()+j);
    }
    ret.order[7] = ReactionPreference::Switch;
    return ret;
}

int RandomBattler::getSwitchPeoplemon(PeoplemonRef op, Game* g)
{
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        if (peoplemon->at(i).curHp>0)
        {
            curPeoplemon = i;
            return i;
        }
    }
    return -1;
}
