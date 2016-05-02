#include "Game/BattleState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Battle/PlayerBattler.hpp"
#include "Game/EvolveState.hpp"
#include "Game/DeleteMoveState.hpp"
using namespace std;
using namespace sf;

BattleState::BattleState(Game* g, Battler* op, string opName, string ll, int pm, bool cr, string music, string bgnd, Gamestate* n) : Gamestate(g,n)
{
    lastMoveHit = false;
    ensureClosed = false;
    playerWinned = false;
    player = new PlayerBattler(this, &g->player,cr);
    player->healAils(false);
    opponent = op;
    canRun = cr;
    opponentName = opName;
    loseLine = ll;
    prizeMoney = pm;
    playlist = music;
    playerBox.setIsPlayer();
    playerBox.update(player->getPeoplemon()->at(player->getCurrentPeoplemon()));
    opBox.update(opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()));
    backgroundTxtr = imagePool.loadResource(Properties::BattleImagePath+bgnd);
    background.setTexture(*backgroundTxtr);
    playerAnims.load(game, player->getPeoplemon()->at(player->getCurrentPeoplemon()),opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()),true);
    opponentAnims.load(game, opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()), player->getPeoplemon()->at(player->getCurrentPeoplemon()),false);
	game->hud.rePosition(Vector2f(15,480),480);
}

BattleState::~BattleState()
{
    game->data.gameClosedFlag = ensureClosed;
    game->hud.setAlwaysShow(false);
    player->zeroStages();
    player->healAils(false);
    game->hud.rePosition();
    delete player;
    delete opponent;
}

bool BattleState::shouldClose()
{
    if (finishFrame() || game->data.gameClosedFlag)
    {
        ensureClosed = true;
        return true;
    }
    return false;
}

