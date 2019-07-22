#include "Battle/MoveAnimations.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace sf;
using namespace std;

void MoveAnimations::load(string pplmon, string op, Move m, bool isPlayer)
{
    string p1 = (isPlayer)?("Back"):("Front");
    string p2 = (isPlayer)?("Front"):("Back");
    if (m.attackerAnim.size()>0)
        atk = animPool.loadResource(Properties::PeoplemonAnimationPath+pplmon+"/"+p1+"/"+m.attackerAnim);
    else
        atk = animPool.loadResource(Properties::PeoplemonAnimationPath+pplmon+"/"+p1+"/Still.anim");
    if (m.defenderAnim.size()>0)
        def = animPool.loadResource(Properties::PeoplemonAnimationPath+op+"/"+p2+"/"+m.defenderAnim);
    else
        def = animPool.loadResource(Properties::PeoplemonAnimationPath+op+"/"+p2+"/Still.anim");
    bgnd = animPool.loadResource(Properties::MoveAnimationPath+m.name+"/"+p1+"/Background.anim");
    fgnd = animPool.loadResource(Properties::MoveAnimationPath+m.name+"/"+p1+"/Foreground.anim");

    background.setSource(bgnd);
    foreground.setSource(fgnd);

    attacker.setSource(atk);
    defender.setSource(def);
}
