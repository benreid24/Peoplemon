#include "Battle/PeoplemonAnimations.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
#include "Game/Game.hpp"
using namespace std;
using namespace sf;

void PeoplemonAnimations::load(Game* g, PeoplemonRef ppl, PeoplemonRef op, bool isPlayer)
{
    Vector2f pos = (isPlayer)?(Vector2f(375,490)):(Vector2f(/*675*/0,/*290*/600));
    string p1 = (isPlayer)?("Back"):("Front");
    cIn = animPool.loadResource(Properties::PeoplemonAnimationPath+intToString(ppl.id)+"/"+p1+"/ComeIn.anim");
    sO = animPool.loadResource(Properties::PeoplemonAnimationPath+intToString(ppl.id)+"/"+p1+"/SendOut.anim");
    fnt = animPool.loadResource(Properties::PeoplemonAnimationPath+intToString(ppl.id)+"/"+p1+"/Faint.anim");
    st = animPool.loadResource(Properties::PeoplemonAnimationPath+intToString(ppl.id)+"/"+p1+"/Still.anim");
    for (int i = 0; i<4; ++i)
    {
        if (ppl.moves[i].id!=0)
            moves[i].load(intToString(ppl.id),intToString(op.id),g->moveList[ppl.moves[i].id],isPlayer);
    }

    comeIn.setSource(cIn);
    comeIn.setPosition(pos);

    sendOut.setSource(sO);
    sendOut.setPosition(pos);

    faint.setSource(fnt);
    faint.setPosition(pos);

    still.setSource(st);
    still.setPosition(pos);
}
