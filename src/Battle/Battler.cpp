#include "Battle/Battler.hpp"
using namespace std;

BattlerFlags::BattlerFlags()
{
    protectUsedLast = false;
    trapped = false;
    ailSafe = false;
    isCharging = false;
    flinched = false;
    subHealth = 0;
    lastMoveUsed = 0;
}

void BattlerFlags::reset()
{
    protectUsedLast = false;
    trapped = false;
    isCharging = false;
    flinched = false;
    subHealth = 0;
    lastMoveUsed = 0;
}

Battler::Battler(vector<PeoplemonRef>* ppl)
{
    lastDamageDealt = 0;
    lastDamageTaken = 0;
    peoplemon = ppl;
    curPeoplemon = 0;
    for (unsigned int i = 0; i<ppl->size(); ++i)
    {
        if (ppl->at(i).curHp>0)
        {
            curPeoplemon = i;
            break;
        }
    }
}

Battler::~Battler()
{
    //dtor
}

Turn Battler::getTurn(PeoplemonRef op, Game* g)
{
    Turn t;
    t.type = Turn::Run;
    return t;
}

int Battler::getCurrentPeoplemon()
{
    return curPeoplemon;
}

bool Battler::statusIsGood()
{
    return true;
}

int Battler::getSwitchPeoplemon(PeoplemonRef op, Game* g)
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

vector<PeoplemonRef>* Battler::getPeoplemon()
{
    return peoplemon;
}

void Battler::recalcStats(Game* g)
{
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        peoplemon->at(i).recalcStats(g);
    }
}

void Battler::healAils(bool a)
{
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        for (int j = a?(0):(1); j<4; ++j)
        {
            peoplemon->at(i).curAils[j] = Peoplemon::None;
        }
    }
}

void Battler::zeroStages()
{
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        peoplemon->at(i).stages.zero();
    }
}

void Battler::setLastDamageDealt(int d)
{
    lastDamageDealt = d;
}

void Battler::setLastDamageTaken(int d)
{
    lastDamageTaken = d;
}
