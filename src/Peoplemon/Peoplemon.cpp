#include "Peoplemon/Peoplemon.hpp"
#include "Util/File.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include <cmath>
using namespace std;
using namespace sf;

const pair<string,string> Peoplemon::abilityTexts[43] = {make_pair("None",""),
                                                        make_pair("Board Game Master", "The opposing Peoplemon cannot switch out"),
                                                        make_pair("Chillaxed", "Cannot receive critical hits"),
                                                        make_pair("Classy", "Cannot be Frustrated"),
                                                        make_pair("Goon", "Enemy Peoplemon take damage when they attack physically"),
                                                        make_pair("Quick Draw", "Attacks first on the first turn of every battle"),
                                                        make_pair("Always Friendly", "Is cured of all ailments when switched out"),
                                                        make_pair("Skipped Slot", "Id #7 is missing, it's Chris' fault"),
                                                        make_pair("Impulse Buy", "May purchase items at random in stores"),
                                                        make_pair("Run Away", "Always able to get away from battle"),
                                                        make_pair("Mr. Extra", "Occasionally finds items when exploring and pockets them"),
                                                        make_pair("Beefed Up", "Athletic type moves do more damage when below 25% HP"),
                                                        make_pair("Reserved", "Quiet type moves do more damage when below 25% HP"),
                                                        make_pair("Duke of Jokes", "Joke based moves do 50% more damage"),
                                                        make_pair("Engaging", "Moves that otherwise would not affect enemy Peoplemon become Not Very Effective instead"),
                                                        make_pair("Snack Share", "Chance to give inflicted ailments to the attacker as well"),
                                                        make_pair("Derp Derp", "Chance to Confuse opponent when taking damage"),
                                                        make_pair("Klutz", "Chance to drop hold item before battle, making it unusable for the battle"),
                                                        make_pair("Gender Bend", "Changes the gender of enemy Peoplemon"),
                                                        make_pair("Garbage", "This Peoplemon is garbage and has no special skills"),
                                                        make_pair("Bike Mechanic", "Can fix broken bikes"),
                                                        make_pair("Sidetrack Teach", "Teach based moves have a chance to Distract opponents"),
                                                        make_pair("No Joke Teach", "Takes no damage from Joke based moves if a Teach based move was used"),
                                                        make_pair("Fiery Teach", "Teach based moves have a chance to raise Special Attack"),
                                                        make_pair("Experienced Teach", "Cannot be K.O'd if a Teach based move was used"),
                                                        make_pair("New Teach", "Teach based moves used on the first turn of a battle have higher speed"),
                                                        make_pair("Doze Off", "Teach based moves have a chance to make this Peoplemon fall asleep"),
                                                        make_pair("Douse Flames", "Has a chance to reduce opponent Attack after taking physical damage"),
                                                        make_pair("Flirty", "Has a chance to reduce opponent Special Attack after taking special damage"),
                                                        make_pair("Undying Faith", "Has a chance to survive a lethal attack"),
                                                        make_pair("Too Cool", "Cannot be Stunned"),
                                                        make_pair("Fake Study", "If an attack misses the move used is not revealed to the opponent"),
                                                        make_pair("Alcoholic", "Consumes Alcohol based items at the beginning of each turn"),
                                                        make_pair("Total Bro", "Increases Bro Power of other Bros in your party"),
                                                        make_pair("Total Mom", "Restores some PP of the next Peoplemon when K.O'd"),
                                                        make_pair("Can't Swim", "Has a chance to decrease opponents Attack when hit with a physical move"),
                                                        make_pair("All Nighter", "Cannot be put to sleep"),
                                                        make_pair("Ailment Saturated", "Can only ever have one Ailment at a time"),
                                                        make_pair("Adamant", "Can't be forced to switch out of battle by an opponent"),
                                                        make_pair("Absolute Pitch", "Super Effective moves do additional damage, but Not Very Effective moves do even less damage"),
                                                        make_pair("GameMaker Virus", "Eliminates half of the PP of any moves that it gets hit by"),
                                                        make_pair("Snapshot", "Cannot take Super Effective damage from the same move twice in a row"),
                                                        make_pair("Get Baked", "Automatically self-destructs if at 20% or less HP")};