bool BattleState::execute()
{
    //transition screen

    //first send outs with intro text in between, or maybe use existing structure (below) to do the send outs

    //main battle loop
    //  get input and populate turns array
    //  figure out who goes first then assign the proper pointers in the order arrays
    //  loop through order array
    //      do first turn, play animations, deal damage and apply other move effects
    //      if not dead, do second turn. if dead then get the peoplemon to switch to and play switch anims
    //      if second mover killed the first get their switch and play switch anims

	game->music.load(playlist,true);
    game->music.play();
    transitionScreen();

    //Calculate all stats to ensure they are current
    player->recalcStats(game);
    opponent->recalcStats(game);

    //introduce opponent peoplemon
    game->hud.setAlwaysShow(true);
    opBox.update(getPeoplemon(opponent,opponent->getCurrentPeoplemon()),true);
    playerBox.update(getPeoplemon(player,player->getCurrentPeoplemon()),true);
    renderStatic();
    sleep(milliseconds(1500));
    if (canRun)
        toDraw.push_back(&opponentAnims.still);
    displayMessage(getIntroLine());
    if (shouldClose())
        return true;
    if (!canRun)
    {
        playIntroAnim(opponent);
        if (shouldClose())
            return true;
        toDraw.push_back(&opponentAnims.still);
    }
    game->hud.displayMessage("");

    //send in player peoplemon
    displayMessage("Go "+getPeoplemonName(player,player->getCurrentPeoplemon())+"!");
    if (shouldClose())
        return true;
    playIntroAnim(player);
    if (shouldClose())
        return true;
    toDraw.clear();
    sentIn.push_back(player->getCurrentPeoplemon());

    //handle Bud ability
    player->getPeoplemon()->at(player->getCurrentPeoplemon()).curAbility = game->peoplemonList[player->getPeoplemon()->at(player->getCurrentPeoplemon()).id].specialAbilityId;
    opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()).curAbility = game->peoplemonList[opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()).id].specialAbilityId;
    if (player->getPeoplemon()->at(player->getCurrentPeoplemon()).curAbility==Peoplemon::Bud)
	{
		if (opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()).curAbility!=Peoplemon::Bud)
		{
			player->getPeoplemon()->at(player->getCurrentPeoplemon()).curAbility = opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()).curAbility;
			displayMessage(player->getPeoplemon()->at(player->getCurrentPeoplemon()).name+" is a Bud and copied "+opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()).name+"'s "+Peoplemon::abilityTexts[opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()).curAbility].first+"!");
		}
	}
	if (opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()).curAbility==Peoplemon::Bud)
	{
		if (player->getPeoplemon()->at(player->getCurrentPeoplemon()).curAbility!=Peoplemon::Bud)
		{
			opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()).curAbility = player->getPeoplemon()->at(player->getCurrentPeoplemon()).curAbility;
			displayMessage(opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()).name+" is a Bud and copied "+player->getPeoplemon()->at(player->getCurrentPeoplemon()).name+"'s "+Peoplemon::abilityTexts[player->getPeoplemon()->at(player->getCurrentPeoplemon()).curAbility].first+"!");
		}
	}

    int runTries = 0;
    //battle loop
    while (true)
    {
        //render everything first
        renderStatic();

        //get player turn from input, opponent turn and determine order
        {
            PeoplemonRef p = getPeoplemon(player,player->getCurrentPeoplemon()), op = getPeoplemon(opponent,opponent->getCurrentPeoplemon());
            Turn pTurn = player->getTurn(op,game);
            if (shouldClose())
                return true;

            Turn oTurn = opponent->getTurn(p,game);
            bool pFirst = true;
            if (oTurn.type==Turn::Switch || oTurn.type==Turn::Item)
            {
                if (pTurn.type==Turn::Move)
                    pFirst = false;
            }
            else if (pTurn.type==Turn::Move)
            {
                int opMoveP = game->moveList[oTurn.id].priority;
                int pMoveP = game->moveList[pTurn.id].priority;
				pFirst = !(opMoveP>pMoveP || (opMoveP==pMoveP && (op.hasAilment(Peoplemon::Annoyed)?(op.stats.spd/4):(op.stats.spd))>(p.hasAilment(Peoplemon::Annoyed)?(p.stats.spd/4):(p.stats.spd))));
            }

            if (pFirst)
            {
                turns[0] = pTurn;
                order[0] = player;
                anims[0] = &playerAnims;
                turns[1] = oTurn;
                order[1] = opponent;
                anims[1] = &opponentAnims;
            }
            else
            {
                turns[0] = oTurn;
                order[0] = opponent;
                anims[0] = &opponentAnims;
                turns[1] = pTurn;
                order[1] = player;
                anims[1] = &playerAnims;
            }
        } //end input and ordering

        //loop through turns and do them while outputting stuff. declare things here to indicate moves like protect being used
        bool shouldStop = false;
        for (int i = 0; i<2; ++i)
        {
            player->recalcStats(game);
            opponent->recalcStats(game);

            if (shouldStop)
                break;

            int j = (i==0)?(1):(0); //to quickly reference the other guy
            toDraw.clear();
            toDraw.push_back(&playerAnims.still);
            toDraw.push_back(&opponentAnims.still);

            if (turns[i].type==Turn::Switch)
            {
            	if (order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).curAbility==Peoplemon::Engage)
				{
					displayMessage(order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).name+"'s Engage prevents switching!");
					continue;
				}
                displayMessage(getSwitchLine(order[i],turns[i].id));
                if (shouldClose())
                    return true;

                playSwitchAnim(order[i],order[j],turns[i].id,order[i]->getCurrentPeoplemon());
                if (shouldClose())
                    return true;

                game->hud.displayMessage("");
                renderStatic();
                if (shouldClose())
                    return true;

				if (order[i]==player)
				{
					if (find(sentIn.begin(),sentIn.end(),order[i]->getCurrentPeoplemon())==sentIn.end())
						sentIn.push_back(order[i]->getCurrentPeoplemon());
				}
            }
            else if (turns[i].type==Turn::Item)
            {
                displayMessage(getItemLine(order[i],turns[i].id));
                if (shouldClose())
                    return true;
                //TODO - apply the item with id turns[i].id to the peoplemon& at order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon())
                if (order[i]!=player)
				{
					switch (turns[i].id)
					{
					case 1:
						order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp += 20;
						break;
					case 2:
						order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp += 50;
						break;
					case 3:
						order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp += 200;
						break;
					case 4:
						order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp += 9001;
						for (int i = 0; i<4; ++i)
							order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curAils[i] = Peoplemon::None;
						break;
					}
					//cap health
					if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp>order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp)
						order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp;
					renderStatic();
				}
            }
            else if (turns[i].type==Turn::Move)
            {
                PeoplemonRef attacker = getPeoplemon(order[i],order[i]->getCurrentPeoplemon());
                vector<string> lines;
                if (attacker.hasAilment(Peoplemon::Confused))
                {
                    displayMessage(attacker.name+" is confused!");
                    if (shouldClose())
                        return true;

                    if (Random(0,100)<35 || attacker.turnsConfused>=4)
                    {
                        displayMessage(attacker.name+" snapped out of confusion!");
                        if (shouldClose())
                            return true;

                        order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).removePassiveAilment(Peoplemon::Confused);
                        order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsConfused = 0;
                    }
                    if (Random(0,100)<50)
                    {
                        displayMessage(attacker.name+" hurt itself in confusion!");
                        if (shouldClose())
                            return true;
                        applyMove(order[i],order[i],80);
                        goto noMove;
                    }
                }
                if (attacker.hasAilment(Peoplemon::Annoyed))
                {
                    displayMessage(attacker.name+" is annoyed!");
                    if (shouldClose())
                        return true;

                    if (Random(0,100)<25)
                    {
                        displayMessage(attacker.name+" is too annoyed to move!");
                        if (shouldClose())
                            return true;
                        goto noMove;
                    }
                }
                if (attacker.hasAilment(Peoplemon::Sleep))
                {
                    int turnWait = (attacker.curAbility==Peoplemon::LateRiser)?(1):(0);
                    if ((attacker.turnsWithAil>turnWait && Random(0,100)<35) || attacker.turnsWithAil>=4)
                    {
                        displayMessage(attacker.name+" woke up!");
                        if (shouldClose())
                            return true;
                        order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curAils[0] = Peoplemon::None;
                    }
                    else
                    {
                        displayMessage(attacker.name+" is asleep!");
                        if (shouldClose())
                            return true;
                        goto noMove;
                    }
                }
                if (attacker.hasAilment(Peoplemon::Frozen))
                {
                    if (attacker.turnsWithAil>1 && Random(0,100)<20)
                    {
                        displayMessage(attacker.name+" thawed out!");
                        if (shouldClose())
                            return true;
                        order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curAils[0] = Peoplemon::None;
                    }
                    else
                    {
                        displayMessage(attacker.name+" is frozen!");
                        if (shouldClose())
                            return true;
                        goto noMove;
                    }
                }

                lines = applyMove(order[i],order[j],turns[i].id);
                displayMessage(getMoveLine(order[i],turns[i].id));
                if (shouldClose())
                    return true;

                toDraw.clear();
                if (lastMoveHit)
                    playAttackAnim(order[i],turns[i].id);
                if (shouldClose())
                    return true;
                toDraw.push_back(&anims[0]->still);
                toDraw.push_back(&anims[1]->still);

                renderStatic();
                if (shouldClose())
                    return true;
                for (unsigned int k = 0; k<lines.size(); ++k)
                {
                    displayMessage(lines[k]);
                    if (shouldClose())
                        return true;
                }
                game->hud.displayMessage("");
                if (getPeoplemon(order[j],order[j]->getCurrentPeoplemon()).curHp<=0) //they fainted
                {
                    shouldStop = true;
                    bool done = doFaint(i,j);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
                }
                if (getPeoplemon(order[i],order[i]->getCurrentPeoplemon()).curHp<=0) //attacker fainted
                {
                    shouldStop = true;
                    bool done = doFaint(j,i);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
                }
                noMove:;
            }
            else //run
            {
                runTries++;
                int rSpd = getPeoplemon(order[i],order[i]->getCurrentPeoplemon()).stats.spd;
                if (getPeoplemon(order[i],order[i]->getCurrentPeoplemon()).hasAilment(Peoplemon::Annoyed))
                    rSpd /= 4;
                int dSpd = getPeoplemon(order[j],order[j]->getCurrentPeoplemon()).stats.spd;
                if (getPeoplemon(order[j],order[j]->getCurrentPeoplemon()).hasAilment(Peoplemon::Annoyed))
                    dSpd /= 4;
                int F = ((rSpd*128)/dSpd+(30*runTries))%256;
                bool gotAway = Random(0,255)<F || order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curAbility==Peoplemon::RunAway;

                if (gotAway)
                {
                    displayMessage("Got away safely!");
                    if (shouldClose())
                        return true;
                    return false;
                }
                else
                {
                    displayMessage("Can't escape!");
                    if (shouldClose())
                        return true;
                }
            }
        } //end turn loop

        //TODO - apply end turn ailments and other effects here
        order[0] = opponent;
        order[1] = player;
        bool opAlive = true;
        for (int i = 0; i<2; ++i)
        {
            int j = (i==0)?(1):(0);
            PeoplemonRef ppl = getPeoplemon(order[i],order[i]->getCurrentPeoplemon());

			if (ppl.holdItem==50) //bag of goldfish
			{
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp += order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp/16;
                if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp>order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp)
					order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp;
                displayMessage(ppl.name+"'s Bag of Goldfish restored health!");
			}
            if (ppl.hasAilment(Peoplemon::Frustrated))
            {
                displayMessage(ppl.name+" is hurt by frustration!");
                if (shouldClose())
                    return true;
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp -= order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp/16;
                if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp<0)
                {
                    order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = 0;
                    bool done = doFaint(i,j);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
                }
            }
            if (ppl.hasAilment(Peoplemon::Stolen) && opAlive)
            {
                displayMessage(ppl.name+" is having its health stolen by "+order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).name+"!");
                if (shouldClose())
                    return true;

                int dmg = float(ppl.stats.hp)/16.0+0.5;
                if (dmg>ppl.curHp)
                    dmg = ppl.curHp;
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp -= dmg;
                order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).curHp += dmg;
                if (order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).curHp>order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).stats.hp)
                    order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).curHp = order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).stats.hp;
				renderStatic();
				renderStatic();

                if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp<=0)
                {
                    bool done = doFaint(j,i);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
                }
            }
            if (ppl.hasAilment(Peoplemon::Sticky))
            {
                displayMessage(ppl.name+" is hurt from being sticky!");
                if (shouldClose())
                    return true;

                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp -= order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsWithAil*order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp/16;
                if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp<0)
                {
                    order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = 0;
                    bool done = doFaint(i,j);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
                }
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsWithAil++;
            }
            if (ppl.holdItem==55) //super tiny mini fridge
			{
				displayMessage(ppl.name+" has a Super Tiny Mini Fridge!");
				applyMove(order[i],order[j],81);
				bool done = doFaint(j,i);
				if (shouldClose())
					return true;
				if (done)
					return false;
				if (getPeoplemon(order[j],order[j]->getCurrentPeoplemon()).curHp<=0) //they fainted
                {
                    bool done = doFaint(i,j);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
					opAlive = false;
					if (i==0)
						break;
                }
			}
            if (ppl.hasAilment(Peoplemon::Sleep))
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsWithAil++;
            if (ppl.hasAilment(Peoplemon::Confused))
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsConfused++;
        }
    } //end main loop

    return false;
}

