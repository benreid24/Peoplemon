#include "Battle/Battler.hpp"
#include "Battle/AI/AggressiveBattler.hpp"
#include "Battle/AI/AverageBattler.hpp"
#include "Battle/AI/DefensiveBattler.hpp"
#include "Battle/AI/DumbBattler.hpp"
#include "Battle/AI/LearningBattler.hpp"
#include "Battle/AI/RandomBattler.hpp"
#include "Battle/AI/SmartBattler.hpp"
#include "Battle/AI/SuicidalBattler.hpp"
#include "Globals.hpp"
using namespace std;

BattlerFlags::BattlerFlags()
{
    reset();
}

void BattlerFlags::reset()
{
    protectUsedLast = false;
    isCharging = false;
    subHealth = 0;
    lastMoveUsed = 0;
    ballHandled = false;
    ballSet = false;
    ballIsUp = false;
    switchAfterMove = false;
    enduredLastTurn = false;
    endureThisTurn = false;
    spikesApplied = 0;
    healNextPeoplemon = false;
    roarUsed = false;
    move64Hit = false;
    encoreHit = false;
    encoreMoveId = -1;
    encoreTurnsLeft = 0;
    deathTurnCounter = -1;
    koRevive = false;
    koReviveHp = 0;
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
    zeroStages();
    isWild = false;
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

void Battler::setCurrentPeoplemon(int c)
{
	curPeoplemon = c;
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

int Battler::getRandomFaintedPeoplemon(bool updateCurrent) {
    vector<int> validIndices;
    for (unsigned int i = 0; i<peoplemon->size(); ++i) {
        if (peoplemon->at(i).curHp==0)
            validIndices.push_back(i);
    }
    if (validIndices.size()==0)
        return -1;

    int id = validIndices[Random(0, validIndices.size()-1)];
    if (updateCurrent)
        curPeoplemon = id;
    return id;
}

vector<PeoplemonRef>* Battler::getPeoplemon()
{
    return peoplemon;
}

void Battler::recalcStats(Game* g, bool resetAbilities)
{
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        peoplemon->at(i).recalcStats(g, resetAbilities);
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

Battler* createBattler(int aiType, vector<PeoplemonRef>* pplmon, vector<int> items)
{
	switch (aiType)
	{
	case 0:
		return new RandomBattler(pplmon,items);
	case 1:
		return new DumbBattler(pplmon,items);
	case 2:
		return new SmartBattler(pplmon,items);
	case 3:
		return new SuicidalBattler(pplmon,items);
	case 4:
		return new AggressiveBattler(pplmon,items);
	case 5:
		return new DefensiveBattler(pplmon,items);
	case 6:
		return new AverageBattler(pplmon,items);
	default:
		return new LearningBattler(pplmon,items);
	}
}

void Battler::setIsWild(bool wild)
{
	isWild = wild;
}

double Battler::getBroPower() {
    vector<int> seenIds;
    double power = 1;

    for (unsigned int i = 0; i<peoplemon->size(); ++i) {
        if (peoplemon->at(i).curAbility==Peoplemon::TotalBro) {
            if (find(seenIds.begin(), seenIds.end(), peoplemon->at(i).id) != seenIds.end()) {
                power *= 1.5;
                seenIds.push_back(peoplemon->at(i).id);
            }
        }
    }

    return power;
}
