#include "World/People/Npc.hpp"
#include "Util/File.hpp"
#include "Properties.hpp"
#include "Game/Game.hpp"
#include "World/People/WanderingController.hpp"
#include "World/People/PathController.hpp"
#include "World/People/StandingController.hpp"
#include "World/People/SpinningController.hpp"
#include "Game/ConversationState.hpp"
using namespace std;
using namespace sf;

Npc::Npc(string file, bool talked)
{
    wasTalkedTo = talked;

    File input(file);
    name = input.getString();
    file = input.getString();
    loadAnim(walking, file);
    file = input.getString();
    convo.load(file);

    int bId = input.get<uint8_t>();
    if (bId==0)
        behavior = new StandingController(this);
    else if (bId==1)
        behavior = new SpinningController(this, &input);
    else if (bId==2)
        behavior = new PathController(this, &input);
    else
        behavior = new WanderingController(this, &input);
}

void Npc::update(Game* game)
{
    Character::update(game);
}

void Npc::interact(Game* game)
{
    int tDir = dir;
    dir = game->player.getDir()+2;
    if (dir>3)
        dir -= 4;
    game->runStateUnderPriveldged(new ConversationState(game,this,&convo));
    dir = tDir;
    if (!wasTalkedTo)
		game->world.setNpcTalkedTo(name);
    wasTalkedTo = true;
}

bool Npc::talkedTo()
{
    return wasTalkedTo;
}

string Npc::getIdentifier()
{
    return "NPC: "+name;
}