void BattleState::renderBase()
{
    playerAnims.still.update();
    opponentAnims.still.update();

    game->mainWindow.draw(background);
    playerAnims.still.draw(&game->mainWindow);
    opponentAnims.still.draw(&game->mainWindow);
    opBox.draw(&game->mainWindow);
    playerBox.draw(&game->mainWindow);
}

void BattleState::displayMessage(string m)
{
    game->hud.displayMessage(m);
    while (!game->hud.messageFinished())
    {
        if (finishFrame())
        {
            game->data.gameClosedFlag = true;
            return;
        }

        game->hud.update();
        opBox.update();
        playerBox.update();

        game->mainWindow.draw(background);
        renderQueue();
        opBox.draw(&game->mainWindow);
        playerBox.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(50));
    }
}

void BattleState::renderStatic()
{
    opBox.update(getPeoplemon(opponent,opponent->getCurrentPeoplemon()),true);
    playerBox.update(getPeoplemon(player,player->getCurrentPeoplemon()),true);
    while (!opBox.barGood() || !playerBox.barGood())
    {
        opBox.update();
        playerBox.update();
        playerAnims.still.update();
        opponentAnims.still.update();

        game->mainWindow.draw(background);
        playerAnims.still.draw(&game->mainWindow);
        opponentAnims.still.draw(&game->mainWindow);
        opBox.draw(&game->mainWindow);
        playerBox.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(33));
    }
}

string BattleState::getIntroLine()
{
    if (opponentName.find("WILD")==string::npos)
        return opponentName+" sent out "+getPeoplemonName(opponent,opponent->getCurrentPeoplemon())+"!";
    return "A "+opponentName+" appeared!";
}

string BattleState::getSwitchLine(Battler* b, int nextP)
{
    if (b==player)
        return "Go "+getPeoplemonName(b,nextP)+"!";
    return opponentName+" sent out "+getPeoplemonName(b,nextP)+"!";
}

string BattleState::getPeoplemonName(Battler* b, int i)
{
    return b->getPeoplemon()->at(i).name;
}

PeoplemonRef BattleState::getPeoplemon(Battler* b, int i)
{
    return b->getPeoplemon()->at(i);
}

string BattleState::getItemLine(Battler* b, int id)
{
    if (b==player)
    {
        //TODO - create something based on item effect
        return "You used an item";
    }
    else
        return opponentName+" used a "+game->itemList[id].name+"!";
}

string BattleState::getMoveLine(Battler* b, int id)
{
    return b->getPeoplemon()->at(b->getCurrentPeoplemon()).name+" used "+game->moveList[id].name+"!";
}

