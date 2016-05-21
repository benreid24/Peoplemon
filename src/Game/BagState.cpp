#include "Game/BagState.hpp"
#include "Game/PeoplemonState.hpp"
#include "Menu/BagItemList.hpp"
#include "Menu/BagSectionSelector.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "SFML.hpp"
#include "Globals.hpp"
#include <iostream>
using namespace std;
using namespace sf;

BagState::BagState(Game* g, bool inB, bool getC) : Gamestate(g)
{
    getChoice = getC;
    inBattle = inB;
    choice = 0;
}

string BagState::wordWrap(string str)
{
    Text temp;
    temp.setFont(Properties::PrimaryMenuFont);
    temp.setCharacterSize(22);
    temp.setString(str);
    for (unsigned int i = 0; i<str.size(); ++i)
    {
        if (temp.findCharacterPos(i).x>=600)
        {
            for (unsigned int j = i; j>=0; j--)
            {
                if (str[j]==' ')
                {
                    str.insert(j,"\n");
                    temp.setString(str);
                    break;
                }
            }
        }
    }
    return str;
}

int BagState::chosenItem()
{
    return choice;
}

string BagState::getMessage()
{
    return retMes;
}

bool BagState::displayMessage(string m)
{
	game->hud.displayMessage(m);
	while (!game->hud.messageFinished())
	{
		if (finishFrame())
			return true;
		game->hud.update();

		game->hud.draw(&game->mainWindow);
		game->mainWindow.display();
		sleep(milliseconds(30));
	}
	return false;
}