double Peoplemon::getSTAB(Type atk, Type mv)
{
	if (atk==mv ||
       (atk==FunnyPartyAnimal && (mv==PartyAnimal || mv==Funny)) ||
       (atk==IntelligentAthletic && (mv==Intelligent || mv==Athletic)) ||
       (atk==IntelligentNormal && (mv==Normal || mv==Intelligent)) ||
       (atk==NormalQuiet && (mv==Normal || mv==Quiet)) ||
       (atk==AwkwardFunny && (mv==Awkward || mv==Funny)) ||
       (atk==IntelligentFunny && (mv==Intelligent || mv==Funny)) ||
       (atk==AthleticNormal && (mv==Athletic || mv==Normal)) ||
       (atk==FunnyNormal && (mv==Funny || mv==Normal)) ||
       (atk==NormalAwkward && (mv==Normal || mv==Awkward)) ||
       (atk==QuietAthletic && (mv==Quiet || mv==Athletic)) ||
       (atk==IntelligentAwkward && (mv==Intelligent || mv==Awkward)))
		return 1.5;
    return 1;
}

double Peoplemon::getEffectivenessMultiplier(Type mv, Type def)
{
	return typeMultipliers[mv][def];
}

double Peoplemon::getStatMultiplier(int s, bool isCrit)
{
    if (isCrit)
    {
        if (s>0)
            return pow(2,s);
        return 1.0;
    }
    else
    {
        double num = (s<=0)?(2):(2+s);
        double den = (s<=0)?(2-s):(2);
        return num/den;
    }
}

PeoplemonRef::PeoplemonRef()
{
	level = 0;
	curXp = 0;
	nextLvlXp = 0;
	holdItem = 0;
	curHp = 0;
	turnsWithAil = 0;
	turnsConfused = 0;
	curAbility = Peoplemon::NoAbility;
	turnsUntilWake = -1;
	lastSuperId = 0;
	stages.zero();
}

void PeoplemonRef::save(File* file)
{
    file->writeString(name);
    file->write<uint16_t>(id);
    file->write<uint16_t>(level);
    file->write<uint32_t>(curXp);
    file->write<uint32_t>(nextLvlXp);
    file->write<uint16_t>(curHp);
    file->write<uint8_t>(curAils[0]);
    file->write<uint16_t>(holdItem);
    for (int i = 0; i<4; ++i)
    {
        file->write<uint16_t>(moves[i].id);
        file->write<uint16_t>(moves[i].curPp);
    }
    file->write<uint16_t>(ivs.hp);
    file->write<uint16_t>(ivs.atk);
    file->write<uint16_t>(ivs.def);
    file->write<uint16_t>(ivs.spAtk);
    file->write<uint16_t>(ivs.spDef);
    file->write<uint16_t>(ivs.acc);
    file->write<uint16_t>(ivs.evade);
    file->write<uint16_t>(ivs.spd);
    file->write<uint16_t>(ivs.crit);
    file->write<uint16_t>(evs.hp);
    file->write<uint16_t>(evs.atk);
    file->write<uint16_t>(evs.def);
    file->write<uint16_t>(evs.spAtk);
    file->write<uint16_t>(evs.spDef);
    file->write<uint16_t>(evs.acc);
    file->write<uint16_t>(evs.evade);
    file->write<uint16_t>(evs.spd);
    file->write<uint16_t>(evs.crit);

}