bool BattleState::doFaint(int i, int j)
{
    bool done = false;
    toDraw.clear();
    toDraw.push_back(&anims[i]->still);

    PeoplemonAnimations temp;
    Battler* b = order[i];
    Battler* o = order[j];
    bool isPlayer = b==player;

    anims[j]->faint.play();
    while (!anims[j]->faint.finished())
    {
        if (finishFrame())
        {
            game->data.gameClosedFlag = true;
            return true;
        }
        anims[j]->faint.update();

        game->mainWindow.draw(background);
        anims[j]->faint.draw(&game->mainWindow);
        anims[i]->still.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(30));
    }

    displayMessage(getPeoplemonName(order[j],order[j]->getCurrentPeoplemon())+" fainted!");
    if (shouldClose())
        return true;

    int id = o->getSwitchPeoplemon(b->getPeoplemon()->at(b->getCurrentPeoplemon()), game);
    if (shouldClose())
		return true;

    if (id==-1)
        done = true;
	if (!done)
		temp.load(game,o->getPeoplemon()->at(id),b->getPeoplemon()->at(b->getCurrentPeoplemon()),!isPlayer);

    if (isPlayer)
    {
        int index = order[j]->getCurrentPeoplemon();
        double xp = (canRun)?(1):(1.5);
        xp *= game->peoplemonList[getPeoplemon(order[j],index).id].xpYield;
        xp *= getPeoplemon(order[j],index).level;
        xp *= (getPeoplemon(order[j],index).level>game->peoplemonList[getPeoplemon(order[j],index).id].evolveLevel)?(1.2):(1);
        xp /=  sentIn.size();
        for (unsigned int k = 0; k<sentIn.size(); ++k)
        {
            order[i]->getPeoplemon()->at(sentIn[k]).awardEVs(game,game->peoplemonList[getPeoplemon(order[j],order[j]->getCurrentPeoplemon()).id].evAward);
            order[i]->getPeoplemon()->at(sentIn[k]).recalcStats(game);
            order[i]->getPeoplemon()->at(sentIn[k]).curXp += xp;
            displayMessage(getPeoplemonName(order[i],sentIn[k])+" earned "+intToString(xp)+" XP!");
            if (shouldClose())
                return true;

            while (getPeoplemon(order[i],sentIn[k]).curXp>=getPeoplemon(order[i],sentIn[k]).nextLvlXp)
            {
                int level = ++order[i]->getPeoplemon()->at(sentIn[k]).level;
                order[i]->getPeoplemon()->at(sentIn[k]).recalcStats(game);
                order[i]->getPeoplemon()->at(sentIn[k]).curXp -= order[i]->getPeoplemon()->at(sentIn[k]).nextLvlXp;

                if (sentIn[k]==order[i]->getCurrentPeoplemon())
                    playerBox.update(getPeoplemon(order[i],sentIn[k]),false);
                displayMessage(getPeoplemon(order[i],sentIn[k]).name+" grew to level "+intToString(level)+"!");
                if (shouldClose())
                    return true;

                vector<pair<int,int> > learnSet = game->peoplemonList[order[i]->getPeoplemon()->at(sentIn[k]).id].learnSet;
                for (unsigned int m = 0; m<learnSet.size(); ++m)
                {
                    if (learnSet[m].first==level)
                    {
                        string moveName = game->moveList[learnSet[m].second].name;
                        int moveId = learnSet[m].second;
                        bool learned = false;
						if (!order[i]->getPeoplemon()->at(sentIn[k]).knowsMove(moveId))
						{
							learned = order[i]->getPeoplemon()->at(sentIn[k]).teachMove(game,moveId);

							if (!learned)
							{
								displayMessage(getPeoplemon(order[i],sentIn[k]).name+" is trying to learn "+moveName+"!");
								if (shouldClose())
									return true;

								DeleteMoveState* state = new DeleteMoveState(game,&order[i]->getPeoplemon()->at(sentIn[k]),learnSet[m].second);
								if (game->runState(state))
									return true;
								if (sentIn[k]==order[i]->getCurrentPeoplemon())
									anims[i]->load(game,getPeoplemon(order[i],sentIn[k]),getPeoplemon(order[j],order[j]->getCurrentPeoplemon()),true);
							}
							else
							{
								displayMessage(getPeoplemon(order[i],sentIn[k]).name+" learned "+moveName+"!");
								if (shouldClose())
									return true;
							}
						}
                    }
                }

                if (level==game->peoplemonList[order[i]->getPeoplemon()->at(sentIn[k]).id].evolveLevel)
                {
                    EvolveState* s = new EvolveState(game,&order[i]->getPeoplemon()->at(sentIn[k]));
                    if (game->runState(s,false))
                        return true;
                    if (s->evolutionSuccessful() && order[i]->getCurrentPeoplemon()==sentIn[k])
                        anims[i]->load(game,getPeoplemon(order[i],sentIn[k]),getPeoplemon(order[j],order[j]->getCurrentPeoplemon()),order[i]==player);
                    delete s;
                }
            }
        }
        sentIn.clear();
        sentIn.push_back(player->getCurrentPeoplemon());
    }

    if (!done)
    {
    	string line = (isPlayer)?(opponentName+" sent out "+opponent->getPeoplemon()->at(id).name+"!"):("Go "+player->getPeoplemon()->at(id).name+"!");
    	displayMessage(line);
    	if (shouldClose())
			return true;

    	toDraw.clear();
    	temp.sendOut.play();
    	if (isPlayer)
			opBox.update(o->getPeoplemon()->at(id),true);
		else
			playerBox.update(o->getPeoplemon()->at(id),true);

		while (!temp.sendOut.finished())
		{
			if (finishFrame())
			{
				game->data.gameClosedFlag = true;
				return true;
			}
			temp.sendOut.update();
			opBox.update();
			playerBox.update();

			game->mainWindow.draw(background);
			temp.sendOut.draw(&game->mainWindow);
			anims[i]->still.draw(&game->mainWindow);
			game->hud.draw(&game->mainWindow);
			game->mainWindow.display();

			sleep(milliseconds(30));
		}

		if (!isPlayer)
		{
			playerAnims = temp;
			opponentAnims.load(game,opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()),player->getPeoplemon()->at(player->getCurrentPeoplemon()),false);
		}
		else
		{
			opponentAnims = temp;
			playerAnims.load(game,player->getPeoplemon()->at(player->getCurrentPeoplemon()),opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()),true);
		}
		toDraw.push_back(&anims[i]->still);
		toDraw.push_back(&anims[j]->still);
    }

    game->hud.displayMessage("");
    if (done)
    {
        if (order[j]==player)
        {
            playerWinned = false;
            displayMessage(game->player.getName()+" is all out of usable Peoplemon!");
            if (shouldClose())
                return true;
            displayMessage(game->player.getName()+" oranged out!");
            if (shouldClose())
                return true;
            game->data.whiteoutFlag = true;
            return true;
        }
        else
        {
            playerWinned = true;
            if (loseLine.size()>0)
            {
                displayMessage(loseLine);
                if (shouldClose())
                    return true;
            }
            if (prizeMoney>0)
            {
                displayMessage(game->player.getName()+" won "+intToString(prizeMoney)+" monies!");
                if (shouldClose())
                    return true;
            }
            return true;
        }
    }
    renderStatic();
    return false;
}

