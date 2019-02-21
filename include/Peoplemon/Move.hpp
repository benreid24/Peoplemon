#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>
#include "Battle/AI/ReactionPreference.hpp"

/**
 * This defines all of the type for moves and peoplemon
 *
 * \ingroup Peoplemon
 */
enum Type
{
    None = 0,
    Normal = 1,
    Intelligent = 2,
    Funny = 3,
    Athletic = 4,
    Quiet = 5,
    Awkward = 6,
    PartyAnimal = 7,
    FunnyPartyAnimal = 8,
    IntelligentAthletic = 9,
    IntelligentNormal = 10,
    NormalQuiet = 11,
    AwkwardFunny = 12,
    IntelligentFunny = 13,
    AthleticNormal = 14,
    FunnyNormal = 15,
    NormalAwkward = 16,
    QuietAthletic = 17,
    IntelligentAwkward = 18
};

/**
 * Structure for storing data related to moves. Instances are stored in a global lookup table
 *
 * \ingroup Peoplemon
 */
struct Move
{
    int id;
    std::string name, description;
    int dmg, acc, priority, pp;
    bool makesContact, isSpecial;

    Type type;

    /**
     * The type of special effect the move has
     */
    enum Effect
    {
        None = 0,
        Heal = 1,
        Poison = 2,
        Burn = 3,
        Paralyze = 4,
        Freeze = 5,
        Confuse = 6,
        LeechSeed = 7,
        Flinch = 8,
        Trap = 9,
        Sleep = 10,
        Protection = 11,
        Safegaurd = 12,
        Substitute = 13,
        HealBell = 14,
        CritUp = 15,
        AtkUp = 16,
        DefUp = 17,
        AccUp = 18,
        EvdUp = 19,
        SpdUp = 20,
        SpAtkUp = 21,
        SpDefUp = 22,
        CritDown = 23,
        AtkDown = 24,
        DefDown = 25,
        AccDown = 26,
        EvdDown = 27,
        SpdDown = 28,
        SpAtkDown = 29,
        SpDefDown = 30,
        Recoil = 31,
        Charge = 32,
        Suicide = 33,
        Counter = 34,
        MirrorCoat = 35,
        OnlySleeping = 36,
        Peanut = 37,
        SetBall = 38,
        WakeBoth = 39,
        HealPercent = 40,
        Encore = 41,
        RandomMove = 42,
        BatonPass = 43,
        DieIn3Turns = 44,
        CritEvdUp = 45,
        BumpBall = 46,
        SpikeBall = 47,
        DeathSwap = 48,
        Gamble = 49,
        StayAlive = 50,
        MaxAtkMinAcc = 51,
        FrustConfuse = 52,
        Spikes = 53,
        DoubleFamily = 54,
        EnemyPPDown = 55,
        HealNext = 56,
        Roar = 57,
        FailOnMove64 = 58,
        SleepHeal = 59,
        SpdAtkUp = 60,
        StealStats = 61,
        BlockBall = 62, //not used explicitly; affects other effects
        SwipeBall = 63,
        DamageThenSwitch = 64,
        RoarCancelBallSpikes = 65
    }effect;
    int chanceOfEffect, intensityOfEffect;
    bool targetIsSelf;

    ReactionPreference::ReactionType classification;

    /**
     * This function returns a non-normalized AI score for the move given the battle state
     *
     * \param atkAdv The regular attack advantage score
     * \param spAtkAdv The special attack advantage score
     * \param userType The type of the attacking peoplemon
     * \param opType The type of the defending peoplemon
     * \return The calculated score
     */
    double damageScore(double atkAdv, double spAtkAdv, Type userType, Type opType);
    double effectScore;
    std::string attackerAnim, defenderAnim;
};

/**
 * Structure for storing data related to a move "owned" by a peoplemon
 *
 * \ingroup Peoplemon
 */
struct MoveRef
{
    int id, curPp;
};

#endif // MOVE_HPP