void PeoplemonRef::load(Game* g, File* file)
{
    name = file->getString();
    id = file->get<uint16_t>();
    level = file->get<uint16_t>();
    curXp = file->get<uint32_t>();
    nextLvlXp = file->get<uint32_t>();
    curHp = file->get<uint16_t>();
    curAils[0] = Peoplemon::Ailment(file->get<uint8_t>());
    for (int i = 1; i<4; ++i)
        curAils[i] = Peoplemon::None;
    holdItem = file->get<uint16_t>();
    for (int i = 0; i<4; ++i)
    {
        moves[i].id = file->get<uint16_t>();
        moves[i].curPp = file->get<uint16_t>();
        if (moves[i].curPp>g->moveList[moves[i].id].pp)
			moves[i].curPp = g->moveList[moves[i].id].pp;
    }
    ivs.hp = file->get<uint16_t>();
    ivs.atk = file->get<uint16_t>();
    ivs.def = file->get<uint16_t>();
    ivs.spAtk = file->get<uint16_t>();
    ivs.spDef = file->get<uint16_t>();
    ivs.acc = file->get<uint16_t>();
    ivs.evade = file->get<uint16_t>();
    ivs.spd = file->get<uint16_t>();
    ivs.crit = file->get<uint16_t>();
    evs.hp = file->get<uint16_t>();
    evs.atk = file->get<uint16_t>();
    evs.def = file->get<uint16_t>();
    evs.spAtk = file->get<uint16_t>();
    evs.spDef = file->get<uint16_t>();
    evs.acc = file->get<uint16_t>();
    evs.evade = file->get<uint16_t>();
    evs.spd = file->get<uint16_t>();
    evs.crit = file->get<uint16_t>();
    if (name.size()==0)
        name = g->peoplemonList[id].name;

    recalcStats(g);
    if (curHp==999)
		curHp = stats.hp;
}

void PeoplemonRef::load(Game* g, string file)
{
    File t(file);
    load(g, &t);
}

void PeoplemonRef::recalcStats(Game* g, bool resetAbility)
{
    stats.hp = (g->peoplemonList[id].baseStats.hp*2+ivs.hp+evs.hp/4)*level/100+10+level;
    stats.atk = (g->peoplemonList[id].baseStats.atk*2+ivs.atk+evs.atk/4)*level/100+5;
    stats.atk *= Peoplemon::getStatMultiplier(stages.atk,false);
    stats.def = (g->peoplemonList[id].baseStats.def*2+ivs.def+evs.def/4)*level/100+5;
    stats.def *= Peoplemon::getStatMultiplier(stages.def,false);
    stats.acc = 100;
    stats.acc *= Peoplemon::getStatMultiplier(stages.acc,false);
    stats.spAtk = (g->peoplemonList[id].baseStats.spAtk*2+ivs.spAtk+evs.spAtk/4)*level/100+5;
    stats.spAtk *= Peoplemon::getStatMultiplier(stages.spAtk,false);
    stats.spDef = (g->peoplemonList[id].baseStats.spDef*2+ivs.spDef+evs.spDef/4)*level/100+5;
    stats.spDef *= Peoplemon::getStatMultiplier(stages.spDef,false);
    stats.evade = 100;
    stats.evade *= Peoplemon::getStatMultiplier(stages.evade,false);
    stats.crit = 6;
    stats.crit *= Peoplemon::getStatMultiplier(stages.crit,true);
    stats.spd = (g->peoplemonList[id].baseStats.spd*2+ivs.spd+evs.spd/4)*level/100+5;
    stats.spd *= Peoplemon::getStatMultiplier(stages.spd,false);
    int lType = g->peoplemonList[id].xpGroup;
    if (lType==0)
        nextLvlXp = 5*pow(level,3)/4;
    else if (lType==1)
        nextLvlXp = pow(level,3);
    else
        nextLvlXp = 4*pow(level,3)/5;

    if (resetAbility) {
        curAbility = g->peoplemonList[id].specialAbilityId;
        lastSuperId = 0;
    }
}

void PeoplemonRef::awardEVs(Game* g, Stats evAward)
{
    if (level!=100 && evs.sum()<510)
    {
        evs.hp += evAward.hp;
        evs.atk += evAward.atk;
        evs.def += evAward.def;
        evs.spAtk += evAward.spAtk;
        evs.spDef += evAward.spDef;
        evs.spd += evAward.spd;
        evs.cap(252);
        recalcStats(g);
    }
}