vector<string> BattleState::applyMove(Battler* atk, Battler* def, int id)
{
	cout << "Move " << game->moveList[id].name << " used with power " << game->moveList[id].dmg << endl;

    vector<string> ret;
    PeoplemonRef attacker = getPeoplemon(atk,atk->getCurrentPeoplemon()), defender = getPeoplemon(def,def->getCurrentPeoplemon());

    if (game->moveList[id].effect==Move::OnlySleeping && defender.curAils[0]!=Peoplemon::Sleep)
    {
        ret.push_back(attacker.name+" tried to use "+game->moveList[id].name+" but it failed because "+defender.name+" is not asleep!");
        return ret;
    }
    else if (game->moveList[id].effect==Move::Charge)
    {
        //TODO - make sure they don't pick another move while charging
        if (atk->state.isCharging)
        {
            atk->state.isCharging = false;
            ret.push_back(attacker.name+" unleashed power!");
        }
        else
        {
            atk->state.isCharging = true;
            ret.push_back(attacker.name+" is charging power!");
            return ret;
        }
    }

    ReactionPreference::ReactionType type = game->moveList[id].classification;
    if (def->state.protectUsedLast && (type==ReactionPreference::AttackingAilment || type==ReactionPreference::DirectAttack || type==ReactionPreference::PassiveAttack))
    {
        ret.push_back(attacker.name+" tried to use "+game->moveList[id].name+" but "+defender.name+" blocked the attack!");
        return ret;
    }

	double power = game->moveList[id].dmg;
    double multiplier = 1;
    bool hit = (Random(0,100)<game->moveList[id].acc*attacker.stats.acc/defender.stats.evade) || game->moveList[id].acc==0;
    bool critical = Random(0,100)<=attacker.stages.getCritChance() && power>0.1;
    double stab = Peoplemon::getSTAB(game->peoplemonList[attacker.id].type,game->moveList[id].type);
    double effectiveness = Peoplemon::getEffectivenessMultiplier(game->moveList[id].type,game->peoplemonList[defender.id].type);
    bool isSuper = effectiveness>1 && power>0.1;
    bool isBad = effectiveness<1 && power>0.1;
    multiplier *= stab*effectiveness;

    if (critical && defender.curAbility==Peoplemon::Lax)
	{
		critical = false;
		ret.push_back(defender.name+"'s Lax prevents critical hits!");
	}
    multiplier *= (critical)?(2):(1);
    if (attacker.curAbility==Peoplemon::Edumucator && game->moveList[id].type==Intelligent && power>0.1)
	{
		multiplier *= 1.1;
		ret.push_back(attacker.name+" does more damage with intelligent moves as an edumucator!");
	}

    bool isSpecial = game->moveList[id].isSpecial;
    double atkS = (isSpecial)?(attacker.stats.spAtk):(attacker.stats.atk);
    if (attacker.hasAilment(Peoplemon::Frustrated) && !isSpecial)
        atkS /= 2;
    double defS = (isSpecial)?(defender.stats.spDef):(defender.stats.def);
    double damage = (power>0.1)?((((2*double(attacker.level)+10)/250)*(atkS/defS)*power+2)*multiplier):(0);
    cout << "Damage was: " << damage << endl;

   // if (def==player && damage>0.1)
	//	damage = 2;

    if (attacker.holdItem==54)
	{
		damage *= 1.1;
		ret.push_back(attacker.name+"'s Slapping Glove increased damage!");
	}

	atk->setLastDamageDealt(damage); //for ai
	def->setLastDamageTaken(damage);

    if (hit)
	{
		if (defender.curAbility==Peoplemon::EasyGoing && damage>=defender.curHp && defender.curHp>1)
		{
			ret.push_back(defender.name+"'s Easy Going nature prevented a one hit KO!");
			def->getPeoplemon()->at(def->getCurrentPeoplemon()).curHp = 1;
		}
		else
			def->getPeoplemon()->at(def->getCurrentPeoplemon()).curHp -= damage;
	}
	else
	{
		ret.clear();
        ret.push_back("But it missed!");
        lastMoveHit = false;
        return ret;
	}

    if (def->getPeoplemon()->at(def->getCurrentPeoplemon()).curHp<0)
        def->getPeoplemon()->at(def->getCurrentPeoplemon()).curHp = 0;

    if (critical && hit)
        ret.push_back("It was a critical hit!");
    if (isSuper && hit)
        ret.push_back("It was super effective!");
    if (isBad && hit)
	{
		if (effectiveness>0)
			ret.push_back("It wasn't very effective");
		else
			ret.push_back(game->typeList[game->moveList[id].type]+" moves don't affect "+defender.name+"!");
	}

    Move::Effect effect = game->moveList[id].effect;
    int intensity = game->moveList[id].intensityOfEffect;
    PeoplemonRef* giver = &atk->getPeoplemon()->at(atk->getCurrentPeoplemon());
	PeoplemonRef* taker = &def->getPeoplemon()->at(def->getCurrentPeoplemon());
	bool reciprocateAil = taker->curAbility==Peoplemon::Share && !game->moveList[id].targetIsSelf;

	if (taker->curAbility==Peoplemon::Sassy && !isSpecial)
	{
		int dmg = giver->stats.hp/16;
		giver->curHp -= dmg;
		if (giver->curHp<0)
			giver->curHp = 0;
		ret.push_back(giver->name+" took damage because "+taker->name+" is Sassy!");
	}
	if (taker->curAbility==Peoplemon::Opinionated && giver->curAils[0]==Peoplemon::None && Random(0,100)<=20)
	{
		giver->curAils[0] = Peoplemon::Annoyed;
		ret.push_back(giver->name+" was annoyed because "+taker->name+" is opinionated!");
	}

    if (Random(0,100)<=game->moveList[id].chanceOfEffect && effect!=Move::None)
    {
        bool canGetAils = !def->state.ailSafe && def->state.subHealth==0;
        if (game->moveList[id].targetIsSelf)
        {
            swap(giver,taker);
            canGetAils = !atk->state.ailSafe && atk->state.subHealth==0;
        }

        if (effect==Move::Heal)
        {
            taker->curHp += (100.0/double(intensity))*taker->stats.hp;
            if (taker->curHp>taker->stats.hp)
                taker->curHp = taker->stats.hp;
            ret.push_back(taker->name+" regained health!");
        }
        else if (effect==Move::Poison)
        {
            if (taker->curAils[0]==Peoplemon::None && canGetAils)
            {
                taker->curAils[0] = Peoplemon::Sticky;
                taker->turnsWithAil = 1;
                ret.push_back(taker->name+" was made sticky!");
                if (reciprocateAil)
				{
					if (giver->curAils[0]==Peoplemon::None)
					{
						giver->curAils[0] = Peoplemon::Sticky;
						ret.push_back(taker->name+" Shared its stickiness with "+giver->name+"!");
					}
					else
						ret.push_back(taker->name+" tried to Share its stickiness with "+giver->name+" but if failed!");
				}
            }
            else
                ret.push_back(giver->name+" tried to make "+taker->name+" sticky but it failed!");
        }
        else if (effect==Move::Burn)
        {
            if (taker->curAils[0]==Peoplemon::None && canGetAils)
            {
                taker->curAils[0] = Peoplemon::Frustrated;
                ret.push_back(taker->name+" was frustrated!");
                if (reciprocateAil)
				{
                    if (giver->curAils[0]==Peoplemon::None)
					{
						giver->curAils[0] = Peoplemon::Frustrated;
						ret.push_back(taker->name+" Shared its frustration with "+giver->name+"!");
					}
					else
						ret.push_back(taker->name+" tried to Share its frustration with "+giver->name+" but it failed!");
				}
            }
            else
                ret.push_back(giver->name+" tried to frustrate "+taker->name+" but it failed!");
        }
        else if (effect==Move::Paralyze)
        {
            if (taker->curAils[0]==Peoplemon::None && canGetAils)
            {
                taker->curAils[0] = Peoplemon::Annoyed;
                ret.push_back(taker->name+" was annoyed!");
                if (reciprocateAil)
				{
                    if (giver->curAils[0]==Peoplemon::None)
					{
						giver->curAils[0] = Peoplemon::Annoyed;
						ret.push_back(taker->name+" Shared its annoyance with "+giver->name+"!");
					}
					else
						ret.push_back(taker->name+" tried to Share its annoyance with "+giver->name+" but it failed!");
				}
            }
            else
                ret.push_back(giver->name+" tried to annoy "+taker->name+" but it failed!");
        }
        else if (effect==Move::Freeze)
        {
            if (taker->curAils[0]==Peoplemon::None && canGetAils)
            {
                taker->curAils[0] = Peoplemon::Frozen;
                ret.push_back(taker->name+" was frozen!");
                if (reciprocateAil)
				{
                    if (giver->curAils[0]==Peoplemon::None)
					{
						giver->curAils[0] = Peoplemon::Frozen;
						ret.push_back(taker->name+" Shared its frozeness with "+giver->name+"!");
					}
					else
						ret.push_back(taker->name+" tried to Share its frozeness with "+giver->name+" but it failed!");
				}
            }
            else
                ret.push_back(giver->name+" tried to freeze "+taker->name+" but it failed!");
        }
        else if (effect==Move::Confuse)
        {
            if (canGetAils)
            {
                taker->addPassiveAilment(Peoplemon::Confused);
                taker->turnsConfused = 1;
                ret.push_back(taker->name+" was confused!");
                if (reciprocateAil)
				{
					giver->addPassiveAilment(Peoplemon::Confused);
					ret.push_back(taker->name+" Shared its confusion with "+giver->name+"!");
				}
            }
            else
                ret.push_back(giver->name+" tried to confuse "+taker->name+" but it failed!");
        }
        else if (effect==Move::LeechSeed)
        {
            if (canGetAils)
            {
                taker->addPassiveAilment(Peoplemon::Stolen);
                ret.push_back(taker->name+" was jumped!");
                if (reciprocateAil)
				{
					giver->addPassiveAilment(Peoplemon::Stolen);
					ret.push_back(taker->name+" Shared the mugging with "+giver->name+"!");
				}
            }
            else
                ret.push_back(giver->name+" tried to jump "+taker->name+" but it failed!");
        }
        else if (effect==Move::Flinch && atk==order[0])
        {
            if (canGetAils)
            {
                def->state.flinched = true;
                ret.push_back(taker->name+" was distracted!");
            }
            else
                ret.push_back(giver->name+" tried to distract "+taker->name+" but it failed!");
        }
        else
            def->state.flinched = false; //catch all to make sure the flag is reset

        if (effect==Move::Trap)
        {
            if (canGetAils)
            {
                def->state.trapped = true;
                ret.push_back(taker->name+" was trapped!");
                if (reciprocateAil)
				{
					atk->state.trapped = true;
					ret.push_back(taker->name+" Shared its entrapment with "+giver->name+"!");
				}
            }
            else
                ret.push_back(giver->name+" tried to trap "+taker->name+" but it failed!");
        }
        else if (effect==Move::Sleep)
        {
            if (taker->curAils[0]==Peoplemon::None && canGetAils)
            {
                taker->curAils[0] = Peoplemon::Sleep;
                taker->turnsWithAil = 0;
                ret.push_back(taker->name+" fell asleep!");
                if (reciprocateAil)
				{
                    if (giver->curAils[0]==Peoplemon::None)
					{
						giver->curAils[0] = Peoplemon::Sleep;
						ret.push_back(taker->name+" Shared its sleepiness with "+giver->name+"!");
					}
					else
						ret.push_back(taker->name+" tried to Share its sleepiness with "+giver->name+" but it failed!");
				}
            }
            else
                ret.push_back(giver->name+" tried to make "+taker->name+" fall asleep but it failed!");
        }
        else if (effect==Move::Protection)
        {
            if (atk==order[1] || atk->state.lastMoveUsed==id)
            {
                atk->state.lastMoveUsed = 0;
                atk->state.protectUsedLast = false;
                ret.push_back("But it failed!");
            }
            else;
            {
                atk->state.lastMoveUsed = id;
                atk->state.protectUsedLast = true;
            }
        }
        else
            atk->state.protectUsedLast = false; //catch all to make sure reset occurs

        if (effect==Move::Substitute)
        {
            taker->curHp -= taker->stats.hp/4;
            if (taker->curHp<=0)
            {
                taker->curHp = 0;
                ret.push_back(taker->name+" tried to create a substitute but killed itself!");
            }
            else if (atk->state.subHealth>0)
                ret.push_back(taker->name+" tried to create a substitute but already has one!");
            else
            {
                ret.push_back(taker->name+" created a substitute!");
                atk->state.subHealth = taker->stats.hp/4;
            }
        }
        else if (effect==Move::Safegaurd)
        {
            atk->state.ailSafe = true;
            ret.push_back(taker->name+"'s party is now blissfully ignorant");
        }
        else if (effect==Move::HealBell)
        {
            atk->healAils(true);
            ret.push_back(taker->name+"'s party was made healthy!");
        }
        else if (effect==Move::CritUp)
        {
            taker->stages.crit += intensity;
            taker->stages.crit = (taker->stages.crit>6)?(6):(taker->stages.crit);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s criticalness increased!");
        }
        else if (effect==Move::AtkUp)
        {
            taker->stages.atk += intensity;
            taker->stages.atk = (taker->stages.atk>6)?(6):(taker->stages.atk);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s attack rose!");
        }
        else if (effect==Move::DefUp)
        {
            taker->stages.def += intensity;
            taker->stages.def = (taker->stages.def>6)?(6):(taker->stages.def);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s defense rose!");
        }
        else if (effect==Move::AccUp)
        {
            taker->stages.acc += intensity;
            taker->stages.acc = (taker->stages.acc>6)?(6):(taker->stages.acc);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s accuracy rose!");
        }
        else if (effect==Move::EvdUp)
        {
            taker->stages.evade += intensity;
            taker->stages.evade = (taker->stages.evade>6)?(6):(taker->stages.evade);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s evasiveness rose!");
        }
        else if (effect==Move::SpdUp)
        {
            taker->stages.spd += intensity;
            taker->stages.spd = (taker->stages.spd>6)?(6):(taker->stages.spd);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s speed rose!");
        }
        else if (effect==Move::SpAtkUp)
        {
            taker->stages.spAtk += intensity;
            taker->stages.spAtk = (taker->stages.spAtk>6)?(6):(taker->stages.spAtk);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s special attack rose!");
        }
        else if (effect==Move::SpDefUp)
        {
            taker->stages.spDef += intensity;
            taker->stages.spDef = (taker->stages.spDef>6)?(6):(taker->stages.spDef);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s special defense rose!");
        }
        else if (effect==Move::CritDown)
        {
            taker->stages.crit -= intensity;
            taker->stages.crit = (taker->stages.crit<-6)?(-6):(taker->stages.crit);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s criticalness fell!");
        }
        else if (effect==Move::AtkDown)
        {
            taker->stages.atk -= intensity;
            taker->stages.atk = (taker->stages.atk<-6)?(-6):(taker->stages.atk);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s attack fell!");
        }
        else if (effect==Move::DefDown)
        {
            taker->stages.def -= intensity;
            taker->stages.def = (taker->stages.def<-6)?(-6):(taker->stages.def);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s defense fell!");
        }
        else if (effect==Move::AccDown)
        {
        	if (taker->curAbility==Peoplemon::Observant)
				ret.push_back(taker->name+"'s Observant prevents accuracy loss!");
			else
            {
            	taker->stages.acc -= intensity;
				taker->stages.acc = (taker->stages.acc<-6)?(-6):(taker->stages.acc);
				taker->recalcStats(game);
				ret.push_back(taker->name+"'s accuracy fell!");
            }
        }
        else if (effect==Move::EvdDown)
        {
            taker->stages.evade -= intensity;
            taker->stages.evade = (taker->stages.evade<-6)?(-6):(taker->stages.evade);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s evasiveness fell!");
        }
        else if (effect==Move::SpdDown)
        {
            taker->stages.spd -= intensity;
            taker->stages.spd = (taker->stages.spd<-6)?(-6):(taker->stages.spd);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s speed fell!");
        }
        else if (effect==Move::SpAtkDown)
        {
            taker->stages.spAtk -= intensity;
            taker->stages.spAtk = (taker->stages.spAtk<-6)?(-6):(taker->stages.spAtk);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s special attack fell!");
        }
        else if (effect==Move::SpDefDown)
        {
            taker->stages.spDef -= intensity;
            taker->stages.spDef = (taker->stages.spDef<-6)?(-6):(taker->stages.spDef);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s special defense fell!");
        }
        else if (effect==Move::Recoil)
        {
            double rec = damage*(100.0/double(intensity));
            taker->curHp -= rec;
            if (taker->curHp<0)
                taker->curHp = 0;
            ret.push_back(taker->name+" hurt itself too!");
        }
        else if (effect==Move::Suicide)
        {
            taker->curHp = 0;
            ret.push_back(taker->name+" killed itself!");
        }
        else if (effect==Move::Counter)
        {
            double dmg = (game->moveList[id].isSpecial)?(0):(damage*2);
            if (dmg>0)
            {
                taker->curHp -= dmg;
                if (taker->curHp<0)
                    taker->curHp = 0;
                ret.push_back(taker->name+" was hurt by Counter!");
            }
        }
        else if (effect==Move::MirrorCoat)
        {
            double dmg = (game->moveList[id].isSpecial)?(damage*2):(0);
            if (dmg>0)
            {
                taker->curHp -= dmg;
                if (taker->curHp<0)
                    taker->curHp = 0;
                ret.push_back(taker->name+" was hurt by Mirror Coat!");
            }
        }
    }

    lastMoveHit = true;
    return ret;
}

