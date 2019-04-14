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
