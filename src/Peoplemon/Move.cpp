#include "Peoplemon/Move.hpp"
#include "Game/Game.hpp"
using namespace std;

double Move::damageScore(double atkAdv, double spAtkAdv, Type userType, Type opType)
{
    double stab = (userType==type)?(1.5):(1.0);
    double adv = (isSpecial)?(spAtkAdv):(atkAdv);
    double typeM = Peoplemon::getSTAB(userType,type)*Peoplemon::getEffectivenessMultiplier(type,opType);
    return double(dmg)*stab*typeM*adv/140;
}

bool Move::isJokeBased() {
    return id==115 || id==25;
}

bool Move::isTeachBased() {
    return id>=14 && id<=17;
}

bool Move::isAthletic() {
    return type==Type::Athletic ||
           type==Type::AthleticNormal ||
           type==Type::IntelligentAthletic ||
           type==Type::QuietAthletic;
}

bool Move::isQuiet() {
    return type==Type::NormalQuiet ||
           type==Type::Quiet ||
           type==Type::QuietAthletic;
}

string moveTypeToString(Type type) {
    switch (type) {
        case Normal:
            return "Normal";
        case Intelligent:
            return "Intelligent";
        case Funny:
            return "Funny";
        case Athletic:
            return "Athletic";
        case Quiet:
            return "Quiet";
        case Awkward:
            return "Awkward";
        case PartyAnimal:
            return "PartyAnimal";
        case FunnyPartyAnimal:
            return "Funny/PartyAnimal";
        case IntelligentAthletic:
            return "Intelligent/Athletic";
        case IntelligentNormal:
            return "Intelligent/Normal";
        case NormalQuiet:
            return "Normal/Quiet";
        case AwkwardFunny:
            return "Awkward/Funny";
        case IntelligentFunny:
            return "Intelligent/Funny";
        case AthleticNormal:
            return "Athletic/Normal";
        case FunnyNormal:
            return "Funny/Normal";
        case NormalAwkward:
            return "Normal/Awkward";
        case QuietAthletic:
            return "Quiet/Athletic";
        case IntelligentAwkward:
            return "Intelligent/Awkward";
        case None:
        default:
            return "ERROR: Invalid type";
    }
}