void BattleState::transitionScreen()
{
	int trans = Random(0,1);
	trans = 1;
	Image bgndData = game->mainWindow.capture();
	Texture bgndTxtr;
	Sprite bgnd;

	bgndTxtr.loadFromImage(bgndData);
	bgnd.setTexture(bgndTxtr);


    if (trans==0)
    {
    	CircleShape circle;
		circle.setFillColor(Color(Random(0,255),Random(0,255),Random(0,255)));
		circle.setRadius(1);
		circle.setPosition(Properties::ScreenWidth/2,Properties::ScreenHeight/2);
		circle.setOutlineColor(Color(Random(0,255),Random(0,255),Random(0,255)));
		circle.setOutlineThickness(8);
		int t = 0;

		for (int r = 1; r<510; r += 6)
		{
			circle.setOrigin(circle.getRadius(),circle.getRadius());
			circle.setRadius(r);
			t++;
			if (t%12==0)
			{
				circle.setFillColor(Color(Random(0,255),Random(0,255),Random(0,255)));
				circle.setOutlineColor(Color(Random(0,255),Random(0,255),Random(0,255)));
			}
			game->mainWindow.draw(bgnd);
			game->mainWindow.draw(circle);
			game->mainWindow.display();
			sleep(milliseconds(10));
		}
    }
    else
	{
		TextureReference ballTxtr = imagePool.loadResource(Properties::BattleImagePath+"battleBall.png");
		Sprite ball;
		CircleShape coverer;
		RectangleShape cover(Vector2f(Properties::ScreenWidth,Properties::ScreenHeight));

		ball.setTexture(*ballTxtr);
		ball.setScale(0,0);
		ball.setPosition(Properties::ScreenWidth/2,Properties::ScreenHeight/2);
		coverer.setFillColor(Color::Black);
		coverer.setRadius(26);
		cover.setFillColor(Color::Transparent);

		game->mainWindow.clear();
		game->mainWindow.draw(bgnd);
		game->mainWindow.display();

		for (int x = -25; x<Properties::ScreenWidth+25; x += 7)
		{
            for (int i = 0; i<Properties::ScreenHeight/50; ++i)
			{
				if (i%2==0)
					coverer.setPosition(x,i*50);
				else
					coverer.setPosition(Properties::ScreenWidth-x,i*50);
				game->mainWindow.draw(coverer);
			}
			float s = float(x+25)/float(Properties::ScreenWidth);
			ball.setScale(s,s);
			ball.setOrigin(float(ballTxtr->getSize().x)*s/2,float(ballTxtr->getSize().y)*s/2);
			game->mainWindow.draw(ball);
			game->mainWindow.display();
			sleep(milliseconds(5));
		}

		for (int a = 0; a<220; a += 5)
		{
			cover.setFillColor(Color(255,255,255,a));
			game->mainWindow.draw(cover);
			game->mainWindow.display();
			sleep(milliseconds(15));
		}
	}
}

