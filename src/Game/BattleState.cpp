#include "Game/BattleState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Battle/PlayerBattler.hpp"
#include "Game/EvolveState.hpp"
#include "Game/DeleteMoveState.hpp"
#include "Game/NicknameState.hpp"
#include <cmath>
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
    opponent->setIsWild(opName.find("WILD")!=string::npos);
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
    ballT = animPool.loadResource(Properties::MiscAnimationPath+"Ball5/Throw.anim");
    ballB = animPool.loadResource(Properties::MiscAnimationPath+"Ball5/Break.anim");
    ballR = animPool.loadResource(Properties::MiscAnimationPath+"Ball5/Rock.anim");
    ballF = animPool.loadResource(Properties::MiscAnimationPath+"Ball5/Fall.anim");
    ballC = animPool.loadResource(Properties::MiscAnimationPath+"Ball5/Click.anim");
    ballThrow.setSource(ballT);
    ballBreak.setSource(ballB);
    ballRock.setSource(ballR);
    ballFall.setSource(ballF);
    ballClick.setSource(ballC);
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

	int turnsElapsed = 0;
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
    bool applyAfterTurn[2] = {true,true}; //whether or not to apply after turn effects like hold items. Used when peoplemon faint or are switched out
    //battle loop
    while (true)
    {
        //render everything first
        renderStatic();
        player->recalcStats(game);
        opponent->recalcStats(game);
        applyAfterTurn[0] = applyAfterTurn[1] = true;

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
                float opSpd = (op.hasAilment(Peoplemon::Annoyed)?(op.stats.spd/4):(op.stats.spd));
                float pSpd = (p.hasAilment(Peoplemon::Annoyed)?(p.stats.spd/4):(p.stats.spd));
                if (p.holdItem==58 && p.curHp<=p.stats.hp/4) //speed juice
				{
					displayMessage(p.name+"'s Speed Juice increased Speed!");
					if (shouldClose())
						return true;
					pSpd *= 1.1;
					player->getPeoplemon()->at(player->getCurrentPeoplemon()).holdItem = 0;
				}
				if (op.holdItem==58 && op.curHp<=op.stats.hp/4) //speed juice
				{
					displayMessage(op.name+"'s Speed Juice increased Speed!");
					if (shouldClose())
						return true;
					opSpd *= 1.1;
					opponent->getPeoplemon()->at(opponent->getCurrentPeoplemon()).holdItem = 0;
				}
				pFirst = !(opMoveP>pMoveP || (opMoveP==pMoveP && pSpd>=opSpd));
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
        renderStatic();

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
					order[i]->setCurrentPeoplemon(turns[i].id); //reset
					if (shouldClose())
						return true;
					continue;
				}
				if (order[i]->getPeoplemon()->at(turns[i].id).hasAilment(Peoplemon::Trapped))
				{
					displayMessage(order[i]->getPeoplemon()->at(turns[i].id).name+" was Trapped and could not switch out!");
					order[i]->setCurrentPeoplemon(turns[i].id);
					if (shouldClose())
						return true;
					continue;
				}
				if (order[i]->getPeoplemon()->at(turns[i].id).curAbility==Peoplemon::Forgiving && order[i]->getPeoplemon()->at(turns[i].id).hasAtLeastOneAilment())
				{
					for (int z = 0; z<4; ++z)
					{
						order[i]->getPeoplemon()->at(turns[i].id).curAils[z] = Peoplemon::None;
					}
					displayMessage(order[i]->getPeoplemon()->at(turns[i].id).name+"'s Forgiving ability cured it of all ailments!");
					if (shouldClose())
						return true;
				}

				applyAfterTurn[i] = false;
				if (order[i]==player)
				{
					if (find(sentIn.begin(),sentIn.end(),turns[i].id)==sentIn.end())
						sentIn.push_back(turns[i].id);
					displayMessage("That's enough, "+order[i]->getPeoplemon()->at(turns[i].id).name+"!");
					if (shouldClose())
						return true;
				}

                playSwitchAnim(order[i],order[j],turns[i].id,order[i]->getCurrentPeoplemon());
                if (shouldClose())
                    return true;

                game->hud.displayMessage("");
                renderStatic();
                if (shouldClose())
                    return true;
            }
            else if (turns[i].type==Turn::Item)
            {
                if (order[i]!=player)
				{
					displayMessage(getItemLine(turns[i].id));
					if (shouldClose())
						return true;
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
				else if (turns[i].id>=5 && turns[i].id<=17)
				{
					if (opponentName.find("WILD")==string::npos)
					{
						if (turns[i].id==15)
						{
							game->player.addStoredPeoplemon(order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()));
							displayMessage(getPeoplemon(order[j],order[j]->getCurrentPeoplemon()).name+" was Cloned into the PC!");
							if (shouldClose())
								return true;
						}
						else
						{
							displayMessage("Kidnapping Peoplemon isn't legal, don't try it again creep");
							if (shouldClose())
								return true;
						}
					}
					else
					{
						cout << "Peopleball with id: " << turns[i].id << " used\n";
						bool skip = false;
						if (turns[i].id==13)
						{
							order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).curHp -= order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).stats.hp/16;
							renderStatic();
							displayMessage("The Questionable Abuse Ball verbally abused "+order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).name+" and caused damage!");
							if (shouldClose())
								return true;
							if (order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).curHp<=0)
							{
								order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).curHp = 0;
								bool done = doFaint(i,j);
								if (shouldClose())
									return true;
								if (done)
									return false;
								skip = true;
							}
						}
						if (!skip)
						{
							double curHp = order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).curHp;
							double maxHp = order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).stats.hp;
							double rate = 48;
							double status = (order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).hasAtLeastOneAilment())?(2):(1); //verify
							double ball = order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()).getBallBonus(game,turns[i].id,turnsElapsed,order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).level);
							double a = ((3*maxHp-2*curHp)*rate*ball*status)/(maxHp*3);
							double b = 1048560 / sqrt(sqrt(16711680/a));
							cout << "b: " << b << endl;

							int shakes = 0;
							for (int z = 0; z<4; ++z)
							{
								int r = Random(0,65535);
								if (r<b)
									shakes++;
							}
							cout << "Shakes: " << shakes << endl;

							playPeopleballAnimations(shakes,turns[i].id);
							if (shouldClose())
								return true;

							if (shakes==4)
							{
								PeoplemonRef ppl = order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon());
								displayMessage("Gotcha! "+ppl.name+" was caught!");
								if (shouldClose())
									return true;
								if (game->peoplemonList[ppl.id].numCaught==0)
								{
									displayMessage(ppl.name+"'s data was added to the Peopledex");
									if (shouldClose())
										return true;
								}
								game->peoplemonList[ppl.id].numCaught++;
								game->peoplemonList[ppl.id].numSeen++;

								NicknameState* state = new NicknameState(game, &ppl);
                                if (game->runState(state,true))
									return true;

								if (game->player.getCurrentPeoplemon()->size()<6)
									game->player.getCurrentPeoplemon()->push_back(ppl);
								else
								{
									game->player.addStoredPeoplemon(ppl);
									displayMessage(ppl.name+" was sent to the PC!");
									if (shouldClose())
										return true;
								}
								return false;
							}
							else
							{
								//TODO - play breakout animation
								displayMessage("Crap! I thought I killed it that time!");
								if (shouldClose())
									return true;
							}
						}
					}
				}
            }
            else if (turns[i].type==Turn::Move)
            {
                PeoplemonRef attacker = getPeoplemon(order[i],order[i]->getCurrentPeoplemon());
                vector<string> lines;
                if (attacker.hasAilment(Peoplemon::Confused))
                {
                    displayMessage(attacker.name+" is Confused!");
                    if (shouldClose())
                        return true;

                    if (Random(0,100)<35 || attacker.turnsConfused>=4)
                    {
                        displayMessage(attacker.name+" snapped out of Confusion!");
                        if (shouldClose())
                            return true;

                        order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).removePassiveAilment(Peoplemon::Confused);
                        order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsConfused = 0;
                    }
                    else if (Random(0,100)<50)
                    {
                        displayMessage(attacker.name+" hurt itself in Confusion!");
                        if (shouldClose())
                            return true;
                        applyMove(order[i],order[i],80);
                        renderStatic();
                        if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp<=0)
						{
							bool done = doFaint(j,i);
							if (shouldClose())
								return true;
							if (done)
								return false;
						}
                        goto noMove;
                    }
                }
                if (attacker.hasAilment(Peoplemon::Annoyed))
                {
                    displayMessage(attacker.name+" is Annoyed!");
                    if (shouldClose())
                        return true;

                    if (Random(0,100)<25)
                    {
                        displayMessage(attacker.name+" is too Annoyed to move!");
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
                        order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsWithAil = 0;
                    }
                    else
                    {
                        displayMessage(attacker.name+" is Sleeping!");
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
                        displayMessage(attacker.name+" is Frozen!");
                        if (shouldClose())
                            return true;
                        goto noMove;
                    }
                }
                if (attacker.hasAilment(Peoplemon::Distracted))
				{
					displayMessage(attacker.name+" is distracted!");
					if (shouldClose())
						return true;
					goto noMove;
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
                	applyAfterTurn[j] = false;
                    shouldStop = true;
                    bool done = doFaint(i,j);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
                }
                if (getPeoplemon(order[i],order[i]->getCurrentPeoplemon()).curHp<=0) //attacker fainted
                {
                	applyAfterTurn[i] = false;
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

        //apply end turn ailments and other effects here
        order[0] = opponent;
        order[1] = player;
        for (int i = 0; i<2; ++i)
        {
        	if (!applyAfterTurn[i]) //skip if not supposed to do end turn stuff
				continue;

            int j = (i==0)?(1):(0);
            PeoplemonRef ppl = getPeoplemon(order[i],order[i]->getCurrentPeoplemon());

			if (ppl.holdItem==50) //bag of goldfish
			{
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp += order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp/16;
                if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp>order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp)
					order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp;
                displayMessage(ppl.name+"'s Bag of Goldfish restored health!");
			}
			else if (ppl.holdItem==56) //sketchy sack
			{
                if (ppl.curHp<=ppl.stats.hp/2)
				{
					displayMessage(ppl.name+"was hurt by its Sketchy Sack!");
					if (shouldClose())
						return true;
					order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp -= ppl.stats.hp/16;
					order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).holdItem = 0;
					renderStatic();
					if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp<=0)
					{
						order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = 0;
						bool done = doFaint(j,i);
						if (shouldClose())
							return true;
						if (done)
							return false;
						continue;
					}
				}
			}
			else if (ppl.holdItem==57 && ppl.curHp<=ppl.stats.hp/2) //goldfish cracker
			{
				displayMessage(ppl.name+" ate a Goldfish Cracker and regained health!");
				if (shouldClose())
					return true;
				order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp += ppl.stats.hp/8;
				if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp > ppl.stats.hp)
					order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = ppl.stats.hp;
				order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).holdItem = 0;
				renderStatic();
			}
			else if (ppl.holdItem==61 && ppl.hasAilment(Peoplemon::Sleep)) //wake up belle
			{
				displayMessage(ppl.name+"'s Wake Up Belle shook it awake!");
				if (shouldClose())
					return true;
				order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curAils[0] = Peoplemon::None;
				renderStatic();
			}
            if (ppl.hasAilment(Peoplemon::Frustrated))
            {
                displayMessage(ppl.name+" is hurt by Frustration!");
                if (shouldClose())
                    return true;
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp -= order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp/16;
                renderStatic();
                if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp<=0)
                {
                    order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = 0;
                    bool done = doFaint(j,i);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
					continue;
                }
            }
            if (ppl.hasAilment(Peoplemon::Stolen))
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

                if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp<=0)
                {
                    bool done = doFaint(j,i);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
					continue;
                }
            }
            if (ppl.hasAilment(Peoplemon::Sticky))
            {
                displayMessage(ppl.name+" is hurt from being sticky!");
                if (shouldClose())
                    return true;

                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp -= order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsWithAil*order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp/16;
                renderStatic();
                if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp<0)
                {
                    order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = 0;
                    bool done = doFaint(i,j);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
                    continue;
                }
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsWithAil++;
            }
            if (ppl.holdItem==55) //super tiny mini fridge
			{
				displayMessage(ppl.name+" has a Super Tiny Mini Fridge!");
				applyMove(order[i],order[j],81);
				order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = 0; //TODO - play animation here
				renderStatic();
				bool done = doFaint(j,i);
				if (shouldClose())
					return true;
				if (getPeoplemon(order[j],order[j]->getCurrentPeoplemon()).curHp<=0) //they fainted
                {
                    bool done = doFaint(i,j);
                    if (shouldClose())
                        return true;
                    if (done)
                        return false;
					applyAfterTurn[j] = false;
                }
                if (done)
					return false;
				continue;
			}
            if (ppl.hasAilment(Peoplemon::Sleep))
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsWithAil++;
            if (ppl.hasAilment(Peoplemon::Confused))
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsConfused++;
			if (ppl.hasAilment(Peoplemon::Trapped))
				order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).removePassiveAilment(Peoplemon::Trapped);
			if (ppl.hasAilment(Peoplemon::Distracted))
				order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).removePassiveAilment(Peoplemon::Distracted);
			for (unsigned int k = 0; k<order[i]->getPeoplemon()->size(); ++k)
			{
				if (order[i]->getPeoplemon()->at(k).hasAilment(Peoplemon::Guarded))
				{
					order[i]->getPeoplemon()->at(k).turnsWithAil++;
					if (order[i]->getPeoplemon()->at(k).turnsWithAil>=5)
					{
						for (unsigned int l = 0; l<order[i]->getPeoplemon()->size(); ++l)
						{
							if (order[i]->getPeoplemon()->at(l).curAils[0]==Peoplemon::Guarded)
								order[i]->getPeoplemon()->at(l).curAils[0] = Peoplemon::None;
						}
						displayMessage(ppl.name+"'s party was no longer Guarded!");
						if (shouldClose())
							return true;
					}
				}
			}
        }
        turnsElapsed++;
    } //end main loop

    return false;
}

