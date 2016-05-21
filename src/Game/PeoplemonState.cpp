#include "Game/PeoplemonState.hpp"
#include "Game/PeoplemonInfoState.hpp"
#include "Menu/PeoplemonSelector.hpp"
#include "Game/BagState.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include <iostream>
using namespace sf;
using namespace std;

PeoplemonState::PeoplemonState(Game* g, bool getChoice, vector<PeoplemonRef>* ppl, string p, int item) : Gamestate(g)
{
    shouldReturnChoice = getChoice;
    peoplemon = ppl;
    choicePrompt = p;
    chosenIndex = -1;
    itemId = item;
}

bool PeoplemonState::execute()
{
    sleep(milliseconds(300));

    MenuImage background("peoplemonBgnd.png");
    PeoplemonSelector menu(peoplemon, game,!shouldReturnChoice);
    bool gettingSecond = false;

    while (!finishFrame())
    {
        menu.update();

        if (menu.selectionMade())
        {
            sleep(milliseconds(450));

            if (gettingSecond)
            {
                if (unsigned(menu.secondarySelection())==peoplemon->size())
                {
                    gettingSecond = false;
                    menu.setSecondSelection(false);
                }
                else
                {
                    PeoplemonRef t = peoplemon->at(menu.primarySelection());
                    peoplemon->at(menu.primarySelection()) = peoplemon->at(menu.secondarySelection());
                    peoplemon->at(menu.secondarySelection()) = t;
                    menu.sync(peoplemon, game);
                    menu.setSecondSelection(false);
                    gettingSecond = false;
                }
            }
            else
            {
                if (unsigned(menu.primarySelection())==peoplemon->size() && !shouldReturnChoice)
                    return false;
                else
                {
                    if (shouldReturnChoice || itemId!=-1 || choicePrompt.size()>0)
                    {
                    	if ((shouldReturnChoice || itemId!=-1) && peoplemon->at(menu.primarySelection()).curHp==0)
						{
							game->hud.displayMessage("Don't take advantage of unconscious Peoplemon!");
							while (!game->hud.messageFinished())
							{
								if (finishFrame())
									return true;

								game->hud.update();

								game->hud.draw(&game->mainWindow);
								game->mainWindow.display();
								sleep(milliseconds(30));
							}
							menu.sync(peoplemon,game);
							goto noAction;
						}

                    	sleep(milliseconds(175));
                        game->hud.getChoice(choicePrompt, vector<string>({"Yes","No"}),false);
                        menu.sync(peoplemon,game);
                        while (true)
                        {
                            game->hud.update();

                            if (game->hud.getChoice()=="Yes")
                            {
                                if (itemId!=-1)
								{
									string out;
									int i = menu.primarySelection();

									if (itemId>=1 && itemId<=4) //potion
									{
										if (peoplemon->at(i).curHp==peoplemon->at(i).stats.hp)
											out = peoplemon->at(i).name+"'s HP is already full!";
										else
										{
											int lk[] = {20,50,200,9001};
											int cap = lk[itemId-1];
                                            int restore = peoplemon->at(i).stats.hp-peoplemon->at(i).curHp;
                                            if (restore>cap)
												restore = cap;
											out = peoplemon->at(i).name+"'s HP was restored by "+intToString(restore)+" points!";
											peoplemon->at(i).curHp += restore;
										}
									}
									else if (itemId>=18 && itemId<=21) //pp pack. 19 and 21 aren't actually included
									{
										bool full = true;
										for (int j = 0; j<4; ++j)
										{
											if (peoplemon->at(i).moves[j].id!=0 && peoplemon->at(i).moves[j].curPp<game->moveList[peoplemon->at(i).moves[j].id].pp)
												full = false;
										}
										if (full)
											out = peoplemon->at(i).name+"'s PP is all full!";
										else
										{
											int pp = (itemId==18)?(10):(10000);
											for (int j = 0; j<4; ++j)
											{
												peoplemon->at(i).moves[j].curPp += pp;
												if (peoplemon->at(i).moves[j].curPp>game->moveList[peoplemon->at(i).moves[j].id].pp)
													peoplemon->at(i).moves[j].curPp = game->moveList[peoplemon->at(i).moves[j].id].pp;
											}
											out = peoplemon->at(i).name+"'s PP was replenished!";
										}
									}
									else if (itemId==22) //keg of protein
									{
										int evCap = 510-peoplemon->at(i).evs.sum();
										if (evCap<=0 || 252-peoplemon->at(i).evs.atk<=0)
											out = peoplemon->at(i).name+"'s attack won't go any higher!";
										else
                                        {
                                        	int plus = 10;
                                        	if (evCap<10)
												plus -= evCap;
                                        	out = peoplemon->at(i).name+"'s Attack rose from a Keg of Protein!";
											peoplemon->at(i).evs.atk += plus;
											peoplemon->at(i).evs.cap(252);
                                        }
									}
									else if (itemId==23) //super power juice
									{
										int evCap = 510-peoplemon->at(i).evs.sum();
										if (evCap<=0 || 252-peoplemon->at(i).evs.spAtk<=0)
											out = peoplemon->at(i).name+"'s Special Attack won't go any higher!";
										else
                                        {
                                        	int plus = 10;
                                        	if (evCap<10)
												plus -= evCap;
                                        	out = peoplemon->at(i).name+"'s Special Attack rose from a Super Power Juice!";
											peoplemon->at(i).evs.spAtk += plus;
											peoplemon->at(i).evs.cap(252);
                                        }
									}
									else if (itemId==24) //tub of ice cream
									{
										int evCap = 510-peoplemon->at(i).evs.sum();
										if (evCap<=0 || 252-peoplemon->at(i).evs.spDef<=0)
											out = peoplemon->at(i).name+"'s Special Defense won't go any higher!";
										else
                                        {
                                        	int plus = 10;
                                        	if (evCap<10)
												plus -= evCap;
                                        	out = peoplemon->at(i).name+"'s Special Defense rose from a Tub of Ice Cream!";
											peoplemon->at(i).evs.spDef += plus;
											peoplemon->at(i).evs.cap(252);
                                        }
									}
									else if (itemId==25) //jar of encouragement
									{
										int evCap = 510-peoplemon->at(i).evs.sum();
										if (evCap<=0 || 252-peoplemon->at(i).evs.def<=0)
											out = peoplemon->at(i).name+"'s Defense won't go any higher!";
										else
                                        {
                                        	int plus = 10;
                                        	if (evCap<10)
												plus -= evCap;
                                        	out = peoplemon->at(i).name+"'s Defense rose from a Jar of Encouragement!";
											peoplemon->at(i).evs.def += plus;
											peoplemon->at(i).evs.cap(252);
                                        }
									}
									else if (itemId==26) //super speed juice
									{
										int evCap = 510-peoplemon->at(i).evs.sum();
										if (evCap<=0 || 252-peoplemon->at(i).evs.spd<=0)
											out = peoplemon->at(i).name+"'s Speed won't go any higher!";
										else
                                        {
                                        	int plus = 10;
                                        	if (evCap<10)
												plus -= evCap;
                                        	out = peoplemon->at(i).name+"'s Speed rose from a Keg of Protein!";
											peoplemon->at(i).evs.spd += plus;
											peoplemon->at(i).evs.cap(252);
                                        }
									}
									else if (itemId==27) //compliments
									{
										int evCap = 510-peoplemon->at(i).evs.sum();
										if (evCap<=0 || 252-peoplemon->at(i).evs.hp<=0)
											out = peoplemon->at(i).name+"'s Health won't go any higher!";
										else
                                        {
                                        	int plus = 10;
                                        	if (evCap<10)
												plus -= evCap;
                                        	out = peoplemon->at(i).name+"'s Health rose from a Keg of Protein!";
											peoplemon->at(i).evs.hp += plus;
											peoplemon->at(i).evs.cap(252);
                                        }
									}
									else if (itemId==39)
									{
                                        if (peoplemon->at(i).curAils[0]!=Peoplemon::Annoyed)
											out = peoplemon->at(i).name+" isn't Annoyed yet, but you can keep trying";
										else
										{
											peoplemon->at(i).curAils[0] = Peoplemon::None;
											out = peoplemon->at(i).name+" is no longer Annoyed!";
										}
									}
									else if (itemId==40)
									{
                                        if (peoplemon->at(i).curAils[0]!=Peoplemon::Frustrated)
											out = peoplemon->at(i).name+" isn't Frustrated yet, but you can keep trying";
										else
										{
											peoplemon->at(i).curAils[0] = Peoplemon::None;
											out = peoplemon->at(i).name+" is no longer Frustrated!";
										}
									}
									else if (itemId==41)
									{
                                        if (peoplemon->at(i).curAils[0]!=Peoplemon::Sleep)
											out = peoplemon->at(i).name+" isn't Sleeping!";
										else
										{
											peoplemon->at(i).curAils[0] = Peoplemon::None;
											out = peoplemon->at(i).name+" is no longer Sleeping!";
										}
									}
									else if (itemId==42)
									{
                                        if (peoplemon->at(i).curAils[0]!=Peoplemon::Sticky)
											out = peoplemon->at(i).name+" isn't Sticky yet, but you can keep trying";
										else
										{
											peoplemon->at(i).curAils[0] = Peoplemon::None;
											out = peoplemon->at(i).name+" is no longer Sticky!";
										}
									}
									else if (itemId==43)
									{
                                        if (peoplemon->at(i).curAils[0]!=Peoplemon::Frozen)
											out = peoplemon->at(i).name+" isn't Frozen!";
										else
										{
											peoplemon->at(i).curAils[0] = Peoplemon::None;
											out = peoplemon->at(i).name+" is no longer Frozen!";
										}
									}

									game->hud.displayMessage(out);

									while (!game->hud.messageFinished())
									{
										game->hud.update();
										menu.sync(peoplemon,game);

										if (finishFrame())
											return true;

										background.draw(&game->mainWindow);
										menu.draw(&game->mainWindow);
										game->hud.draw(&game->mainWindow);
										game->mainWindow.display();
										sleep(milliseconds(30));
									}
								}
								chosenIndex = menu.primarySelection();
                                return false;
                            }
                            if (game->hud.getChoice()=="No")
                                break;

                            if (finishFrame())
                                return true;

                            background.draw(&game->mainWindow);
                            menu.draw(&game->mainWindow);
                            game->hud.draw(&game->mainWindow);
                            game->mainWindow.display();

                            sleep(milliseconds(50));
                        }
                    }
                    else
                    {
                        ChoiceBox prompt;
                        prompt.addChoice("Summary");
                        prompt.addChoice("Switch");
                        prompt.addChoice("Item");
                        prompt.addChoice("Cancel");
                        prompt.setBackgroundColor(Color(200,200,200));
                        prompt.setBorder(Color::Black, 3);
                        prompt.setTextProps(Color::Black, 32);
                        prompt.setPosition(Vector2f(560, 510));
                        prompt.setAllowedCols(2);

                        while (true)
                        {
                            prompt.update();

                            if (prompt.getChoice()=="Summary")
                            {
                                if (game->runState(new PeoplemonInfoState(game,peoplemon->at(menu.primarySelection()))))
                                    return true;
                                break;
                            }
                            if (prompt.getChoice()=="Switch")
                            {
                                gettingSecond = true;
                                menu.setSecondSelection(true);
                                break;
                            }
                            if (prompt.getChoice()=="Item")
                            {
                                sleep(milliseconds(450));
                                ChoiceBox gt;
                                gt.addChoice("Give");
                                gt.addChoice("Take");
                                gt.addChoice("Cancel");
                                gt.setBackgroundColor(Color(200,200,200));
                                gt.setBorder(Color::Black, 3);
                                gt.setTextProps(Color::Black, 32);
                                gt.setPosition(Vector2f(600, 490));

                                while (true)
                                {
                                    gt.update();

                                    if (gt.getChoice()=="Give")
                                    {
                                        BagState* b = new BagState(game,false,true);
                                        bool r = b->run();
                                        if (r)
                                            return true;
                                        if (b->chosenItem()!=0)
                                        {
                                            if (peoplemon->at(menu.primarySelection()).holdItem!=0)
                                                game->player.giveItem(peoplemon->at(menu.primarySelection()).holdItem);
                                            peoplemon->at(menu.primarySelection()).holdItem = b->chosenItem();
                                            menu.sync(peoplemon,game);
                                        }
                                        break;
                                    }
                                    if (gt.getChoice()=="Take")
                                    {
                                        int i = (*peoplemon)[menu.primarySelection()].holdItem;
                                        (*peoplemon)[menu.primarySelection()].holdItem = 0;
                                        menu.sync(peoplemon,game);
                                        string out = "Took a " + game->itemList[i].name + " from " + game->peoplemonList[(*peoplemon)[menu.primarySelection()].id].name;
                                        if (i==0)
                                            out = (*peoplemon)[menu.primarySelection()].name + " wasn't holding anything!";
                                        else
                                            game->player.getItems()->push_back(i);

                                        game->hud.displayMessage(out);

                                        while (!game->hud.messageFinished())
                                        {
                                            game->hud.update();

                                            if (finishFrame())
                                                return true;

                                            background.draw(&game->mainWindow);
                                            menu.draw(&game->mainWindow);
                                            game->hud.draw(&game->mainWindow);
                                            game->mainWindow.display();
                                            sleep(milliseconds(30));
                                        }
                                        break;
                                    }
                                    if ((gt.getChoice()=="Cancel" || user.isInputActive(PlayerInput::Run)) && !shouldReturnChoice)
									{
										sleep(milliseconds(250));
										break;
									}
                                    else if (gt.getChoice()=="Cancel")
                                    {
                                        gt.reset();
                                        sleep(milliseconds(225));
                                    }

                                    if (finishFrame())
                                        return true;

                                    background.draw(&game->mainWindow);
                                    menu.draw(&game->mainWindow);
                                    gt.draw(&game->mainWindow);
                                    game->mainWindow.display();
                                    sleep(milliseconds(50));
                                }
                                break;
                            }
                            if (prompt.getChoice()=="Cancel" || user.isInputActive(PlayerInput::Run))
							{
								sleep(milliseconds(250));
								break;
							}

                            if (finishFrame())
                                return true;

                            background.draw(&game->mainWindow);
                            menu.draw(&game->mainWindow);
                            prompt.draw(&game->mainWindow);
                            game->mainWindow.display();

                            sleep(milliseconds(50));
                        }
                    }
                }
                noAction:;
            }
        }

        if (user.isInputActive(PlayerInput::Run))
			return false;

        background.draw(&game->mainWindow);
        menu.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(120));
    }

    return true;
}

int PeoplemonState::getChosenIndex()
{
    return chosenIndex;
}
