#include "Battle/AI/SuicidalBattler.hpp"
#include "Globals.hpp"
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

int SuicidalBattler::getSwitchPeoplemon(PeoplemonRef op, Game* g)
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

	int r = -1;
	while (r==-1)
	{
		int i = Random(0,scores.size()-1);
		if (scores[i]!=-1000)
			r = i;
	}
	return r;
}