void BattleState::renderBase()
{
    playerAnims.still.update();
    opponentAnims.still.update();

    game->mainWindow.clear();
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

        game->mainWindow.clear();
		game->mainWindow.draw(background);
        renderQueue();
        opBox.draw(&game->mainWindow);
        playerBox.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(50));
    }
}

void BattleState::renderStatic(bool rs, bool up, bool op)
{
	if (op)
		opBox.update(getPeoplemon(opponent,opponent->getCurrentPeoplemon()),true);
    if (up)
		playerBox.update(getPeoplemon(player,player->getCurrentPeoplemon()),true);
    while (!opBox.barGood() || !playerBox.barGood())
    {
        opBox.update();
        playerBox.update();
        playerAnims.still.update();
        opponentAnims.still.update();

        game->mainWindow.clear();
		game->mainWindow.draw(background);
        if (rs)
        {
        	playerAnims.still.draw(&game->mainWindow);
			opponentAnims.still.draw(&game->mainWindow);
        }
        renderQueue();
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

string BattleState::getItemLine(int id)
{
	return opponentName+" used a "+game->itemList[id].name+"!";
}

string BattleState::getMoveLine(Battler* b, int id)
{
    return b->getPeoplemon()->at(b->getCurrentPeoplemon()).name+" used "+game->moveList[id].name+"!";
}

bool BattleState::doFaint(int alive, int dead)
{
	int i = alive;
	int j = dead;
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

        game->mainWindow.clear();
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
    	for (unsigned int k = 0; k<sentIn.size(); ++k)
		{
			if (order[i]->getPeoplemon()->at(sentIn[k]).curHp==0)
			{
				sentIn.erase(sentIn.begin()+k);
				k--;
			}
		}

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
			renderStatic(false,true,false);
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

			game->mainWindow.clear();
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
            	game->player.alterMoney(prizeMoney);
                displayMessage(game->player.getName()+" won "+intToString(prizeMoney)+" monies!");
                if (shouldClose())
                    return true;
            }
            for (unsigned int i = 0; i<opponent->getPeoplemon()->size(); ++i)
				game->peoplemonList[opponent->getPeoplemon()->at(i).id].numSeen++;
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
	double acc = attacker.stats.acc;
	if (attacker.holdItem==52 && game->moveList[id].acc!=0 && !game->moveList[id].targetIsSelf)
	{
		acc *= 1.1;
		ret.push_back(attacker.name+"'s Glasses increase Accuracy!");
	}
    double multiplier = 1;
    bool hit = (Random(0,100)<game->moveList[id].acc*acc/defender.stats.evade) || game->moveList[id].acc==0;
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
		ret.push_back(attacker.name+" does more damage with Intelligent moves as an Edumucator!");
	}

    bool isSpecial = game->moveList[id].isSpecial;
    double atkS = (isSpecial)?(attacker.stats.spAtk):(attacker.stats.atk);
    if (attacker.hasAilment(Peoplemon::Frustrated) && !isSpecial)
        atkS /= 2;
	if (attacker.holdItem==51)
	{
		atkS /= 2;
		ret.push_back(attacker.name+"'s Backwards Hoodie reduces Attack!");
	}
	if (attacker.holdItem==53 && isSpecial && !game->moveList[id].targetIsSelf)
	{
		atkS *= 1.1;
		ret.push_back(attacker.name+"'s Spoon increases Special Attack!");
	}
	if (attacker.holdItem==54 && !isSpecial && !game->moveList[id].targetIsSelf)
	{
		atkS *= 1.1;
		ret.push_back(attacker.name+"'s Slapping Glove increases Attack!");
	}
	if (attacker.holdItem==59 && attacker.curHp<=attacker.stats.hp/4 && power>0.1)
	{
		atkS *= 1.1;
		ret.push_back(attacker.name+"'s Power Juice boosted it's attack!");
		atk->getPeoplemon()->at(atk->getCurrentPeoplemon()).holdItem = 0;
	}
    double defS = (isSpecial)?(defender.stats.spDef):(defender.stats.def);
    if (defender.holdItem==60 && defender.curHp<=defender.stats.hp/4 && power>0.1)
	{
		defS *= 1.1;
		ret.push_back(defender.name+"'s Iced Tea made it more resilient!");
		def->getPeoplemon()->at(def->getCurrentPeoplemon()).holdItem = 0;
	}
    double damage = (power>0.1)?((((2*double(attacker.level)+10)/250)*(atkS/defS)*power+2)*multiplier):(0);
    cout << "Damage was: " << damage << endl;

	atk->setLastDamageDealt(damage); //for ai
	def->setLastDamageTaken(damage);

    if (hit)
	{
		if (defender.holdItem==51 && Random(0,100)<25 && !attacker.hasAilment(Peoplemon::Confused) && damage>0.1)
		{
			ret.push_back(defender.name+"'s Backwards Hoodie Confused "+attacker.name+"!");
			attacker.addPassiveAilment(Peoplemon::Confused);
		}
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

	if (taker->curAbility==Peoplemon::Sassy && game->moveList[id].makesContact && hit)
	{
		int dmg = giver->stats.hp/16;
		giver->curHp -= dmg;
		if (giver->curHp<0)
			giver->curHp = 0;
		ret.push_back(giver->name+" took damage because "+taker->name+" is Sassy!");
	}
	if (taker->curAbility==Peoplemon::Opinionated && giver->curAils[0]==Peoplemon::None && Random(0,100)<=20 && game->moveList[id].makesContact && hit)
	{
		giver->curAils[0] = Peoplemon::Annoyed;
		ret.push_back(giver->name+" was Annoyed because "+taker->name+" is Opinionated!");
	}

    if (Random(0,100)<=game->moveList[id].chanceOfEffect && effect!=Move::None)
    {
        bool canGetAils = !def->getPeoplemon()->at(def->getCurrentPeoplemon()).hasAilment(Peoplemon::Guarded) && def->state.subHealth==0;
        if (game->moveList[id].targetIsSelf)
        {
            swap(giver,taker);
            canGetAils = !atk->getPeoplemon()->at(atk->getCurrentPeoplemon()).hasAilment(Peoplemon::Guarded)  && atk->state.subHealth==0;
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
                ret.push_back(taker->name+" was made Sticky!");
                if (reciprocateAil)
				{
					if (giver->curAils[0]==Peoplemon::None)
					{
						giver->curAils[0] = Peoplemon::Sticky;
						ret.push_back(taker->name+" Shared its Stickiness with "+giver->name+"!");
					}
					else
						ret.push_back(taker->name+" tried to Share its Stickiness with "+giver->name+" but if failed!");
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
                ret.push_back(taker->name+" was Frustrated!");
                if (reciprocateAil)
				{
                    if (giver->curAils[0]==Peoplemon::None)
					{
						giver->curAils[0] = Peoplemon::Frustrated;
						ret.push_back(taker->name+" Shared its Frustration with "+giver->name+"!");
					}
					else
						ret.push_back(taker->name+" tried to Share its Frustration with "+giver->name+" but it failed!");
				}
            }
            else
                ret.push_back(giver->name+" tried to Frustrate "+taker->name+" but it failed!");
        }
        else if (effect==Move::Paralyze)
        {
            if (taker->curAils[0]==Peoplemon::None && canGetAils)
            {
                taker->curAils[0] = Peoplemon::Annoyed;
                ret.push_back(taker->name+" was Annoyed!");
                if (reciprocateAil)
				{
                    if (giver->curAils[0]==Peoplemon::None)
					{
						giver->curAils[0] = Peoplemon::Annoyed;
						ret.push_back(taker->name+" Shared its Annoyance with "+giver->name+"!");
					}
					else
						ret.push_back(taker->name+" tried to Share its Annoyance with "+giver->name+" but it failed!");
				}
            }
            else
                ret.push_back(giver->name+" tried to Annoy "+taker->name+" but it failed!");
        }
        else if (effect==Move::Freeze)
        {
            if (taker->curAils[0]==Peoplemon::None && canGetAils)
            {
                taker->curAils[0] = Peoplemon::Frozen;
                ret.push_back(taker->name+" was Frozen!");
                if (reciprocateAil)
				{
                    if (giver->curAils[0]==Peoplemon::None)
					{
						giver->curAils[0] = Peoplemon::Frozen;
						ret.push_back(taker->name+" Shared its Frozenness with "+giver->name+"!");
					}
					else
						ret.push_back(taker->name+" tried to Share its Frozenness with "+giver->name+" but it failed!");
				}
            }
            else
                ret.push_back(giver->name+" tried to Freeze "+taker->name+" but it failed!");
        }
        else if (effect==Move::Confuse)
        {
            if (canGetAils)
            {
                taker->addPassiveAilment(Peoplemon::Confused);
                taker->turnsConfused = 1;
                ret.push_back(taker->name+" was Confused!");
                if (reciprocateAil)
				{
					giver->addPassiveAilment(Peoplemon::Confused);
					ret.push_back(taker->name+" Shared its Confusion with "+giver->name+"!");
				}
            }
            else
                ret.push_back(giver->name+" tried to Confuse "+taker->name+" but it failed!");
        }
        else if (effect==Move::LeechSeed)
        {
            if (canGetAils)
            {
                taker->addPassiveAilment(Peoplemon::Stolen);
                ret.push_back(taker->name+" was Jumped!");
                if (reciprocateAil)
				{
					giver->addPassiveAilment(Peoplemon::Stolen);
					ret.push_back(taker->name+" Shared the Mugging with "+giver->name+"!");
				}
            }
            else
                ret.push_back(giver->name+" tried to Jump "+taker->name+" but it failed!");
        }
        else if (effect==Move::Flinch && atk==order[0])
        {
            if (canGetAils)
            {
                taker->addPassiveAilment(Peoplemon::Distracted);
                ret.push_back(taker->name+" was Distracted!");
            }
            else
                ret.push_back(giver->name+" tried to Distract "+taker->name+" but it failed!");
        }

        if (effect==Move::Trap)
        {
            if (canGetAils)
            {
                taker->addPassiveAilment(Peoplemon::Trapped);
                ret.push_back(taker->name+" was Trapped!");
                if (reciprocateAil)
				{
					giver->addPassiveAilment(Peoplemon::Trapped);
					ret.push_back(taker->name+" Shared its entrapment with "+giver->name+"!");
				}
            }
            else
                ret.push_back(giver->name+" tried to Trap "+taker->name+" but it failed!");
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
                ret.push_back(taker->name+" tried to create a Substitute but killed itself!");
            }
            else if (atk->state.subHealth>0)
                ret.push_back(taker->name+" tried to create a Substitute but already has one!");
            else
            {
                ret.push_back(taker->name+" created a Substitute!");
                atk->state.subHealth = taker->stats.hp/4;
            }
        }
        else if (effect==Move::Safegaurd)
        {
            for (unsigned int k = 0; k<atk->getPeoplemon()->size(); ++k)
			{
				if (atk->getPeoplemon()->at(k).curAils[0]==Peoplemon::None)
					atk->getPeoplemon()->at(k).curAils[0] = Peoplemon::Guarded;
			}
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
            ret.push_back(taker->name+"'s Criticalness increased!");
        }
        else if (effect==Move::AtkUp)
        {
            taker->stages.atk += intensity;
            taker->stages.atk = (taker->stages.atk>6)?(6):(taker->stages.atk);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Attack rose!");
        }
        else if (effect==Move::DefUp)
        {
            taker->stages.def += intensity;
            taker->stages.def = (taker->stages.def>6)?(6):(taker->stages.def);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Defense rose!");
        }
        else if (effect==Move::AccUp)
        {
            taker->stages.acc += intensity;
            taker->stages.acc = (taker->stages.acc>6)?(6):(taker->stages.acc);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Accuracy rose!");
        }
        else if (effect==Move::EvdUp)
        {
            taker->stages.evade += intensity;
            taker->stages.evade = (taker->stages.evade>6)?(6):(taker->stages.evade);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Evasiveness rose!");
        }
        else if (effect==Move::SpdUp)
        {
            taker->stages.spd += intensity;
            taker->stages.spd = (taker->stages.spd>6)?(6):(taker->stages.spd);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Speed rose!");
        }
        else if (effect==Move::SpAtkUp)
        {
            taker->stages.spAtk += intensity;
            taker->stages.spAtk = (taker->stages.spAtk>6)?(6):(taker->stages.spAtk);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Special Attack rose!");
        }
        else if (effect==Move::SpDefUp)
        {
            taker->stages.spDef += intensity;
            taker->stages.spDef = (taker->stages.spDef>6)?(6):(taker->stages.spDef);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Special Defense rose!");
        }
        else if (effect==Move::CritDown)
        {
            taker->stages.crit -= intensity;
            taker->stages.crit = (taker->stages.crit<-6)?(-6):(taker->stages.crit);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Criticalness fell!");
        }
        else if (effect==Move::AtkDown)
        {
            taker->stages.atk -= intensity;
            taker->stages.atk = (taker->stages.atk<-6)?(-6):(taker->stages.atk);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Attack fell!");
        }
        else if (effect==Move::DefDown)
        {
            taker->stages.def -= intensity;
            taker->stages.def = (taker->stages.def<-6)?(-6):(taker->stages.def);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Defense fell!");
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
				ret.push_back(taker->name+"'s Accuracy fell!");
            }
        }
        else if (effect==Move::EvdDown)
        {
            taker->stages.evade -= intensity;
            taker->stages.evade = (taker->stages.evade<-6)?(-6):(taker->stages.evade);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Evasiveness fell!");
        }
        else if (effect==Move::SpdDown)
        {
            taker->stages.spd -= intensity;
            taker->stages.spd = (taker->stages.spd<-6)?(-6):(taker->stages.spd);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Speed fell!");
        }
        else if (effect==Move::SpAtkDown)
        {
            taker->stages.spAtk -= intensity;
            taker->stages.spAtk = (taker->stages.spAtk<-6)?(-6):(taker->stages.spAtk);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Special Attack fell!");
        }
        else if (effect==Move::SpDefDown)
        {
            taker->stages.spDef -= intensity;
            taker->stages.spDef = (taker->stages.spDef<-6)?(-6):(taker->stages.spDef);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Special Defense fell!");
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

        game->mainWindow.clear();
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

        game->mainWindow.clear();
		game->mainWindow.draw(background);
        anims[i]->comeIn.draw(&game->mainWindow);
        anims[j]->still.draw(&game->mainWindow);
        game->hud.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(30));
    }
    sleep(milliseconds(750)); //for natural pause
    toDraw.clear();
    toDraw.push_back(&anims[j]->still);
    displayMessage(getSwitchLine(order[i],order[i]->getCurrentPeoplemon()));

    if (b->getPeoplemon()->at(curPpl).curAbility==Peoplemon::Forgiving)
	{
		for (int i = 0; i<4; ++i)
			b->getPeoplemon()->at(curPpl).curAils[i] = Peoplemon::None;
	}
	opBox.update(getPeoplemon(opponent,opponent->getCurrentPeoplemon()),false);
	playerBox.update(getPeoplemon(player,player->getCurrentPeoplemon()),false);

    temp.sendOut.play();
    while (!temp.sendOut.finished())
    {
        temp.sendOut.update();

        game->mainWindow.clear();
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

		game->mainWindow.clear();
        game->mainWindow.clear();
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

		game->mainWindow.clear();
		game->mainWindow.clear();
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

void BattleState::playPeopleballAnimations(int r, int p)
{
	Animation* playing[2] = {&ballThrow,NULL};
	int rocks = 0;
	enum
	{
		Throwing,
		Falling,
		Shaking,
		Finishing,
		Done
	}state = Throwing;

	ballThrow.play();

	while (state!=Done)
	{
		for (int i = 0; i<2; ++i)
		{
			if (playing[i]!=NULL)
				playing[i]->update();
		}
		if (finishFrame())
		{
			game->data.gameClosedFlag = true;
			return;
		}

		switch (state)
		{
		case Throwing:
			if (playing[0]->finished())
			{
				state = Falling;
				ballFall.play();
				opponentAnims.suckIn.play();
				playing[0] = &ballFall;
                playing[1] = &opponentAnims.suckIn;
			}
			break;

		case Falling:
			if (playing[0]->finished() && playing[1]->finished())
			{
				state = Shaking;
				ballRock.play();
				playing[0] = &ballRock;
				playing[1] = NULL;
			}
			break;

		case Shaking:
			if (playing[0]->finished())
			{
				rocks++;
				if (rocks>=r)
				{
					state = Finishing;
					if (r==4)
					{
						ballClick.play();
						playing[0] = &ballClick;
					}
					else
					{
						ballBreak.play();
						opponentAnims.breakout.play();
						playing[0] = &ballBreak;
						playing[1] = &opponentAnims.breakout;
					}
				}
				else
					ballRock.play();
			}
			break;

		case Finishing:
			if (playing[0]->finished())
			{
				bool done = true;
                if (playing[1]!=NULL)
				{
					if (!playing[1]->finished())
						done = false;
				}
				if (done)
					state = Done;
			}
			break;

		default:
			break;
		}

		game->mainWindow.clear();
		game->mainWindow.draw(background);
		playerBox.draw(&game->mainWindow);
		opBox.draw(&game->mainWindow);
		for (int i = 0; i<2; ++i)
		{
			if (playing[i]!=NULL)
				playing[i]->draw(&game->mainWindow);
		}
		game->hud.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(30));
	}
}

bool BattleState::playerWon()
{
    return playerWinned;
}
