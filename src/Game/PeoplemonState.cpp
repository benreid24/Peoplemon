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
							goto noAction;
						}

                    	sleep(milliseconds(175));
                        game->hud.getChoice(choicePrompt, vector<string>({"Yes","No"}),false);
                        while (true)
                        {
                            game->hud.update();

                            if (game->hud.getChoice()=="Yes")
                            {
                                if (itemId!=-1)
								{
									string out;
									int i = menu.primarySelection();

									if (itemId==22) //keg of protein
									{
										int evCap = 510-peoplemon->at(i).evs.sum();
										if (evCap<=0 || 252-peoplemon->at(i).evs.atk<=0)
											out = peoplemon->at(i).name+"'s attack won't go any higher!";
										else
                                        {
                                        	int plus = 10;
                                        	if (evCap<10)
												plus -= evCap;
                                        	out = peoplemon->at(i).name+"'s attack rose from a Keg of Protein!";
											peoplemon->at(i).evs.atk += plus;
											peoplemon->at(i).evs.cap(252);
                                        }
									}
									//TODO - other PM context items here

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
