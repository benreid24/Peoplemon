#include "Menu/PeoplemonSelector.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
using namespace std;
using namespace sf;

PeoplemonButtonBase::PeoplemonButtonBase()
{
    isActive = false;
}

PeoplemonButtonBase::~PeoplemonButtonBase()
{
    //dtor
}

void PeoplemonButtonBase::makeActive(bool a)
{
    isActive = a;
}

void PeoplemonButtonBase::draw(RenderWindow* window)
{
    if (isActive)
        activePic.draw(window);
    else
        pic.draw(window);
    elements.draw(window);
}

PeoplemonButtonMain::PeoplemonButtonMain(Vector2f pos, PeoplemonRef ppl, Game* g) : PeoplemonButtonBase()
{
    pic.setImage("peoplemonButton.png");
    activePic.setImage("hPeoplemonButton.png");
    hp.setProps(Color::Black, 22);
    name.setProps(Color::Black, 22);
    level.setProps(Color::Black, 22);
    item.setProps(Color::Black, 22);

    pic.setPosition(pos);
    activePic.setPosition(pos);
    name.setPosition(Vector2f(pos.x+132, pos.y+16));
    hp.setPosition(Vector2f(pos.x+167,pos.y+85));
    level.setPosition(Vector2f(pos.x+307, pos.y+20));
    item.setPosition(Vector2f(pos.x+75,pos.y+140));
    hpBar.setPosition(pos.x+163, pos.y+70);

    elements.add(&hp);
    elements.add(&name);
    elements.add(&level);
    elements.add(&item);

    update(ppl, g);
}

PeoplemonButtonMain::~PeoplemonButtonMain()
{
    //dtor
}

void PeoplemonButtonMain::update(PeoplemonRef ppl, Game* g)
{
    name.setText(ppl.name);
    level.setText(intToString(ppl.level));
    if (ppl.holdItem!=0)
        item.setText(g->itemList[ppl.holdItem].name);
    else
        item.setText("None");
    hp.setText(intToString(ppl.curHp)+"/"+intToString(ppl.stats.hp));
    double hpScale = double(ppl.curHp)/double(ppl.stats.hp);
    hpBar.setSize(Vector2f(180*hpScale, 20));
    if (hpScale>0.4)
        hpBar.setFillColor(Color::Green);
    else if (hpScale>0.2)
        hpBar.setFillColor(Color::Yellow);
    else
        hpBar.setFillColor(Color::Red);
}

void PeoplemonButtonMain::draw(RenderWindow* window)
{
    window->draw(hpBar);
    PeoplemonButtonBase::draw(window);
}

PeoplemonButtonCancel::PeoplemonButtonCancel() : PeoplemonButtonBase()
{
    pic.setImage("peoplemonCancel.png");
    activePic.setImage("hPeoplemonCancel.png");
    pic.setPosition(Vector2f(600, 550));
    activePic.setPosition(Vector2f(600,550));
}

PeoplemonButtonCancel::~PeoplemonButtonCancel()
{
    //dtor
}

PeoplemonSelector::PeoplemonSelector(vector<PeoplemonRef>* ppl, Game* g)
{
    curPointer = secondPointer = 0;
    getSecondary = enterPressed = false;
    sync(ppl,g);
}

PeoplemonSelector::~PeoplemonSelector()
{
    for (unsigned int i = 0; i<buttons.size(); ++i)
    {
        delete buttons[i];
    }
}

void PeoplemonSelector::sync(vector<PeoplemonRef>* ppl, Game* g)
{
    bool make = buttons.size()==0;
    for (unsigned int i = 0; i<ppl->size(); ++i)
    {
        if (make)
            buttons.push_back(new PeoplemonButtonMain(Vector2f((i%2)*370+30, (i/2)*180+5), (*ppl)[i], g));
        else
            ((PeoplemonButtonMain*)(buttons[i]))->update((*ppl)[i],g);
    }
    if (make)
        buttons.push_back(new PeoplemonButtonCancel());
    getSecondary = enterPressed = false;
}

int PeoplemonSelector::primarySelection()
{
    return curPointer;
}

int PeoplemonSelector::secondarySelection()
{
    return secondPointer;
}

bool PeoplemonSelector::selectionMade()
{
    bool r = enterPressed;
    enterPressed = false;
    return r;
}

void PeoplemonSelector::setSecondSelection(bool s)
{
    getSecondary = s;
}

void PeoplemonSelector::update()
{
    if (user.isInputActive(PlayerInput::Up))
    {
        if (getSecondary)
        {
            if (secondPointer-2>=0 && secondPointer-2!=curPointer)
                secondPointer -= 2;
        }
        else if (curPointer-2>=0)
            curPointer -= 2;
    }
    else if (user.isInputActive(PlayerInput::Down))
    {
        if (getSecondary)
        {
            if (unsigned(secondPointer+2)<buttons.size() && secondPointer+2!=curPointer)
                secondPointer += 2;
            else if (unsigned(secondPointer+1)<buttons.size() && secondPointer+1!=curPointer)
                secondPointer++;
        }
        else if (unsigned(curPointer+2)<buttons.size())
            curPointer += 2;
        else if (unsigned(curPointer+1)<buttons.size())
            curPointer++;
    }
    else if (user.isInputActive(PlayerInput::Right))
    {
        if (getSecondary)
        {
            if (unsigned(secondPointer+1)<buttons.size() &&secondPointer+1!=curPointer)
                secondPointer++;
        }
        else if (unsigned(curPointer+1)<buttons.size())
            curPointer++;
    }
    else if (user.isInputActive(PlayerInput::Left))
    {
        if (getSecondary)
        {
            if (secondPointer-1>=0 &&secondPointer-1!=curPointer)
                secondPointer--;
        }
        else if (curPointer-1>=0)
            curPointer--;
    }
    if (user.isInputActive(PlayerInput::Interact))
        enterPressed = true;
}

void PeoplemonSelector::draw(RenderWindow* window)
{
    for (unsigned int i = 0; i<buttons.size(); ++i)
    {
        if (i==unsigned(curPointer) || (i==unsigned(secondPointer) && getSecondary))
            buttons[i]->makeActive(true);
        else
            buttons[i]->makeActive(false);
        buttons[i]->draw(window);
    }
}