void BattleState::playIntroAnim(Battler* b)
{
    Animation* anim = (b==player)?(&playerAnims.sendOut):(&opponentAnims.sendOut);
    anim->play();

    while (!anim->finished())
    {
        if (finishFrame())
        {
            game->data.gameClosedFlag = true;
            return;
        }

        anim->update();

        game->mainWindow.draw(background);
        anim->draw(&game->mainWindow);
        renderQueue();
        opBox.draw(&game->mainWindow);
        playerBox.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(30));
    }
}

void BattleState::renderQueue()
{
    for (unsigned int i = 0; i<toDraw.size(); ++i)
    {
        toDraw[i]->update();
        toDraw[i]->draw(&game->mainWindow);
    }
}

void BattleState::playSwitchAnim(Battler* b, Battler* o, int curPpl, int newPpl)
{
    PeoplemonAnimations temp;
    int i = (b==order[0])?(0):(1);
    int j = (i==0)?(1):(0);
    bool isPlayer = b==player;
    temp.load(game,b->getPeoplemon()->at(newPpl),o->getPeoplemon()->at(o->getCurrentPeoplemon()),isPlayer);

    anims[i]->comeIn.play();
    while (!anims[i]->comeIn.finished())
    {
        anims[i]->comeIn.update();

        game->mainWindow.draw(background);
        anims[i]->comeIn.draw(&game->mainWindow);
        anims[j]->still.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(30));
    }
    sleep(milliseconds(750)); //for natural pause

    if (b->getPeoplemon()->at(curPpl).curAbility==Peoplemon::Forgiving)
	{
		for (int i = 0; i<4; ++i)
			b->getPeoplemon()->at(curPpl).curAils[i] = Peoplemon::None;
	}

    temp.sendOut.play();
    while (!temp.sendOut.finished())
    {
        temp.sendOut.update();

        game->mainWindow.draw(background);
        temp.sendOut.draw(&game->mainWindow);
        anims[j]->still.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(30));
    }

    if (isPlayer)
	{
		playerAnims = temp;
		opponentAnims.load(game,opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()),player->getPeoplemon()->at(player->getCurrentPeoplemon()),false);
	}
	else
	{
		opponentAnims = temp;
		playerAnims.load(game,player->getPeoplemon()->at(player->getCurrentPeoplemon()),opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()),true);
	}
	toDraw.push_back(&anims[i]->still);
	toDraw.push_back(&anims[j]->still);

	if (b->getPeoplemon()->at(b->getCurrentPeoplemon()).curAbility==Peoplemon::Bud)
	{
		b->getPeoplemon()->at(b->getCurrentPeoplemon()).curAbility = o->getPeoplemon()->at(o->getCurrentPeoplemon()).curAbility;
		displayMessage(b->getPeoplemon()->at(b->getCurrentPeoplemon()).name+" is a Bud and copied "+o->getPeoplemon()->at(o->getCurrentPeoplemon()).name+"'s "+Peoplemon::abilityTexts[b->getPeoplemon()->at(b->getCurrentPeoplemon()).curAbility].first+"!");
	}
	else
		b->getPeoplemon()->at(b->getCurrentPeoplemon()).curAbility =  game->peoplemonList[b->getPeoplemon()->at(b->getCurrentPeoplemon()).id].specialAbilityId;
}

