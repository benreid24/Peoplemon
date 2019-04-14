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
    ballT = animPool.loadResource(Properties::MiscAnimationPath+"Ball5/Throw.anim"); //TODO - different balls?
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
    game->data.lastBattleWon = playerWinned;
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

    int runTries = 0;
    bool applyAfterTurn[2] = {true,true}; //whether or not to apply after turn effects like hold items. Used when peoplemon faint or are switched out
    int ballUpTurns = 0; //for volleyball moves
    bool firstTurn = true;

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

            //player turn
            Turn pTurn;
            if (player->state.isCharging) {
                pTurn.id = player->state.lastMoveUsed;
                pTurn.type = Turn::Move;
            }
            else if (player->state.encoreTurnsLeft>0) {
                pTurn.id = player->state.encoreMoveId;
                pTurn.type = Turn::Move;
                player->state.encoreTurnsLeft--;
            }
            else
                pTurn = player->getTurn(op,game);
            if (shouldClose())
                return true;

            //opponent turn
            Turn oTurn;
            if (opponent->state.isCharging) {
                oTurn.id = opponent->state.lastMoveUsed;
                oTurn.type = Turn::Move;
            }
            else if (opponent->state.encoreTurnsLeft>0) {
                oTurn.id = opponent->state.encoreMoveId;
                oTurn.type = Turn::Move;
                opponent->state.encoreTurnsLeft--;
            }
            else
                oTurn = opponent->getTurn(p,game);

            //determine turn order
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
				if (opMoveP==pMoveP && firstTurn) {
                    if (p.curAbility==Peoplemon::QuickDraw) {
                        pFirst = true;
                        displayMessage(p.name+"'s Quick Draw allows it to attack first!");
                        if (shouldClose())
                            return true;
                    }
                    else if (op.curAbility==Peoplemon::QuickDraw) {
                        pFirst = false;
                        displayMessage(op.name+"'s Quick Draw allows it to attack first!");
                        if (shouldClose())
                            return true;
                    }
				}
            }
            firstTurn = false;

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
        player->state.ballHandled = false;
        opponent->state.ballHandled = false;

        //loop through turns and do them while outputting stuff. declare things here to indicate moves like protect being used
        bool shouldStop = false;
        for (int i = 0; i<2; ++i)
        {
            player->recalcStats(game);
            opponent->recalcStats(game);

            if (shouldStop)
                break;

            int j = (i+1)%2; //to quickly reference the other guy
            toDraw.clear();
            toDraw.push_back(&playerAnims.still);
            toDraw.push_back(&opponentAnims.still);

            if (order[i]->state.healNextPeoplemon) {
                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp += order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp * 0.5;
                if (order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp > order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp)
                    order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stats.hp;
                displayMessage("The weird healing powder fell down and healed "+getPeoplemonName(order[i], order[i]->getCurrentPeoplemon())+"!");
                if (shouldClose())
                    return true;
            }
            order[i]->state.healNextPeoplemon = false;

            if (turns[i].type==Turn::Switch)
            {
                order[i]->state.encoreHit = false;
                order[i]->state.protectUsedLast = false;
                if (doSwitch(order[i], order[j], turns[i].id, &applyAfterTurn[i]))
                    return false;
                if (shouldClose())
                    return true;
            }
            else if (turns[i].type==Turn::Item)
            {
                order[i]->state.encoreHit = false;
                order[i]->state.protectUsedLast = false;
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
								if (doFaint(i,j))
                                    return false;
								if (shouldClose())
									return true;
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
							if (doFaint(j,i))
                                return false;
							if (shouldClose())
								return true;
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
                    if (attacker.turnsUntilWake>0)
                        attacker.turnsUntilWake--;

                    if ((attacker.turnsUntilWake<=0 && Random(0,100)<35) || attacker.turnsWithAil>=4 || attacker.turnsUntilWake==0)
                    {
                        displayMessage(attacker.name+" woke up!");
                        if (shouldClose())
                            return true;
                        order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curAils[0] = Peoplemon::None;
                        order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsWithAil = 0;
                        order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).turnsUntilWake = -1;
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

                lines = applyMove(order[i],order[j],turns[i].id,turns[j].id);
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
                if (order[i]->state.koRevive) { //Death swap
                    applyAfterTurn[j] = false;
                    shouldStop = true;
                    if (doFaint(j, i, true))
                        return false;
                    if (shouldClose())
                        return true;
                }
                if (getPeoplemon(order[j],order[j]->getCurrentPeoplemon()).curHp<=0) //they fainted
                {
                	applyAfterTurn[j] = false;
                    shouldStop = true;
                    if (doFaint(i,j))
                        return false;
                    if (shouldClose())
                        return true;
                }
                if (getPeoplemon(order[i],order[i]->getCurrentPeoplemon()).curHp<=0) //attacker fainted
                {
                	applyAfterTurn[i] = false;
                    shouldStop = true;
                    if (doFaint(i,j))
                        return false;
                    if (shouldClose())
                        return true;
                }

                if (order[i]->state.switchAfterMove) { //baton pass and bump
                    order[i]->state.switchAfterMove = false;

                    int oldPeoplemon = order[i]->getCurrentPeoplemon();
                    order[i]->getSwitchPeoplemon(order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()), game);
                    Stats curStats = order[i]->getPeoplemon()->at(oldPeoplemon).stages;

                    if (doSwitch(order[i], order[j], oldPeoplemon, &applyAfterTurn[i]))
                        return false;

                    order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).stages = curStats;
                    order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).recalcStats(game);

                    game->hud.displayMessage("");
                    renderStatic();
                    if (shouldClose())
                        return true;
                }
                if (order[i]->state.roarUsed) {
                    order[i]->state.roarUsed = false;
                    int oldPeoplemon = order[i]->getCurrentPeoplemon();
                    order[i]->getSwitchPeoplemon(order[j]->getPeoplemon()->at(order[j]->getCurrentPeoplemon()), game);
                    if (doSwitch(order[i], order[j], oldPeoplemon, &applyAfterTurn[i]))
                        return false;
                    game->hud.displayMessage("");
                    renderStatic();
                    if (shouldClose())
                        return true;
                }
                noMove:;
            }
            else //run
            {
                order[i]->state.encoreHit = false;
                order[i]->state.protectUsedLast = false;

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

        if (player->state.ballIsUp || opponent->state.ballIsUp)
            ballUpTurns += 1;
        else
            ballUpTurns = 0;

        //apply end turn ailments and other effects here
        order[0] = opponent;
        order[1] = player;
        for (int i = 0; i<2; ++i)
        {
            int j = (i+1)%2;
            PeoplemonRef ppl = getPeoplemon(order[i],order[i]->getCurrentPeoplemon());

            if (order[i]->state.ballIsUp && (!order[i]->state.ballHandled || ballUpTurns>=3)) {
                order[i]->state.ballIsUp = false;
                ballUpTurns = 0;

                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = 0;
                displayMessage(getPeoplemonName(order[i], order[i]->getCurrentPeoplemon())+" was K.O'd by a Volleyball!");
                if (shouldClose())
                    return true;
                if (doFaint(i,j))
                    return false;
                if (shouldClose())
                    return true;
                continue;
            }

            if (order[i]->state.deathTurnCounter>=0)
                order[i]->state.deathTurnCounter++;
            if (order[i]->state.deathTurnCounter>=3) {
                order[i]->state.deathTurnCounter = -1;

                order[i]->getPeoplemon()->at(order[i]->getCurrentPeoplemon()).curHp = 0;
                displayMessage(getPeoplemonName(order[i], order[i]->getCurrentPeoplemon())+" was K.O'd by the Mark for Death!");

                if (shouldClose())
                    return true;
                if (doFaint(i,j))
                    return false;
                if (shouldClose())
                    return true;
                continue;
            }

        	if (!applyAfterTurn[i]) //skip if not supposed to do end turn stuff
				continue;

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
						if (doFaint(i,j))
                            return false;
                        if (shouldClose())
                            return true;
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
                    if (doFaint(i,j))
                        return false;
                    if (shouldClose())
                        return true;
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
                    if (doFaint(i,j))
                        return false;
                    if (shouldClose())
                        return true;
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
                    if (doFaint(i,j))
                        return false;
                    if (shouldClose())
                        return true;
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
                    if (doFaint(i,j))
                        return false;
                    if (shouldClose())
                        return true;
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

bool BattleState::doSwitch(Battler* switcher, Battler* opp, int oldIndex, bool* fxAfterTurnFlag)
{
    switcher->state.protectUsedLast = false;
    switcher->state.deathTurnCounter = -1;

    if (switcher->getPeoplemon()->at(oldIndex).hasAilment(Peoplemon::Trapped))
    {
        displayMessage(switcher->getPeoplemon()->at(oldIndex).name+" was Trapped and could not switch out!");
        switcher->setCurrentPeoplemon(oldIndex); //reset
        return false;
    }
    if (switcher->getPeoplemon()->at(oldIndex).curAbility==Peoplemon::AlwaysFriendly && switcher->getPeoplemon()->at(oldIndex).hasAtLeastOneAilment())
    {
        for (int z = 0; z<4; ++z)
        {
            switcher->getPeoplemon()->at(oldIndex).curAils[z] = Peoplemon::None;
        }
        displayMessage(switcher->getPeoplemon()->at(oldIndex).name+"'s Always Friendly ability cured it of all ailments!");
        if (shouldClose())
            return false; //unique here b/c retVal is whether all fainted. Calling code still has to check shouldClose()
    }

    *fxAfterTurnFlag = false;
    if (switcher==player)
    {
        if (find(sentIn.begin(),sentIn.end(),oldIndex)==sentIn.end())
            sentIn.push_back(oldIndex);
        displayMessage("That's enough, "+switcher->getPeoplemon()->at(oldIndex).name+"!");
        if (shouldClose())
            return false;
    }

    playSwitchAnim(switcher,opp,oldIndex,switcher->getCurrentPeoplemon());
    if (shouldClose())
        return false;

    switcher->getPeoplemon()->at(switcher->getCurrentPeoplemon()).curAbility = game->peoplemonList[switcher->getPeoplemon()->at(switcher->getCurrentPeoplemon()).id].specialAbilityId;
    if (switcher->state.spikesApplied>0) {
        double fraction = 1.0/8.0;
        if (switcher->state.spikesApplied==2)
            fraction = 1.0/6.0;
        else if (switcher->state.spikesApplied==3)
            fraction = 1.0/4.0;
        double damage = switcher->getPeoplemon()->at(switcher->getCurrentPeoplemon()).stats.hp * fraction;

        switcher->getPeoplemon()->at(switcher->getCurrentPeoplemon()).curHp -= damage;
        displayMessage(switcher->getPeoplemon()->at(switcher->getCurrentPeoplemon()).name+" was hurt by all the Spikes!");
        if (shouldClose())
            return false;

        if (switcher->getPeoplemon()->at(switcher->getCurrentPeoplemon()).curHp <= 0) {
            *fxAfterTurnFlag = false;
            int i = (switcher==order[0]) ? (0):(1);
            int j = (i+1)%2;
            if (doFaint(j,i))
                return true;
        }
    }

    game->hud.displayMessage("");
    renderStatic();
    return false;
}

bool BattleState::doFaint(int alive, int dead, bool chooseRandom)
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
            return false; //same scenario as doSwitch
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
        return false;

    int id = -1;
    if (!chooseRandom)
        id = o->getSwitchPeoplemon(b->getPeoplemon()->at(b->getCurrentPeoplemon()), game);
    else {
        id = o->getRandomFaintedPeoplemon(true);
        o->getPeoplemon()->at(id).curHp = o->state.koReviveHp;
        o->getPeoplemon()->at(id).recalcStats(game, true);
        for (int w = 0; w<4; ++w)
            o->getPeoplemon()->at(id).curAils[w] = Peoplemon::Ailment::None;
    }
    if (shouldClose())
		return false;

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
                return false;
			renderStatic(false,true,false);
			if (shouldClose())
				return false;

            while (getPeoplemon(order[i],sentIn[k]).curXp>=getPeoplemon(order[i],sentIn[k]).nextLvlXp)
            {
                int level = ++order[i]->getPeoplemon()->at(sentIn[k]).level;
                order[i]->getPeoplemon()->at(sentIn[k]).recalcStats(game);
                order[i]->getPeoplemon()->at(sentIn[k]).curXp -= order[i]->getPeoplemon()->at(sentIn[k]).nextLvlXp;

                if (sentIn[k]==order[i]->getCurrentPeoplemon())
                    playerBox.update(getPeoplemon(order[i],sentIn[k]),false);
                displayMessage(getPeoplemon(order[i],sentIn[k]).name+" grew to level "+intToString(level)+"!");
                if (shouldClose())
                    return false;

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
									return false;

								DeleteMoveState* state = new DeleteMoveState(game,&order[i]->getPeoplemon()->at(sentIn[k]),learnSet[m].second);
								if (game->runState(state))
									return false;
								if (sentIn[k]==order[i]->getCurrentPeoplemon())
									anims[i]->load(game,getPeoplemon(order[i],sentIn[k]),getPeoplemon(order[j],order[j]->getCurrentPeoplemon()),true);
							}
							else
							{
								displayMessage(getPeoplemon(order[i],sentIn[k]).name+" learned "+moveName+"!");
								if (shouldClose())
									return false;
							}
						}
                    }
                }

                if (level==game->peoplemonList[order[i]->getPeoplemon()->at(sentIn[k]).id].evolveLevel)
                {
                    EvolveState* s = new EvolveState(game,&order[i]->getPeoplemon()->at(sentIn[k]));
                    if (game->runState(s,false))
                        return false;
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
			return false;

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
				return false;
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
                return false;
            displayMessage(game->player.getName()+" oranged out!");
            if (shouldClose())
                return false;
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
                    return false;
            }
            if (prizeMoney>0)
            {
            	game->player.alterMoney(prizeMoney);
                displayMessage(game->player.getName()+" won "+intToString(prizeMoney)+" monies!");
                if (shouldClose())
                    return false;
            }
            for (unsigned int i = 0; i<opponent->getPeoplemon()->size(); ++i)
				game->peoplemonList[opponent->getPeoplemon()->at(i).id].numSeen++;
            return true;
        }
    }
    renderStatic();
    return false;
}

