#include "Menu/PeopledexList.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
using namespace std;
using namespace sf;

PeopledexList::PeopledexList(Game* g)
{
    done = false;
    curPos = 0;
    upArrow.setImage("pplDexUpArrow.png");
    downArrow.setImage("pplDexDownArrow.png");
    selBox.setImage("pplDexSel.png");
    unSelBox.setImage("pplDexUnsel.png");
    pplName.setProps(Color::Black, 22);
    upArrow.setPosition(Vector2f(575,30));
    downArrow.setPosition(Vector2f(575,550));

    for (auto i = g->peoplemonList.begin(); i!=g->peoplemonList.end(); ++i)
    {
        pplList.push_back(make_pair(i->second.id, i->second.name));
    }
    pplList.push_back(make_pair(-1,"Back"));
}

void PeopledexList::update()
{
    if (user.isInputActive(PlayerInput::Up) && curPos>0)
        curPos--;
    else if (user.isInputActive(PlayerInput::Down) && unsigned(curPos)<pplList.size()-1)
        curPos++;
    else if (user.isInputActive(PlayerInput::Interact) && unsigned(curPos)==pplList.size()-1)
        done = true;
}

void PeopledexList::draw(RenderWindow* window)
{
    int topBut = curPos-4;
    if (unsigned(topBut+9)>pplList.size()-1)
        topBut = pplList.size()-10;
    if (topBut<0)
        topBut = 0;

    for (int i = topBut; i<=topBut+9 && unsigned(i)<pplList.size(); ++i)
    {
        if (curPos==i)
        {
            selBox.setPosition(Vector2f(440, 65+(i-topBut)*55));
            selBox.draw(window);
        }
        else
        {
            unSelBox.setPosition(Vector2f(440, 65+(i-topBut)*55));
            unSelBox.draw(window);
        }
        pplName.setPosition(Vector2f(460, 70+(i-topBut)*55));
        pplName.setText(pplList[i].second);
        pplName.draw(window);
    }
    if (topBut>0)
        upArrow.draw(window);
    if (unsigned(topBut+9)<pplList.size())
        downArrow.draw(window);
}

int PeopledexList::getCurPeoplemon()
{
    return pplList[curPos].first;
}

bool PeopledexList::isFinished()
{
    return done;
}
