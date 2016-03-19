#include "World/People/Trainer.hpp"
#include "World/People/WanderingController.hpp"
#include "World/People/PathController.hpp"
#include "World/People/StandingController.hpp"
#include "World/People/SpinningController.hpp"
#include "Game/ConversationState.hpp"
#include "Game/BattleState.hpp"
#include "Game/TrainerSpottedPlayerState.hpp"
#include "Battle/AI/RandomBattler.hpp"
#include "Util/File.hpp"
#include "Properties.hpp"
#include "Game/Game.hpp"
using namespace std;
using namespace sf;

Trainer::Trainer(Game* g, string file, bool lost)
{
    beaten = lost;
    confrontingPlayer = false;

    File input(file);

    name = input.getString();
    file = input.getString();
    loadAnim(walking,file);
    file = input.getString();
    preBattle.load(file);
    file = input.getString();
    postBattle.load(file);
    loserSay = input.getString();
    bMusic = input.getString();
    bBgnd = input.getString();
    range = input.get<uint8_t>();
    int t = input.get<uint16_t>();
    for (int i = 0; i<t; ++i)
    {
        file = input.getString();
        PeoplemonRef t;
        t.load(g,Properties::OwnedPeoplemonPath+file);
        peoplemon.push_back(t);
    }
    t = input.get<uint8_t>();
    for (int i = 0; i<t; ++i)
    {
        items.push_back(input.get<uint16_t>());
    }
    aiType = input.get<uint8_t>();
    t = input.get<uint16_t>();

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

void Trainer::update(Game* game)
{
    Character::update(game);

    if (!beaten && !confrontingPlayer)
    {
        Object* o =game->world.getFirstObject(mapPos,dir,range);
        if (Player* p = dynamic_cast<Player*>(o))
        {
            if (!p->isOccupied())
                startFight(game);
        }
    }
}

void Trainer::interact(Game* game)
{
    if (beaten)
        game->data.gameClosedFlag = game->runState(new ConversationState(game,this,&preBattle));
    else
        startFight(game);
}

bool Trainer::isDefeated()
{
    return beaten;
}

void Trainer::setBeaten()
{
	beaten = true;
}

string Trainer::getIdentifier()
{
    return "Trainer: "+name;
}

void Trainer::startFight(Game* game)
{
	confrontingPlayer = true;
	behavior->setPaused(true);
	game->player.setLock(true,true);

    TrainerSpottedPlayerState* s = new TrainerSpottedPlayerState(game,this);
    game->runStateUnderPriveldged(s,false);
    delete s;
    if (game->data.gameClosedFlag)
		return;

    ConversationState* pre = new ConversationState(game,this,&preBattle);
    game->runStateUnderPriveldged(pre,false);
    delete pre;
    if (game->data.gameClosedFlag)
		return;

	BattleState* b = new BattleState(game,new RandomBattler(&peoplemon,items),name,loserSay,prizeMoney,false,bMusic,bBgnd);
	game->runStateUnderPriveldged(b,false);
	if (game->data.gameClosedFlag)
	{
		delete b;
		return;
	}
	if (b->playerWon())
	{
		ConversationState* post = new ConversationState(game,this,&postBattle);
		game->runStateUnderPriveldged(post,false);
		delete post;
	}
	else
	{
		//TODO - do whiteout thing
	}

    beaten = b->playerWon();
    delete b;
    game->player.resetLock();
    behavior->setPaused(false);
    confrontingPlayer = false;
}
