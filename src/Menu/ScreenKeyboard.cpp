#include "Menu/ScreenKeyboard.hpp"
#include "Globals.hpp"
#include "World/People/PlayerInput.hpp"
#include <iostream>
using namespace sf;
using namespace std;

ScreenKeyboard::ScreenKeyboard(string i)
{
    finished = false;
    background.setImage("keyboard.png");
    background.setPosition(Vector2f(0,300));
    text.setPosition(Vector2f(10,305));
    text.setProps(Color::Black,22);
    typedText = i;
    text.setText(typedText);

    for (int i = 'A'; i<='Z'; ++i)
        keys.addChoice(string(1,i));
    for (int i = 0; i<4; ++i)
        keys.addChoice(" ");
    for (int i = 'a'; i<='z'; ++i)
        keys.addChoice(string(1,i));
    keys.addChoice("Back");
    keys.addChoice("~");

    keys.setTextProps(Color::Black,22);
    keys.setPosition(Vector2f(15,362));
    keys.setAllowedCols(15);
    keys.setSpeed(100);
    lastTime = 0;
    charLim = 100;
}

void ScreenKeyboard::setInputLimit(int l)
{
    charLim = l;
}

void ScreenKeyboard::update()
{
    keys.update();
    if (keys.getChoice().size()>0 || user.isInputActive(PlayerInput::Run))
    {
    	if (gameClock.getTimeStamp()-lastTime>500 || (keys.getChoice()!=lastInput && gameClock.getTimeStamp()-lastTime>200))
        {
        	lastTime = gameClock.getTimeStamp();
			string t = keys.getChoice();
			keys.reset();
			if (t=="Back" || user.isInputActive(PlayerInput::Run))
			{
				if (typedText.size()>0)
					typedText.erase(typedText.size()-1);
			}
			else if (t=="~")
				finished = true;
			else if (typedText.size()+1<=unsigned(charLim))
			{
				typedText += t;
				text.setText(typedText);
			}
        }
        else
			keys.reset();
    }
}

string ScreenKeyboard::getText()
{
    return typedText;
}

void ScreenKeyboard::draw(RenderWindow* window)
{
    background.draw(window);
    text.draw(window);
    keys.draw(window);
}

bool ScreenKeyboard::isFinished()
{
    return finished;
}
