#include "Game/StorageSystem.hpp"
#include "Game/PeoplemonState.hpp"
#include "Game/PeoplemonInfoState.hpp"
#include "Menu/ChoiceBox.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
using namespace std;
using namespace sf;

StorageSystem::StorageSystem(Game* g, vector<StoredPeoplemon>* ppl) : Gamestate(g)
{
    peoplemon = ppl;
    cursorTxtr = imagePool.loadResource(Properties::MenuImagePath+"storageCursor.png");
    backgroundTxtr = imagePool.loadResource(Properties::MenuImagePath+"storageBGND.png");
    lArrowTxtr = imagePool.loadResource(Properties::MenuImagePath+"storageArrowLeft.png");
    rArrowTxtr = imagePool.loadResource(Properties::MenuImagePath+"storageArrowRight.png");
    cancelBut.setImage("storageDone.png");
    for (int i = 1; i<=14; ++i)
    {
        boxTitleTxtr[i-1] = imagePool.loadResource(Properties::MenuImagePath+"storageBoxTitle"+intToString(i)+".png");
        boxTxtr[i-1] = imagePool.loadResource(Properties::MenuImagePath+"storageBox"+intToString(i)+".png");
    }
    for (auto i = g->peoplemonList.begin(); i!=g->peoplemonList.end(); ++i)
    {
        peoplemonTxtrs[i->first] = imagePool.loadResource(Properties::PeoplemonImagePath+i->second.name+".png");
    }

    cursor.setTexture(*cursorTxtr);
    background.setTexture(*backgroundTxtr);
    lArrow.setTexture(*lArrowTxtr);
    rArrow.setTexture(*rArrowTxtr);
    for (int i = 0; i<14; ++i)
    {
        boxTitle[i].setTexture(*boxTitleTxtr[i]);
        box[i].setTexture(*boxTxtr[i]);
    }
    for (auto i = peoplemonTxtrs.begin(); i!=peoplemonTxtrs.end(); ++i)
    {
        icons.insert(make_pair(i->first,Sprite()));
        icons[i->first].setTexture(*i->second);
        icons[i->first].setScale(40.0/float(i->second->getSize().x),40.0/float(i->second->getSize().y)); //TODO - change to actual size
    }

    cursor.setPosition(295,160);
    lArrow.setPosition(300,70);
    rArrow.setPosition(701,70);
    cancelBut.setPosition(Vector2f(295+8*52,160+7*52));
    for (int i = 0; i<14; ++i)
    {
        boxTitle[i].setPosition(365,70);
        box[i].setPosition(283,148);
    }

    name.setPosition(Vector2f(70,445));
    name.setProps(Color::White,22);
    level.setPosition(Vector2f(70,480));
    level.setProps(Color::White,20);
    item.setPosition(Vector2f(70,505));
    item.setProps(Color::White,20);
}

bool StorageSystem::spaceFree(int b, int x, int y)
{
    if (x==8 && y==7) //bottom right
        return false;

    Vector2i comp(x,y);
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        if (peoplemon->at(i).position==comp && b==peoplemon->at(i).boxId)
            return false;
    }
    return true;
}

PeoplemonRef StorageSystem::getPeoplemon(int b, int x, int y)
{
    Vector2i comp(x,y);
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        if (peoplemon->at(i).boxId==b && peoplemon->at(i).position==comp)
            return peoplemon->at(i).data;
    }
    PeoplemonRef ret;
    ret.id = 0;
    return ret;
}

int StorageSystem::getIndex(int b, int x, int y)
{
    Vector2i comp(x,y);
    for (unsigned int i = 0; i<peoplemon->size(); ++i)
    {
        if (peoplemon->at(i).boxId==b && peoplemon->at(i).position==comp)
            return i;
    }
    return -1;
}

void StorageSystem::updateMenu(int i)
{
    name.setText(peoplemon->at(i).data.name+"\n\\"+game->peoplemonList[peoplemon->at(i).data.id].name);
    level.setText("Level: "+peoplemon->at(i).data.level);
    if (peoplemon->at(i).data.holdItem!=0)
        item.setText("Item: "+game->itemList[peoplemon->at(i).data.holdItem].name);
    else
        item.setText("Item: None");
}