void PeoplemonRef::addPassiveAilment(Peoplemon::Ailment a)
{
	for (int i = 0; i<4; ++i)
	{
		if (curAils[i]==a)
			return;
	}
    for (int i = 1; i<4; ++i)
    {
        if (curAils[i]==Peoplemon::None)
        {
            curAils[i] = a;
            return;
        }
    }
}

void PeoplemonRef::removePassiveAilment(Peoplemon::Ailment ail)
{
    for (int i = 1; i<4; ++i)
    {
        if (curAils[i]==ail)
        {
            curAils[i] = Peoplemon::None;
            return;
        }
    }
}

bool PeoplemonRef::hasAilment(Peoplemon::Ailment ail)
{
    for (int i = 0; i<4; ++i)
    {
        if (curAils[i]==ail)
            return true;
    }
    return false;
}

bool PeoplemonRef::hasAtLeastOneAilment()
{
	for (int i = 0; i<4; ++i)
	{
		if (curAils[i]!=Peoplemon::None)
			return true;
	}
	return false;
}

double PeoplemonRef::getBallBonus(Game* g, int b, int t, int l)
{
	if ((id==123 || id==124) && b!=17)
		return 0;

    switch (b)
    {
	case 5:
		return 0.25;
	case 6:
		return 1;
	case 7:
		return 1.5;
	case 8:
		return 2;
	case 9:
		return 2;
	case 10:
		return double(t+10)/10;
	case 11:
		return (g->peoplemonList[id].type==Type::Intelligent)?(2.5):(1);
	case 12:
		return (t==0)?(4):(1);
	case 13:
		return 1;
	case 14:
		if (level>=l)
			return 1;
		else if (level*4<l)
			return 8;
		else if (level*2<l)
			return 4;
		return 2;
	case 15:
		return 9999999;
	case 16:
		return 9999999;
	case 17:
		return 5;
	default:
		return 1;
    }
}

bool PeoplemonRef::knowsMove(int m)
{
	for (int i = 0; i<4; ++i)
	{
		if (moves[i].id==m)
			return true;
	}
	return false;
}

bool PeoplemonRef::teachMove(Game* g, int m, int i)
{
	if (i!=-1)
	{
		moves[i].id = m;
		moves[i].curPp = g->moveList[m].pp;
		return true;
	}
	for (unsigned int j = 0; j<4; ++j)
	{
		if (moves[j].id==0)
		{
			moves[j].id = m;
			moves[j].curPp = g->moveList[m].pp;
			return true;
		}
	}
	return false;
}

void WildPeoplemon::load(string file)
{
    File input(file);
    pair<string,int> temp;
    int t;

    id = input.get<uint16_t>();
    minL = input.get<uint16_t>();
    maxL = input.get<uint16_t>();
    frequency = input.get<uint16_t>();
    t = input.get<uint16_t>();
    overrides.resize(t);
    for (unsigned int i = 0; i<overrides.size(); ++i)
    {
        temp.first = input.getString();
        temp.second = input.get<uint16_t>();
        overrides[i] = temp;
    }
}

