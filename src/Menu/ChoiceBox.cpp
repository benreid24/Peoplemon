#include "Menu/ChoiceBox.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
#include <iostream>
using namespace std;
using namespace sf;

ChoiceBox::ChoiceBox() : pointer(3)
{
    speed = 300;
    lastTime = 0;
    curC = 0;
    cCols = 1;
    rowHeight = colWidth = 0;
    choiceChosen = false;
    box.setFillColor(Color(0,0,0,0));
    pointer.setPoint(0, Vector2f(0,0));
    pointer.setPoint(1, Vector2f(-5, -3));
    pointer.setPoint(2, Vector2f(-5,3));
    pointer.setFillColor(Color::Black);
    renderText.setFont(Properties::PrimaryMenuFont);
}

void ChoiceBox::setSpeed(int t)
{
    speed = t;
}

Vector2i ChoiceBox::getChoiceSize(int c)
{
    renderText.setString(choices[c]);
    return Vector2i(renderText.getGlobalBounds().width, renderText.getGlobalBounds().height);
}

Vector2f ChoiceBox::getChoicePos(int c)
{
    if (colWidth==0 || rowHeight==0)
    {
        for (unsigned int i = 0; i<choices.size(); ++i)
        {
            if (getChoiceSize(i).x>colWidth)
                colWidth = getChoiceSize(i).x;
        }
        rowHeight = getChoiceSize(0).y;
    }

    return Vector2f(position.x+(c%cCols)*(colWidth+10), position.y+(c/cCols)*(rowHeight+10));
}

void ChoiceBox::useSecondaryFont()
{
    renderText.setFont(Properties::SecondaryMenuFont);
    rowHeight = 0;
}

void ChoiceBox::setPosition(Vector2f pos)
{
    position = pos;
    box.setPosition(position-Vector2f(3,3));
}

void ChoiceBox::setBackgroundColor(Color col)
{
    box.setPosition(position-Vector2f(3,3));
    box.setSize(getChoicePos(choices.size()-1)+Vector2f(colWidth,rowHeight)-position+Vector2f(6,6));
    box.setFillColor(col);
}

void ChoiceBox::setBorder(Color col, int t)
{
    box.setPosition(position-Vector2f(3,3));
    box.setSize(getChoicePos(choices.size()-1)+Vector2f(colWidth,rowHeight)-position+Vector2f(6,6));
    box.setOutlineColor(col);
    box.setOutlineThickness(t);
}

void ChoiceBox::setTextProps(Color col, int size)
{
    renderText.setColor(col);
    renderText.setCharacterSize(size);
    rowHeight = 0;
    pointer.setPoint(0, Vector2f(0,0));
    pointer.setPoint(1, Vector2f(-float(size)*0.3, -float(size)*0.16));
    pointer.setPoint(2, Vector2f(-float(size)*0.3, float(size)*0.16));
    pointer.setFillColor(col);
}

void ChoiceBox::reset()
{
    choiceChosen = false;
}

void ChoiceBox::addChoice(string c)
{
    choices.push_back(c);
}

void ChoiceBox::setCurChoice(int i)
{
    curC = i;
}

void ChoiceBox::setCurChocie(string c)
{
    for (unsigned int i = 0; i<choices.size(); ++i)
    {
        if (choices[i]==c)
        {
            curC = i;
            break;
        }
    }
}

void ChoiceBox::setAllowedCols(int cols)
{
    cCols = cols;
}

string ChoiceBox::getChoice()
{
    if (choiceChosen)
        return choices[curC];
    return string("");
}

Vector2f ChoiceBox::getSize()
{
	return Vector2f(box.getGlobalBounds().width,box.getGlobalBounds().height);
}

string ChoiceBox::getCurrentChoice()
{
    return choices[curC];
}

void ChoiceBox::setCurrentChoice(string c)
{
    for (unsigned int i = 0; i<choices.size(); ++i)
    {
        if (choices[i]==c)
        {
            curC = i;
            return;
        }
    }
}

void ChoiceBox::clear()
{
    choices.clear();
    choiceChosen = false;
    curC = 0;
}

void ChoiceBox::update()
{
    if (gameClock.getTimeStamp()-lastTime>speed)
    {
        if (user.isInputActive(PlayerInput::Up) && curC-cCols>=0)
        {
            curC -= cCols;
            lastTime = gameClock.getTimeStamp();
        }
        else if (user.isInputActive(PlayerInput::Down) && unsigned(curC+cCols)<choices.size())
        {
            curC += cCols;
            lastTime = gameClock.getTimeStamp();
        }
        else if (user.isInputActive(PlayerInput::Right) && (curC%cCols)!=cCols-1 && unsigned(curC)<choices.size()-1)
        {
            curC++;
            lastTime = gameClock.getTimeStamp();
        }
        else if (user.isInputActive(PlayerInput::Left) && (curC%cCols)!=0 && curC>0)
        {
            curC--;
            lastTime = gameClock.getTimeStamp();
        }
        else if (user.isInputActive(PlayerInput::Interact))
            choiceChosen = true;
    }
}

void ChoiceBox::draw(RenderWindow* window)
{
    if (choices.size()>0)
    {
        pointer.setPosition(getChoicePos(curC+cCols)+Vector2f(-rowHeight*0.15, rowHeight*-0.7));
        box.setPosition(position-Vector2f(3+rowHeight*0.5,3));
        box.setSize(getChoicePos(choices.size()-1)+Vector2f(colWidth,rowHeight+10)-position+Vector2f(6+rowHeight*0.5,6));

        window->draw(box);
        for (unsigned int i = 0; i<choices.size(); ++i)
        {
            renderText.setString(choices[i]);
            renderText.setPosition(getChoicePos(i));
            window->draw(renderText);
        }
        window->draw(pointer);
    }
}
