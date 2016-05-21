#include "Menu/BagItemList.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace sf;
using namespace std;

BagItemList::BagItemList(vector<int>* items, int first, int last, Game* g)
{
    upArrow.setImage("bagUpArrow.png");
    downArrow.setImage("bagDownArrow.png");
    unSelBox.setImage("itemUnSel.png");
    selBox.setImage("itemSel.png");
    upArrow.setPosition(Vector2f(560,35));
    downArrow.setPosition(Vector2f(560,375));
    itemName.setProps(Color::Black, 22);
    setItems(items, first, last, g);
}

void BagItemList::setItems(vector<int>* items, int first, int last, Game* g)
{
    itemList.clear();
    for (unsigned int i = 0; i<items->size(); ++i)
    {
        if (items->at(i)>=first && items->at(i)<=last)
        {
            string name = g->itemList[items->at(i)].name;
            itemList.push_back(make_pair(items->at(i), name));
        }
    }
    itemList.push_back(make_pair(-1,"Exit"));
    curItem = 0;
}

void BagItemList::update()
{
    if (user.isInputActive(PlayerInput::Up))
    {
        curItem--;
        if (curItem<0)
            curItem = 0;
    }
    else if (user.isInputActive(PlayerInput::Down))
    {
        curItem++;
        if (unsigned(curItem)>=itemList.size())
            curItem = itemList.size()-1;
    }
}

void BagItemList::draw(RenderWindow* window)
{
    int topItemShown = curItem-2;
    if (unsigned(topItemShown+5)>itemList.size())
        topItemShown = signed(itemList.size()-5);
    if (topItemShown<0)
        topItemShown = 0;

    for (int i = topItemShown; i<topItemShown+5 && i<signed(itemList.size()); ++i)
    {
        if (i==curItem)
        {
            selBox.setPosition(Vector2f(435,100+(i-topItemShown)*55));
            selBox.draw(window);
        }
        else
        {
            unSelBox.setPosition(Vector2f(435,100+(i-topItemShown)*55));
            unSelBox.draw(window);
        }
        itemName.setText(itemList[i].second);
        itemName.setPosition(Vector2f(445,105+(i-topItemShown)*55));
        itemName.draw(window);
    }

    if (topItemShown>0)
        upArrow.draw(window);
    if (unsigned(topItemShown+5)<itemList.size())
        downArrow.draw(window);
}

int BagItemList::getCurrentItem()
{
    if (itemList.size()==0)
        return 0;
    return itemList[curItem].first;
}