PeoplemonRef WildPeoplemon::getPeoplemon(Game* g)
{
    PeoplemonRef ret;

    ret.level = Random(minL, maxL);
    ret.id = id;
    ret.name = g->peoplemonList[id].name;
    for (int i = 0; i<4; ++i)
        ret.curAils[i] = Peoplemon::None;
    ret.curXp = 0;
    ret.nextLvlXp = ret.level*ret.level*ret.level-(ret.level-1)*(ret.level-1)*(ret.level-1);
    ret.holdItem = 0;
    if (Random(0,100)<5)
	{
		ret.holdItem = Random(1,7);
		if (ret.holdItem==7)
			ret.holdItem = 57;
	}

    ret.ivs.acc = Random(0,31);
    ret.ivs.atk = Random(0,31);
    ret.ivs.crit = Random(0,31);
    ret.ivs.def = Random(0,31);
    ret.ivs.evade = Random(0,31);
    ret.ivs.hp = Random(0,31);
    ret.ivs.spAtk = Random(0,31);
    ret.ivs.spDef = Random(0,31);
    ret.evs.acc = 0;
    ret.evs.atk = 0;
    ret.evs.crit = 0;
    ret.evs.def = 0;
    ret.evs.evade = 0;
    ret.evs.hp = 0;
    ret.evs.spAtk = 0;
    ret.evs.spDef = 0;
    ret.recalcStats(g);
    ret.curHp = ret.stats.hp;

    for (int i = 0; i<4; ++i)
    {
        ret.moves[i].id = 0;
    }

    for (unsigned int i = 0; i<g->peoplemonList[ret.id].learnSet.size(); ++i)
    {
        if (g->peoplemonList[ret.id].learnSet[i].first<=ret.level)
        {
            for (int j = 0; j<4; ++j)
            {
                if (ret.moves[j].id==0)
                {
                    ret.moves[j].id = g->peoplemonList[ret.id].learnSet[i].second;
                    ret.moves[j].curPp = g->moveList[ret.moves[j].id].pp;
                    goto learned;
                }
            }

            if (Random(0,1000)<800)
            {
                int f = Random(0,3);
                ret.moves[f].id = g->peoplemonList[ret.id].learnSet[i].second;
                ret.moves[f].curPp = g->moveList[ret.moves[f].id].pp;
            }

            learned:;
        }
    }

    int ovMoveI = 0;
    for (unsigned int i = 0; i<overrides.size(); ++i)
    {
        if (overrides[i].first=="move")
        {
            ret.moves[ovMoveI].id = overrides[i].second;
            ret.moves[ovMoveI].curPp = g->moveList[ret.moves[ovMoveI].id].pp;
            ovMoveI++;
        }
        else if (overrides[i].first=="atk")
            ret.ivs.atk = overrides[i].second;
        else if (overrides[i].first=="def")
            ret.ivs.def = overrides[i].second;
        else if (overrides[i].first=="acc")
            ret.ivs.acc = overrides[i].second;
        else if (overrides[i].first=="evd")
            ret.ivs.evade = overrides[i].second;
        else if (overrides[i].first=="spd")
            ret.ivs.spd = overrides[i].second;
        else if (overrides[i].first=="crit")
            ret.ivs.crit = overrides[i].second;
        else if (overrides[i].first=="hp")
            ret.ivs.hp = overrides[i].second;
        else if (overrides[i].first=="spAtk")
            ret.ivs.spAtk = overrides[i].second;
        else if (overrides[i].first=="spDef")
            ret.ivs.spDef = overrides[i].second;
    }

    return ret;
}

StoredPeoplemon::StoredPeoplemon(int b, Vector2i pos, PeoplemonRef ppl)
{
    boxId = b;
    position = pos;
    data = ppl;
}

double Peoplemon::typeMultipliers[8][19] = {{1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                            {1,  1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0},
                                            {1,  1,1,0.5,2,1,1,1,0.5,2,1,1,0.5,0.5,2,0.5,1,2,1},
                                            {1,  1,2,1,0.5,0.5,0.5,2,2,1,2,0.5,0.5,2,0.5,1,0.5,0.25,1},
                                            {1,  2,0.5,2,1,1,0,1,2,0.5,1,2,0,1,2,4,0,1,0},
                                            {1,  1,1,1,1,1,2,0.5,0.5,1,1,1,2,1,1,1,2,1,2},
                                            {1,  0,1,2,0,0.5,2,1,2,0,0,0,4,2,0,0,0,0,2},
                                            {1,  1,1,0.5,2,2,1,1,0.5,2,1,2,0.5,0.5,2,0.5,1,4,1}};

