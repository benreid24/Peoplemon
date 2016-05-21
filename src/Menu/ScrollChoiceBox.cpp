#include "Menu/ScrollChoiceBox.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
#include "World/People/PlayerInput.hpp"
#include <iostream>
using namespace sf;
using namespace std;

ScrollChoiceBox::ScrollChoiceBox() : pointer(3)
{
    lastTime = topChoice = 0;
    pointerChoice = 1;
    choiceChosen = false;
    choicesVisible = 5;
    renderText.setFont(Properties::PrimaryMenuFont);
    renderText.setCharacterSize(18);
    renderText.setColor(Color::Black);
    pointer.setPoint(0, Vector2f(0,0));
    pointer.setPoint(1, Vector2f(-5, -3));
    pointer.setPoint(2, Vector2f(-5,3));
    pointer.setFillColor(Color::Black);
    box.setFillColor(Color::Transparent);
}

Vector2f ScrollChoiceBox::getChoicePos(int c)
{
    if (textSize.x==0 || textSize.y==0)
    {
        for (unsigned int i = 0; i<choices.size(); ++i)
        {
            renderText.setString(choices[i]);
            if (textSize.x<renderText.getGlobalBounds().width)
                textSize.x = renderText.getGlobalBounds().width;
        }
        textSize.y = renderText.getGlobalBounds().height;
    }

    return Vector2f(position.x, position.y+(c-topChoice)*(textSize.y+10));
}

void ScrollChoiceBox::setPosition(Vector2f pos)
{
    position = pos;
}

void ScrollChoiceBox::useSecondaryFont()
{
    textSize.x = 0;
    renderText.setFont(Properties::SecondaryMenuFont);
}

void ScrollChoiceBox::setVisibleChoices(int v)
{
    choicesVisible = v;
}

void ScrollChoiceBox::setTextProps(Color col, int size)
{
    textSize.x = 0;
    renderText.setColor(col);
    pointer.setFillColor(col);
    renderText.setCharacterSize(size);
    pointer.setPoint(0, Vector2f(0,0));
    pointer.setPoint(1, Vector2f(-float(size)*0.3, -float(size)*0.16));
    pointer.setPoint(2, Vector2f(-float(size)*0.3, float(size)*0.16));
}

void ScrollChoiceBox::setBackgroundColor(Color col)
{
    box.setFillColor(col);
}
void ScrollChoiceBox::setBorder(Color col, int t)
{
    box.setOutlineColor(col);
    box.setOutlineThickness(t);
}

void ScrollChoiceBox::addChoice(string s)
{
    textSize.x = 0;
    choices.push_back(s);
}

void ScrollChoiceBox::setChoice(int i)
{
    topChoice = i;
    pointerChoice = 0;
}

void ScrollChoiceBox::setChoice(string s)
{
    for (unsigned int i = 0; i<choices.size(); ++i)
    {
        if (choices[i]==s)
        {
            topChoice = i;
            pointerChoice = 0;
            return;
        }
    }
}

string ScrollChoiceBox::getChoice()
{
    if (choiceChosen)
        return choices[topChoice+pointerChoice-1];
    return string("");
}

string ScrollChoiceBox::getCurrentChoice()
{
    return choices[topChoice+pointerChoice-1];
}

void ScrollChoiceBox::update()
{
    if (gameClock.getTimeStamp()-lastTime>200)
    {
        if (user.isInputActive(PlayerInput::Up))
        {
            if (pointerChoice>1)
                pointerChoice--;
            else if (topChoice>0)
                topChoice--;

            lastTime = gameClock.getTimeStamp();
        }
        else if (user.isInputActive(PlayerInput::Down))
        {
            if (unsigned(pointerChoice+topChoice)<choices.size())
            {
                if (pointerChoice<choicesVisible || choices.size()<unsigned(choicesVisible))
                    pointerChoice++;
                else
                    topChoice++;
                lastTime = gameClock.getTimeStamp();
            }
        }
        if (user.isInputActive(PlayerInput::Interact))
            choiceChosen = true;
    }
}

void ScrollChoiceBox::clear()
{
    choices.clear();
    topChoice = 0;
    choiceChosen = false;
    pointerChoice = 1;
}

void ScrollChoiceBox::reset()
{
    choiceChosen = false;
}

void ScrollChoiceBox::draw(RenderWindow* window)
{
    if (choices.size()>0)
    {
        box.setPosition(position-Vector2f(textSize.y*0.5, 3));
        box.setSize(Vector2f(textSize.y*0.5+textSize.x+5, choicesVisible*(textSize.y+10)+5));
        pointer.setPosition(getChoicePos(topChoice+pointerChoice)-Vector2f(textSize.y*0.15,textSize.y*0.5));

        window->draw(box);
        for (int i = topChoice; i<topChoice+choicesVisible && unsigned(i)<choices.size(); ++i)
        {
            renderText.setPosition(getChoicePos(i));
            renderText.setString(choices[i]);
            window->draw(renderText);
        }
        window->draw(pointer);
    }
}