void BattleState::playAttackAnim(Battler* b, int moveId)
{
    int m = 0;
    for (; m<4; m++)
    {
        if (b->getPeoplemon()->at(b->getCurrentPeoplemon()).moves[m].id==moveId)
            break;
    }
    int i = (b==order[0])?(0):(1);

    anims[i]->moves[m].attacker.setFrame(0);
    anims[i]->moves[m].defender.setFrame(0);
    anims[i]->moves[m].background.setFrame(0);
    anims[i]->moves[m].foreground.setFrame(0);
    anims[i]->moves[m].attacker.play();
    anims[i]->moves[m].background.play();
    anims[i]->moves[m].foreground.play();

    while (!anims[i]->moves[m].attacker.finished() || !anims[i]->moves[m].background.finished() || !anims[i]->moves[m].foreground.finished())
    {
    	if (finishFrame())
		{
            game->data.gameClosedFlag = true;
            return;
		}

        anims[i]->moves[m].attacker.update();
        anims[i]->moves[m].background.update();
        anims[i]->moves[m].foreground.update();

        game->mainWindow.draw(background);
        opBox.draw(&game->mainWindow);
        playerBox.draw(&game->mainWindow);
        anims[i]->moves[m].background.draw(&game->mainWindow);
        anims[i]->moves[m].attacker.draw(&game->mainWindow);
        anims[i]->moves[m].defender.draw(&game->mainWindow);
        anims[i]->moves[m].foreground.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(30));
    }

    opBox.update(opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()));
    playerBox.update(player->getPeoplemon()->at(player->getCurrentPeoplemon()));
    anims[i]->moves[m].defender.play();

    while (!anims[i]->moves[m].defender.finished())
	{
		if (finishFrame())
		{
            game->data.gameClosedFlag = true;
            return;
		}

		anims[i]->moves[m].defender.update();
		opBox.update();
		playerBox.update();

		game->mainWindow.draw(background);
        opBox.draw(&game->mainWindow);
        playerBox.draw(&game->mainWindow);
        anims[i]->moves[m].background.draw(&game->mainWindow);
        anims[i]->moves[m].attacker.draw(&game->mainWindow);
        anims[i]->moves[m].defender.draw(&game->mainWindow);
        anims[i]->moves[m].foreground.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(30));
	}
    renderStatic();
}

bool BattleState::playerWon()
{
    return playerWinned;
}