bool BagState::execute()
{
    sleep(milliseconds(300));
    MenuImage background("bagBgnd.png");
    BagSectionSelector sectionSelector;
    BagItemList itemList(game->player.getItems(), 1, 100, game);
    MenuText itemDescription;
    itemDescription.setProps(Color::Black, 22);
    itemDescription.setPosition(Vector2f(95,480));
    string lastSec = "Items";

    while (!finishFrame())
    {
        sectionSelector.update();
        itemList.update();

        if (sectionSelector.getCurrentSection()=="Items" && lastSec!="Items")
        {
            lastSec = "Items";
            itemList.setItems(game->player.getItems(), 1, 100, game);
        }
        if (sectionSelector.getCurrentSection()=="Key Items" && lastSec!="Key Items")
        {
            lastSec = "Key Items";
            itemList.setItems(game->player.getItems(), 101, 200, game);
        }
        if (sectionSelector.getCurrentSection()=="TM's" && lastSec!="TM's")
        {
            lastSec = "TM's";
            itemList.setItems(game->player.getItems(), 201, 1000, game);
        }

        if (itemList.getCurrentItem()!=0 && itemList.getCurrentItem()!=-1)
            itemDescription.setText(wordWrap(game->itemList[itemList.getCurrentItem()].description));
        else
            itemDescription.setText("");

        if (user.isInputActive(PlayerInput::Interact))
        {
            sleep(milliseconds(300));
            if (itemList.getCurrentItem()==-1)
                return false;

            ChoiceBox cBox;
            cBox.setPosition(Vector2f(600,325));
            //if (!getChoice)
                cBox.addChoice("Use");
            if (sectionSelector.getCurrentSection()!="Key Items")
			{
				cBox.addChoice("Give");
				cBox.addChoice("Drop");
			}
            cBox.addChoice("Cancel");
            if (!getChoice)
                cBox.setAllowedCols(2);
            cBox.setTextProps(Color::Black, 22);
            cBox.setBackgroundColor(Color::White);
            cBox.setBorder(Color::Black,3);

            while (true)
            {
                if (finishFrame())
                    return true;

                cBox.update();

                if (cBox.getChoice()=="Use")
                {
                    int id = itemList.getCurrentItem();

                    if (inBattle)
                    {
                        if (id<22 || (id>38 && id<50)) //validate that it can be used in battle
                        {
                            if (id<=4 || (id>=38 && id<=43))
							{
								int ppl = getPeoplemon(id);
								if (ppl==-100)
									return true;
								if (ppl!=-1)
								{
									game->player.takeItem(itemList.getCurrentItem());
									choice = id;
									return false;
								}
								else
									break;
							}
							else
                            {
                            	cout << "Use non-Peoplemon item in battle here\n";
								game->player.takeItem(itemList.getCurrentItem());
								choice = id;
								return false;
                            }
                        }
                        else
						{
							if (displayMessage("You can't use that now!"))
								return true;
						}
                    }
                    else
                    {
						if (id<=4 || (id>=18 && id<=32 && id!=19 && id!=21) || (id>=38 && id<=43) || id>=201)
						{
							int ppl = getPeoplemon(id);
							if (ppl!=-1)
								game->player.takeItem(itemList.getCurrentItem());
							if (ppl==-100)
								return true;
						}
						else
                        {
                        	//TODO - use item
                        	if (id==19 || id==21) //pp 6 pack
							{
								int pp = (id==19)?(10):(10000);
								for (unsigned int i = 0; i<game->player.getCurrentPeoplemon()->size(); ++i)
								{
									for (int j = 0; j<4; ++j)
									{
										MoveRef* m = &game->player.getCurrentPeoplemon()->at(i).moves[j];
										if (m->id!=0)
										{
											m->curPp += pp;
											if (m->curPp>game->moveList[m->id].pp)
												m->curPp = game->moveList[m->id].pp;
										}
									}
								}
								if (displayMessage("All of your Peoplemon got PP!"))
									return true;
							}
							cout << "Item use here\n";
							game->player.takeItem(itemList.getCurrentItem());
                        }
                    }
                    break;
                }
                if (cBox.getChoice()=="Give")
                {
                    if (getChoice)
                    {
                        choice = itemList.getCurrentItem();
                        game->player.takeItem(itemList.getCurrentItem());
                        return false;
                    }

                    PeoplemonState* p = new PeoplemonState(game,true,game->player.getCurrentPeoplemon(),"Give the "+game->itemList[itemList.getCurrentItem()].name+"?");
                    bool ret = p->run();
                    if (ret)
                        return true;
                    if (p->getChosenIndex()!=-1)
                    {
                        PeoplemonRef* ppl = &game->player.getCurrentPeoplemon()->at(p->getChosenIndex());
                        if (ppl->holdItem!=0)
                        {
                            int t = ppl->holdItem;
                            ppl->holdItem = itemList.getCurrentItem();
                            game->player.takeItem(itemList.getCurrentItem());
                            game->player.giveItem(t);
                        }
                        else
                        {
                            ppl->holdItem = itemList.getCurrentItem();
                            game->player.takeItem(itemList.getCurrentItem());
                        }
                    }
                    delete p;
                    break;
                }
                if (cBox.getChoice()=="Drop")
                {
                    game->player.takeItem(itemList.getCurrentItem());
                    break;
                }
                if (cBox.getChoice()=="Cancel" || user.isInputActive(PlayerInput::Run))
                    break;

                background.draw(&game->mainWindow);
                itemList.draw(&game->mainWindow);
                sectionSelector.draw(&game->mainWindow);
                itemDescription.draw(&game->mainWindow);
                cBox.draw(&game->mainWindow);
                game->mainWindow.display();

                sleep(milliseconds(50));
            }
            if (sectionSelector.getCurrentSection()=="Items")
                itemList.setItems(game->player.getItems(), 1, 100, game);
            if (sectionSelector.getCurrentSection()=="Key Items")
                itemList.setItems(game->player.getItems(), 101, 200, game);
            if (sectionSelector.getCurrentSection()=="TM's")
                itemList.setItems(game->player.getItems(), 201, 1000, game);
            sleep(milliseconds(300));
        }

        if (user.isInputActive(PlayerInput::Run))
			return false;

        background.draw(&game->mainWindow);
        itemList.draw(&game->mainWindow);
        sectionSelector.draw(&game->mainWindow);
        itemDescription.draw(&game->mainWindow);
        game->mainWindow.display();

        sleep(milliseconds(100));
    }

    return true;
}

int BagState::getPeoplemon(int id)
{
    PeoplemonState* state = new PeoplemonState(game,false,game->player.getCurrentPeoplemon(),"Use the "+game->itemList[id].name+"?",id);
    bool ret = state->run();

    if (ret)
        return -100;

    int i = state->getChosenIndex();
    delete state;
    return i;
}
