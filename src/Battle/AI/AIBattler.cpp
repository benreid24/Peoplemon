#include "Battle/AI/AIBattler.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
using namespace std;

AIBattler::AIBattler(vector<PeoplemonRef>* ppl, vector<int> itms) : Battler(ppl)
{
    turnsSinceSwitch = 0;
    items = itms;
    weights.ailmentAdv = 1;
    weights.atkAdv = 1;
    weights.cHealth = 1;
    weights.defAdv = 1;
    weights.healthAdv = 1;
    weights.level = 1;
    weights.spAtkAdv = 1;
    weights.spdAdv = 1;
    weights.spDefAdv = 1;
    weights.superMove = 1;
    weights.typeAdv = 1;
}

AIBattler::~AIBattler()
{
    //dtor
}

Score AIBattler::getScore(int ppl, PeoplemonRef op, Game* g)
{
    Score s;

    PeoplemonRef cur = peoplemon->at(ppl);
    s.level = double(cur.level-op.level)/10;
    s.cHealth = double(cur.curHp)/double(cur.stats.hp);
    s.healthAdv = (s.cHealth - double(op.curHp)/double(op.stats.hp))*2;
    s.atkAdv = double(cur.stats.atk)/double(3*op.stats.atk);
    s.defAdv = double(cur.stats.def)/double(3*op.stats.def);
    s.spAtkAdv = double(cur.stats.spAtk)/double(3*op.stats.spDef);
    s.spDefAdv = double(cur.stats.spDef)/double(3*op.stats.spAtk);
    s.spdAdv = double(cur.stats.spd-op.stats.spd)/15;
    //needs type, supermove, ailment

    s.limit();
    s.sum(weights);

    return s;
}

StateFlags AIBattler::getState(Score sc, PeoplemonRef op, Game* g)
{
    StateFlags state;
    state.atkAdv = (sc.atkAdv>0.75) || (sc.spAtkAdv>0.75);
    state.atkDisAdv = (sc.atkAdv<0.35) || (sc.spAtkAdv<0.35);
    state.defAdv = (sc.defAdv>0.75) || (sc.spDefAdv>0.75);
    state.defDisAdv = (sc.defAdv<0.35) || (sc.spDefAdv<0.35);
    state.canKillOpp = false;
    if (lastDamageDealt>=op.curHp)
        state.canKillOpp = true;
    state.likelyToDie = false;
    if (lastDamageTaken>=peoplemon->at(curPeoplemon).curHp)
        state.likelyToDie = true;
    state.spdAdv = (sc.spdAdv>0.5);
    state.switchAdv = false;
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        if (i!=unsigned(curPeoplemon))
        {
            Score s = getScore(i,op,g);
            if (s.total>sc.total)
                state.switchAdv = true;
        }
    }
    return state;
}

Turn AIBattler::getTurn(PeoplemonRef opp, Game* g)
{
    Score score = getScore(curPeoplemon,opp,g);
    StateFlags state = getState(score,opp,g);
    ReactionPreference action = getReaction(state,score);
    Turn turn;

    turnsSinceSwitch++;
    for (int i = 0; i<8; ++i)
    {
        if (action.order[i]==ReactionPreference::HealingItem && items.size()>0)
        {
            turn.type = Turn::Item;
            turn.id = items[0];
            items.erase(items.begin());
            return turn;
        }
        else if (action.order[i]==ReactionPreference::Switch)
        {
            int bestIndex = 0;
            double bestScore = 0;
            for (unsigned int j = 0; j<peoplemon->size(); ++j)
            {
                if (j!=unsigned(curPeoplemon) && peoplemon->at(j).curHp>0 && getScore(j,opp,g).total>bestScore)
                {
                    bestIndex = j;
                    bestScore = getScore(j,opp,g).total;
                }
            }
            turn.type = Turn::Switch;
            turn.id = curPeoplemon;
            curPeoplemon = bestIndex;
            turnsSinceSwitch = 0;
            return turn;
        }
        else
        {
            int bestMoveId = 0;
            double bestMoveScore = 0;
            for (int j = 0; j<4; ++j)
            {
                int moveId = peoplemon->at(curPeoplemon).moves[j].id;
                if (moveId!=0)
                {
                    if (g->moveList[moveId].classification==action.order[i] && (g->moveList[moveId].damageScore(score.atkAdv, score.spAtkAdv, g->peoplemonList[peoplemon->at(curPeoplemon).id].type, g->peoplemonList[opp.id].type)+g->moveList[moveId].effectScore)>bestMoveScore)
                    {
                        bestMoveScore = (g->moveList[moveId].damageScore(score.atkAdv, score.spAtkAdv, g->peoplemonList[peoplemon->at(curPeoplemon).id].type, g->peoplemonList[opp.id].type)+g->moveList[moveId].effectScore);
                        bestMoveId = moveId;
                    }
                }
            }
            if (bestMoveId!=0)
            {
                turn.type = Turn::Move;
                turn.id = bestMoveId;
                return turn;
            }
        }
    }

    cout << "Critical error, AI move selection failure\n";
    int* crash = NULL;
    for (int x = 0; true; ++x)
    {
        crash[x] = 0;
    }
}

ReactionPreference AIBattler::getReaction(StateFlags state, Score score)
{
    vector<ReactionPreference::ReactionType> left({ReactionPreference::DirectAttack,ReactionPreference::PassiveAttack,ReactionPreference::DirectDefense,ReactionPreference::PassiveDefense,ReactionPreference::AttackingAilment,ReactionPreference::DefendingAilment});
    ReactionPreference reaction;

    int i = 0;
    while (left.size()>0)
    {
        int j = Random(0,left.size()-1);
        reaction.order[i] = left[j];
        left.erase(left.begin()+j);
        ++i;
    }
    reaction.order[i] = ReactionPreference::HealingItem;
    reaction.order[i+1] = ReactionPreference::Switch;

    return reaction;
}

int AIBattler::getSwitchPeoplemon(PeoplemonRef op, Game* g)
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
