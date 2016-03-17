#include "Battle/MoveAnimations.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace sf;
using namespace std;

void MoveAnimations::load(string pplmon, string op, Move m, bool isPlayer)
{
    Vector2f posA = (isPlayer)?(Vector2f(375,490)):(Vector2f(675,290));
    Vector2f posD = (!isPlayer)?(Vector2f(375,490)):(Vector2f(675,290));
    string p1 = (isPlayer)?("Back"):("Front");
    string p2 = (isPlayer)?("Front"):("Back");
    if (m.attackerAnim.size()>0)
        atk = animPool.loadResource(Properties::PeoplemonAnimationPath+pplmon+"/"+p1+"/"+m.attackerAnim);
    else
        atk = animPool.loadResource(Properties::PeoplemonAnimationPath+pplmon+"/"+p1+"/still.anim");
    if (m.defenderAnim.size()>0)
        def = animPool.loadResource(Properties::PeoplemonAnimationPath+op+"/"+p2+"/"+m.defenderAnim);
    else
        def = animPool.loadResource(Properties::PeoplemonAnimationPath+op+"/"+p2+"/still.anim");
    bgnd = animPool.loadResource(Properties::MoveAnimationPath+m.name+"/"+p1+"/Background.anim");
    fgnd = animPool.loadResource(Properties::MoveAnimationPath+m.name+"/"+p1+"/Foreground.anim");

    background.setSource(bgnd);
    background.setPosition(Vector2f(800,600)); //TODO - figure out why this is here
    foreground.setSource(fgnd);
    foreground.setPosition(Vector2f(800,600));

    attacker.setSource(atk);
    attacker.setPosition(posA);
    defender.setSource(def);
    defender.setPosition(posD);
}