bool StorageSystem::execute()
{
    const int MainMenu = 0, Browsing = 1, Moving = 2;
    int state = MainMenu;

    ChoiceBox mainMenu;
    int curBox = 0;
    int selectedId = 0, selectedIndex = -1; //draw peoplemon graphic in place of cursor when moving

    mainMenu.addChoice("Boxes");
    mainMenu.addChoice("Party");
    mainMenu.addChoice("Exit");
    mainMenu.setPosition(Vector2f(33,25));
    mainMenu.setBackgroundColor(Color::White);
    mainMenu.setBorder(Color::Black,4);
    mainMenu.setTextProps(Color::Black,30);

    while (!finishFrame())
    {
        if (state==MainMenu)
        {
            mainMenu.update();

            if (mainMenu.getChoice()=="Boxes")
            {
                mainMenu.reset();
                state = Browsing;
                sleep(milliseconds(225));
            }
            if (mainMenu.getChoice()=="Party")
            {
                mainMenu.reset();
                sleep(milliseconds(225));
                PeoplemonState* ps = new PeoplemonState(game,true,game->player.getCurrentPeoplemon(),"Deposit this Peoplemon?");
                sleep(milliseconds(225));
                if (ps->run())
                {
                    delete ps;
                    return true;
                }
                if (ps->getChosenIndex()!=-1)
                {
                    for (int b = 0; b<14; b++)
                    {
                        for (int y = 0; y<8; y++)
                        {
                            for (int x = 0; x<9; x++)
                            {
                                if (spaceFree(b,x,y))
                                {
                                    peoplemon->push_back(StoredPeoplemon(b,Vector2i(x,y),game->player.getCurrentPeoplemon()->at(ps->getChosenIndex())));
                                    game->player.getCurrentPeoplemon()->erase(game->player.getCurrentPeoplemon()->begin()+ps->getChosenIndex());
                                    goto done;
                                }
                            }
                        }
                    }
                    cout << "The system is full! You suck!\n";
                    done:
                    delete ps;
                    mainMenu.reset();
                }
            }
            if (mainMenu.getChoice()=="Exit" || user.isInputActive(PlayerInput::Run))
                return false;
        }
        else
        {
            if (user.isInputActive(PlayerInput::Up) && cursorPos.y>0)
                cursorPos.y--;
            else if (user.isInputActive(PlayerInput::Down))
            {
                cursorPos.x++;
                if (cursorPos.x>=9)
                {
                    cursorPos.x = 8;
                    if (curBox<13)
                    {
                        curBox++;
                        cursorPos.x = 0;
                    }
                }
            }
            else if (user.isInputActive(PlayerInput::Interact))
            {
                cursorPos.x--;
                if (cursorPos.x<0)
                {
                    cursorPos.x = 0;
                    if (curBox>0)
                    {
                        curBox--;
                        cursorPos.x = 8;
                    }
                }
            }
            else if (user.isInputActive(PlayerInput::Down) && cursorPos.y<7)
                cursorPos.y++;

			if (user.isInputActive(PlayerInput::Run))
			{
				selectedId = 0;
				selectedIndex = -1;
				state = MainMenu;
				sleep(milliseconds(225));
			}

            if (user.isInputActive(PlayerInput::Interact))
            {
                if (cursorPos.x==8 && cursorPos.y==7)
                {
                    selectedId = 0;
                    selectedIndex = -1;
                    state = MainMenu;
                    sleep(milliseconds(225));
                }
                else if (state==Browsing && !spaceFree(curBox,cursorPos.x,cursorPos.y))
                {
                    sleep(milliseconds(225));
                    ChoiceBox subMenu;
                    subMenu.addChoice("Withdraw");
                    subMenu.addChoice("Summary");
                    subMenu.addChoice("Take Item");
                    subMenu.addChoice("Move");
                    subMenu.addChoice("Back");
                    subMenu.setBackgroundColor(Color::White);
                    subMenu.setBorder(Color::Black,3);
                    subMenu.setTextProps(Color::Black,20);
                    subMenu.setPosition(Vector2f(340,350));

                    while (true)
                    {
                        if (finishFrame())
                            return true;

                        subMenu.update();

                        if (subMenu.getChoice()=="Withdraw")
                        {
                            subMenu.reset();
                            if (game->player.getCurrentPeoplemon()->size()<6)
                            {
                                game->player.getCurrentPeoplemon()->push_back(getPeoplemon(curBox,cursorPos.x,cursorPos.y));
                                peoplemon->erase(peoplemon->begin()+getIndex(curBox,cursorPos.x,cursorPos.y));
                                sleep(milliseconds(225));
                                break;
                            }
                        }
                        if (subMenu.getChoice()=="Summary")
                        {
                            sleep(milliseconds(225));
                            PeoplemonInfoState* pi = new PeoplemonInfoState(game,getPeoplemon(curBox,cursorPos.x,cursorPos.y));
                            sleep(milliseconds(225));
                            if (pi->run())
                            {
                                delete pi;
                                return true;
                            }
                            delete pi;
                            break;
                        }
                        if (subMenu.getChoice()=="Take Item" && peoplemon->at(getIndex(curBox,cursorPos.x,cursorPos.y)).data.holdItem!=0)
                        {
                            game->player.giveItem(peoplemon->at(getIndex(curBox,cursorPos.x,cursorPos.y)).data.holdItem);
                            peoplemon->at(getIndex(curBox,cursorPos.x,cursorPos.y)).data.holdItem = 0;
                            sleep(milliseconds(225));
                            break;
                        }
                        else if (subMenu.getChoice()=="Take Item")
                            subMenu.reset();
                        if (subMenu.getChoice()=="Move")
                        {
                            state = Moving;
                            selectedId = getPeoplemon(curBox,cursorPos.x,cursorPos.y).id;
                            selectedIndex = getIndex(curBox,cursorPos.x,cursorPos.y);
                            break;
                        }
                        if (subMenu.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
                            break;

                        subMenu.draw(&game->mainWindow);
                        game->mainWindow.display();
                        sleep(milliseconds(30));
                    }
                    sleep(milliseconds(225));
                }
                else if (state==Moving && spaceFree(curBox,cursorPos.x,cursorPos.y))
                {
                    peoplemon->at(selectedIndex).boxId = curBox;
                    peoplemon->at(selectedIndex).position = cursorPos;
                    selectedId = 0;
                    selectedIndex = -1;
                    state = Browsing;
                    sleep(milliseconds(225));
                }
            }
        }

        int overIndex = getIndex(curBox,cursorPos.x,cursorPos.y);
        if (selectedIndex!=-1)
            updateMenu(selectedIndex);
        else if (overIndex!=-1)
            updateMenu(overIndex);

        game->mainWindow.draw(background);
        mainMenu.draw(&game->mainWindow);
        if (curBox!=0)
            game->mainWindow.draw(lArrow);
        if (curBox!=13)
            game->mainWindow.draw(rArrow);
        game->mainWindow.draw(boxTitle[curBox]);
        game->mainWindow.draw(box[curBox]);
        name.draw(&game->mainWindow);
        level.draw(&game->mainWindow);
        item.draw(&game->mainWindow);
        if (overIndex!=-1)
        {
            int id = peoplemon->at(overIndex).data.id;
            icons[id].setPosition(68,284);
            icons[id].setScale(150.0/float(peoplemonTxtrs[id]->getSize().x),150.0/float(peoplemonTxtrs[id]->getSize().y));
            game->mainWindow.draw(icons[id]);
        }
        for (unsigned int i = 0; i<peoplemon->size(); ++i)
        {
            if (peoplemon->at(i).boxId==curBox)
            {
                icons[peoplemon->at(i).data.id].setPosition(295+peoplemon->at(i).position.x*52,160+peoplemon->at(i).position.y*52);
                icons[peoplemon->at(i).data.id].setScale(40.0/float(peoplemonTxtrs[peoplemon->at(i).data.id]->getSize().x),40.0/float(peoplemonTxtrs[peoplemon->at(i).data.id]->getSize().y));
                game->mainWindow.draw(icons[peoplemon->at(i).data.id]);
            }
        }
        if (state!=MainMenu)
        {
            cancelBut.draw(&game->mainWindow);
            if (selectedIndex!=-1)
            {
                icons[selectedId].setPosition(295+cursorPos.x*52,160+cursorPos.y*52);
                icons[selectedId].setScale(40.0/float(peoplemonTxtrs[selectedId]->getSize().x),40.0/float(peoplemonTxtrs[selectedId]->getSize().y));
                game->mainWindow.draw(icons[selectedId]);
            }
            cursor.setPosition(295+cursorPos.x*52,160+cursorPos.y*52);
            game->mainWindow.draw(cursor);

        }
        game->mainWindow.display();

        sleep(milliseconds(60));
    }

    return true;
}