vector<string> BattleState::applyMove(Battler* atk, Battler* def, int id, int oppId)
{
	cout << "Move " << game->moveList[id].name << " used with power " << game->moveList[id].dmg << endl;

    vector<string> ret;
    PeoplemonRef attacker = getPeoplemon(atk,atk->getCurrentPeoplemon()), defender = getPeoplemon(def,def->getCurrentPeoplemon());

    if (atk->state.encoreHit) {
        atk->state.encoreHit = false;
        atk->state.encoreMoveId = id;
        atk->state.encoreTurnsLeft = 5;
        ret.push_back(attacker.name+" was convinced to do an Encore of "+game->moveList[id].name+"!");
    }

    if (game->moveList[id].effect==Move::FailOnMove64 && atk->state.move64Hit) {
        lastMoveHit = false;
        ret.push_back("Because of its traumatic past "+attacker.name+" was unable to use "+game->moveList[id].name+"!");
        return ret;
    }

    if (game->moveList[id].effect==Move::RandomMove) {
        string moveName = game->moveList[id].name;
        ret.push_back(attacker.name+" used "+moveName+"!");

        while (game->moveList[id].effect==Move::RandomMove) {
            do {
                id = Random(0, game->moveList.rbegin()->first);
            } while (game->moveList.find(id) == game->moveList.end());
        }
        ret.push_back(moveName+" turned into "+game->moveList[id].name+"!");
    }

    if (game->moveList[id].effect==Move::OnlySleeping && defender.curAils[0]!=Peoplemon::Sleep)
    {
        ret.push_back(attacker.name+" tried to use "+game->moveList[id].name+" but it failed because "+defender.name+" is not asleep!");
        return ret;
    }
    else if (game->moveList[id].effect==Move::Charge)
    {
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

    Move::Effect effect = game->moveList[id].effect;
	double power = game->moveList[id].dmg;

	if (effect==Move::Gamble) {
        int roll = Random(1, 20);
        if (roll==1) {
            atk->getPeoplemon()->at(atk->getCurrentPeoplemon()).curHp = 1;
            ret.push_back(attacker.name+" rolled a 1 and was reduced to 1 HP!");
        }
        else if (roll==20) {
            power = 200;
            ret.push_back(attacker.name+" rolled a 20, Attack Power was increased to 200!");
        }
        else {
            power = roll * 5;
            ret.push_back(attacker.name+" rolled a "+intToString(roll)+", Attack Power is "+intToString(power));
        }
    }
    else if (effect==Move::DoubleFamily) {
        power *= atk->getBroPower();
        ret.push_back(attacker.name+"'s Bro Power multiplied its attack!");
    }
    if (attacker.curAbility==Peoplemon::BeefedUp && attacker.curAbility<=attacker.stats.hp*0.25) {
        if (game->moveList[id].isAthletic()) {
            power *= 1.25;
            ret.push_back(attacker.name+" is Beefed Up and Power is increased!");
        }
    }
    else if (attacker.curAbility==Peoplemon::Reserved && attacker.curHp<=attacker.stats.hp*0.25) {
        if (game->moveList[id].isQuiet()) {
            power *= 1.25;
            ret.push_back(attacker.name+" is Reserved and Power is increased!");
        }
    }
    else if (attacker.curAbility==Peoplemon::DukeOfJokes && game->moveList[id].isJokeBased()) {
        power *= 1.5;
        ret.push_back("Power is boosted because "+attacker.name+" is the Duke of Jokes!");
    }

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

    if (attacker.curAbility==Peoplemon::Engaging && effectiveness==0 && power>0.1) {
        effectiveness = 0.25;
        isSuper = isBad = false;
        ret.push_back("Normally "+game->typeList[game->moveList[id].type]+" moves don't affect "+defender.name+" but "+attacker.name+" was so Engaging that it does damage anyways!");
    }

    multiplier *= stab*effectiveness;
    multiplier *= double(Random(85,100))/100;

    Move::Effect oppEffect = Move::None;
    if (oppId>=0)
        oppEffect = game->moveList[oppId].effect;
    if (atk->state.ballSet && (effect==Move::SpikeBall || effect==Move::SwipeBall))
        hit = true;
    atk->state.ballSet = false;

    if (critical && defender.curAbility==Peoplemon::Chillaxed)
	{
		critical = false;
		ret.push_back(defender.name+"'s Chillax prevents critical hits!");
	}
    multiplier *= (critical)?(2):(1);

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
    if (effect==Move::Peanut) {
        if ((defender.id>=58 && defender.id<=60) || (defender.id>=78 && defender.id<=80) || (defender.id>=84 && defender.id<=86) || (defender.id>=126 && defender.id<=127)) {
            ret.push_back(defender.name+" is allergic to peanuts!");
            damage = 9999999999;
        }
        else {
            ret.push_back(defender.name+" ate the peanut and gained 1 hp");
            defender.curHp += 1;
            damage = 0;
        }
    }
    cout << "Damage was: " << damage << endl;

	atk->setLastDamageDealt(damage); //for ai
	def->setLastDamageTaken(damage);

    if (hit)
	{
	    if (id==64)
            def->state.move64Hit = true;
		if (defender.holdItem==51 && Random(0,100)<25 && !attacker.hasAilment(Peoplemon::Confused) && damage>0.1)
		{
			ret.push_back(defender.name+"'s Backwards Hoodie Confused "+attacker.name+"!");
			attacker.addPassiveAilment(Peoplemon::Confused);
		}
		/*if (defender.curAbility==Peoplemon::EasyGoing && damage>=defender.curHp && defender.curHp>1) //TODO - Teach based move ability
		{
			ret.push_back(defender.name+"'s Easy Going nature prevented a one hit KO!");
			def->getPeoplemon()->at(def->getCurrentPeoplemon()).curHp = 1;
		}
		else*/
			def->getPeoplemon()->at(def->getCurrentPeoplemon()).curHp -= damage;
	}
	else
	{
		ret.clear();
        ret.push_back("But it missed!");
        lastMoveHit = false;
        return ret;
	}

    if (def->getPeoplemon()->at(def->getCurrentPeoplemon()).curHp<=0) {
        def->getPeoplemon()->at(def->getCurrentPeoplemon()).curHp = 0;
        if (def->state.endureThisTurn && !def->state.enduredLastTurn) {
            def->getPeoplemon()->at(def->getCurrentPeoplemon()).curHp = 1;
            ret.push_back(defender.name+" Endured!");
        }
    }
    def->state.enduredLastTurn = def->state.endureThisTurn;
    def->state.endureThisTurn = false;

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

    int intensity = game->moveList[id].intensityOfEffect;
    PeoplemonRef* giver = &atk->getPeoplemon()->at(atk->getCurrentPeoplemon());
	PeoplemonRef* taker = &def->getPeoplemon()->at(def->getCurrentPeoplemon());
	bool reciprocateAil = taker->curAbility==Peoplemon::SnackShare && !game->moveList[id].targetIsSelf;

	if (taker->curAbility==Peoplemon::Goon && game->moveList[id].makesContact && hit)
	{
		int dmg = giver->stats.hp/16;
		giver->curHp -= dmg;
		if (giver->curHp<0)
			giver->curHp = 0;
		ret.push_back(giver->name+" took damage because "+taker->name+" is a Goon!");
	}
	/*if (taker->curAbility==Peoplemon::Opinionated && giver->curAils[0]==Peoplemon::None && Random(0,100)<=20 && game->moveList[id].makesContact && hit)
	{
		giver->curAils[0] = Peoplemon::Annoyed;
		ret.push_back(giver->name+" was Annoyed because "+taker->name+" is Opinionated!");
	}*/ //TODO - ailment abilities like Derp Derp

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
						ret.push_back(taker->name+" tried to Share its Stickiness with "+giver->name+" but it failed!");
				}
            }
            else
                ret.push_back(giver->name+" tried to make "+taker->name+" sticky but it failed!");
        }
        else if (effect==Move::Burn)
        {
            if (taker->curAils[0]==Peoplemon::None && canGetAils)
            {
                if (taker->curAbility!=Peoplemon::Classy)
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
                    ret.push_back(giver->name+" tried to Frustrate "+giver->name+" but "+giver->name+"is Classy and cannot be Frustrated!");
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
					giver->turnsConfused = 1;
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

        else if (effect==Move::Trap)
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
        if (effect==Move::Protection)
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
            atk->state.protectUsedLast = false;

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
            taker->stages.acc -= intensity;
            taker->stages.acc = (taker->stages.acc<-6)?(-6):(taker->stages.acc);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Accuracy fell!");
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
        else if (effect==Move::SetBall)
        {
            if (atk->state.ballIsUp) {
                atk->state.ballSet = true;
                atk->state.ballHandled = true;
                ret.push_back(attacker.name+" Set the ball!");
            }
            else
                ret.push_back(attacker.name+" tried to Set, but there was no ball in the air!");
        }
        else if (effect==Move::BumpBall)
        {
            atk->state.switchAfterMove = true;
            atk->state.ballIsUp = true;
            atk->state.ballHandled = true;
            ret.push_back(attacker.name+" Bumped a ball from out of nowhere!");
        }
        else if (effect==Move::SpikeBall)
        {
            if (atk->state.ballIsUp) {
                if (oppEffect==Move::BlockBall)
                    ret.push_back(attacker.name+" tried to Spike the ball but "+defender.name+" Blocked it!");
                else {
                    atk->state.ballIsUp = false;
                    def->state.ballIsUp = true;
                    def->state.ballHandled = false;
                    ret.push_back(attacker.name+" Spiked the ball!");
                }
            }
            else
                ret.push_back(attacker.name+" tried to Spike but there was no ball above them!");
        }
        else if (effect==Move::BlockBall)
            ret.push_back(attacker.name+" is preparing to Block a ball");
        else if (effect==Move::SwipeBall)
        {
            if (atk->state.ballIsUp) {
                if (oppEffect==Move::BlockBall) {
                    def->state.ballIsUp = true;
                    atk->state.ballIsUp = false;
                    def->state.ballHandled = false;
                    ret.push_back(defender.name+" tried to Block the ball but "+attacker.name+" Swiped it!");
                }
                else
                    ret.push_back(attacker.name+" tried to Swipe the ball but "+defender.name+" wasn't Blocking so it failed!");
            }
            else
                ret.push_back(attacker.name+" tried to Swipe but there was no ball above them!");
        }
        else if (effect==Move::WakeBoth)
        {
            for (unsigned int i = 0; i<atk->getPeoplemon()->size(); ++i) {
                if (atk->getPeoplemon()->at(i).curAils[0] == Peoplemon::Sleep)
                    atk->getPeoplemon()->at(i).curAils[0] = Peoplemon::None;
            }
            for (unsigned int i = 0; i<def->getPeoplemon()->size(); ++i) {
                if (def->getPeoplemon()->at(i).curAils[0] == Peoplemon::Sleep)
                    def->getPeoplemon()->at(i).curAils[0] = Peoplemon::None;
            }
            ret.push_back("It is physically impossible for anyone in a 15 mile radius to still be asleep after that");
        }
        else if (effect==Move::HealPercent)
        {
            double hpGained = damage * double(intensity) / 100;
            if (taker->curHp+hpGained > taker->stats.hp)
                hpGained = taker->stats.hp - taker->curHp;
            taker->curHp += hpGained;
            ret.push_back(taker->name+" gained "+intToString(hpGained)+" HP!");
        }
        else if (effect==Move::BatonPass || effect==Move::DamageThenSwitch) {
            ret.push_back("Baton Pass allows "+attacker.name+" to switch out!");
            atk->state.switchAfterMove = true;
        }
        else if (effect==Move::CritEvdUp) {
            taker->stages.evade += intensity;
            taker->stages.evade = (taker->stages.evade>6)?(6):(taker->stages.evade);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Evasiveness rose!");
            taker->stages.crit += intensity;
            taker->stages.crit = (taker->stages.crit>6)?(6):(taker->stages.crit);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Criticalness increased!");
        }
        else if (effect==Move::StayAlive) {
            if (atk->state.enduredLastTurn) {
                atk->state.endureThisTurn = true;
                ret.push_back(attacker.name+" tried to save itself again but it failed!");
            }
            else {
                atk->state.endureThisTurn = true;
                ret.push_back(attacker.name+" protected itself!");
            }
        }
        else if (effect==Move::MaxAtkMinAcc) {
            taker->stages.atk += 6;
            taker->stages.atk = (taker->stages.atk>6)?(6):(taker->stages.atk);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Attack rose hugely!");
            taker->stages.acc -= 6;
            taker->stages.acc = (taker->stages.acc<-6)?(-6):(taker->stages.acc);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Accuracy decreased so freakin much!");
        }
        else if (effect==Move::FrustConfuse) {
            if (canGetAils)
            {
                taker->addPassiveAilment(Peoplemon::Confused);
                taker->turnsConfused = 1;
                ret.push_back(taker->name+" was Confused!");

                if (taker->curAils[0]==Peoplemon::None) {
                    if (taker->curAbility!=Peoplemon::Classy) {
                        taker->curAils[0] = Peoplemon::Frustrated;
                        ret.push_back(taker->name+" was also Frustrated!");
                        if (reciprocateAil)
                        {
                            giver->addPassiveAilment(Peoplemon::Confused);
                            giver->turnsConfused = 1;
                            ret.push_back(taker->name+" Shared its Confusion with "+giver->name+"!");

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
                        ret.push_back(giver->name+" tried to Frustrate "+giver->name+" but "+giver->name+"is Classy and cannot be Frustrated!");
                }

            }
            else
                ret.push_back(giver->name+" tried to Frustrate and Confuse "+taker->name+" but it failed!");
        }
        else if (effect==Move::Spikes) {
            if (def->state.spikesApplied<3) {
                def->state.spikesApplied++;
                ret.push_back(attacker.name+" put Spikes around "+defender.name+"! They better not move too much");
            }
            else
                ret.push_back(attacker.name+" tried to add more Spikes but they won't fit anywhere. What a mess");
        }
        else if (effect==Move::EnemyPPDown) {
            if (defender.knowsMove(def->state.lastMoveUsed)) {
                int i = 0;
                for (; i<4; ++i) {
                    if (defender.moves[i].id==def->state.lastMoveUsed)
                        break;
                }
                def->getPeoplemon()->at(def->getCurrentPeoplemon()).moves[i].curPp -= 5;
                if (def->getPeoplemon()->at(def->getCurrentPeoplemon()).moves[i].curPp<0)
                    def->getPeoplemon()->at(def->getCurrentPeoplemon()).moves[i].curPp = 0;
                ret.push_back(defender.name+" had its PP for "+game->moveList[def->state.lastMoveUsed].name+" lowered!");
            }
            else
                ret.push_back(attacker.name+" tried to lower "+defender.name+"'s PP but it failed!");
        }
        else if (effect==Move::HealNext) {
            atk->state.healNextPeoplemon = true;
            ret.push_back(attacker.name+" threw some weird healing powder into the air. It went so high it looks like it will take exactly one turn to fall back down");
        }
        else if (effect==Move::Roar) {
            atk->state.roarUsed = true;
            ret.push_back(attacker.name+"'s Roar scared "+defender.name+" so much that they went back in their Peopleball!");
        }
        else if (effect==Move::SpdAtkUp) {
            taker->stages.atk += 1;
            taker->stages.atk = (taker->stages.atk>6)?(6):(taker->stages.atk);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Attack rose!");
            taker->stages.spd += 1;
            taker->stages.spd = (taker->stages.spd>6)?(6):(taker->stages.spd);
            taker->recalcStats(game);
            ret.push_back(taker->name+"'s Speed rose!");
        }
        else if (effect==Move::StealStats) {
            atk->getPeoplemon()->at(atk->getCurrentPeoplemon()).stages = defender.stages;
            def->getPeoplemon()->at(def->getCurrentPeoplemon()).stages.zero();
            atk->recalcStats(game, false);
            def->recalcStats(game, false);
            ret.push_back(attacker.name+" stole all of "+defender.name+"'s stat changes!");
        }
        else if (effect==Move::RoarCancelBallSpikes) {
            atk->state.roarUsed = true;
            atk->state.spikesApplied = 0;
            atk->state.ballIsUp = atk->state.ballSet = false;
            ret.push_back(attacker.name+" was so loud that everything in their immediate area got blown away! No more pesky Spikes or Volleyballs");
        }
        else if (effect==Move::Encore) {
            def->state.encoreHit = true;
            ret.push_back(attacker.name+" is trying to make "+defender.name+" do an Encore!");
        }
        else if (effect==Move::DieIn3Turns) {
            atk->state.deathTurnCounter = 0;
            def->state.deathTurnCounter = 0;
            ret.push_back(attacker.name+" and "+defender.name+" have both been Marked for Death! They better get out of here");
        }
        else if (effect==Move::DeathSwap) {
            atk->state.koRevive = true;
            atk->state.koReviveHp = atk->getPeoplemon()->at(atk->getCurrentPeoplemon()).curHp;
            atk->getPeoplemon()->at(atk->getCurrentPeoplemon()).curHp = 0;
            ret.push_back(attacker.name+" has sacrificed itself!");
        }
        else if (effect==Move::SleepHeal) {
            for (int w = 0; w<4; ++w)
                taker->curAils[w] = Peoplemon::Ailment::None;
            taker->curAils[0] = Peoplemon::Ailment::Sleep;
            taker->curHp = taker->stats.hp;
            taker->turnsUntilWake = 3;
            ret.push_back(taker->name+" put itself to Sleep and got healed!");
        }
    }

    lastMoveHit = true;
    return ret;
}

void BattleState::transitionScreen()
{
	int trans = Random(0,1);
	trans = 1;
	Texture bgndTxtr;
	Sprite bgnd;

	bgndTxtr.create(game->mainWindow.getSize().x,game->mainWindow.getSize().y);
	bgndTxtr.update(game->mainWindow);
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
			game->mainWindow.clear();
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
			game->mainWindow.clear();
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
    int j = (i+1)%2;
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
	Animation* playing[2] = {&ballThrow,nullptr};
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
			if (playing[i]!=nullptr)
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
				playing[1] = nullptr;
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
                if (playing[1]!=nullptr)
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
			if (playing[i]!=nullptr)
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
