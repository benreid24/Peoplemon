#include "Menu/HUD.hpp"
#include "Resources/ResourcePool.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace sf;
using namespace std;

HUD::HUD(Game* g)
{
    game = g;
    displayingMessage = gettingChoice = false;
    alwaysShow = false;
    timeTillDone = 0;
    ghostTimer = 0;
    minDelay = 0;
    wrapW = 390;
    position = Vector2f(200,500);
    textBox.setImage("messageBox.png");
    textBox.setPosition(position);
    messageText.setProps(Color::Black,20);
    messageText.setPosition(position+Vector2f(5,5));
}

string HUD::wordWrap(string str)
{
    Text temp;
    temp.setFont(Properties::PrimaryMenuFont);
    temp.setCharacterSize(20);
    temp.setString(str);
    for (unsigned int i = 0; i<str.size(); ++i)
    {
        if (temp.findCharacterPos(i).x>=wrapW)
        {
            for (int j = i; j>=0; j--)
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

void HUD::rePosition(Vector2f pos, int w)
{
	wrapW = w;
	position = pos;
	textBox.setPosition(position);
	messageText.setPosition(position+Vector2f(5,5));
}

Vector2f HUD::getPosition()
{
	return position;
}

int HUD::getWrapWidth()
{
	return wrapW;
}

bool HUD::isAlwaysShowing()
{
	return alwaysShow;
}

void HUD::displayMessage(string m, string sound, bool block)
{
    minDelay = gameClock.getTimeStamp()+500;
    message = wordWrap(m);
    messageShown.clear();
    if (sound.size())
        game->soundEngine.playSound(sound);
    displayingMessage = true;
    if (block)
	{
		while (displayingMessage)
		{
			sleep(milliseconds(100));
		}
	}

}

bool HUD::messageFinished()
{
    return !displayingMessage;
}

void HUD::setAlwaysShow(bool as)
{
    alwaysShow = as;
}

std::string HUD::getChoice(string message, vector<string> opts, bool block)
{
	minDelay = gameClock.getTimeStamp()+500;
    message = wordWrap(message);
    messageText.setText(message);
    choiceBox.clear();
    for (unsigned int i = 0; i<opts.size(); ++i)
    {
        choiceBox.addChoice(opts[i]);
    }
    choiceBox.setBackgroundColor(Color::White);
    choiceBox.setBorder(Color::Black, 3);
    choiceBox.setTextProps(Color::Black, 18);
    choiceBox.setPosition(Vector2f(620,600-choiceBox.getSize().y));
    gettingChoice = true;
    if (block)
    {
        while (choiceBox.getChoice().size()==0)
        {
            sleep(milliseconds(100));
        }
        return choiceBox.getChoice();
    }
    return "";
}

std::string HUD::getChoice()
{
    return choiceBox.getChoice();
}

void HUD::playAnimation(string file, int x, int y, int loops)
{
    Animation t(AnimationReference(animPool.loadResource(Properties::AnimationPath+file)));
    t.setPosition(Vector2f(x,y));
    t.play();
    playingAnims.push_back(make_pair(make_pair(0,loops),t));
}

void HUD::update()
{
    if (displayingMessage)
    {
    	//Properties::PrimaryMenuFont.loadFromFile(Properties::FontPath+"Dream.ttf");
        if (user.isInputActive(PlayerInput::Interact) && gameClock.getTimeStamp()>minDelay)
        {
            if (messageShown!=message)
            {
                messageShown = message;
                timeTillDone = gameClock.getTimeStamp()+350;
            }
            else if (gameClock.getTimeStamp()>=timeTillDone)
            {
                displayingMessage = false;
                timeTillDone = 0;
            }
        }
        else if (message!=messageShown)
        {
            if (gameClock.getTimeStamp()-ghostTimer>=30)
            {
                messageShown.push_back(message[messageShown.size()]);
                ghostTimer = gameClock.getTimeStamp();
            }
        }
        else if (timeTillDone==0)
            timeTillDone = gameClock.getTimeStamp()+550;

        messageText.setText(messageShown);
    }
    if (gettingChoice && choiceBox.getChoice().size()==0 && gameClock.getTimeStamp()>minDelay)
        choiceBox.update();
    else if (choiceBox.getChoice().size()>0)
        gettingChoice = false;

    for (unsigned int i = 0; i<playingAnims.size(); ++i)
    {
        playingAnims[i].second.update();
        if (playingAnims[i].second.finished())
        {
            playingAnims[i].first.first++;
            if (playingAnims[i].first.first>=playingAnims[i].first.second)
            {
                playingAnims.erase(playingAnims.begin()+i);
                i--;
            }
            else
                playingAnims[i].second.play();
        }
    }
}

void HUD::draw(RenderWindow* window)
{
    if (displayingMessage || alwaysShow)
    {
        textBox.draw(window);
        messageText.draw(window);
    }
    if (gettingChoice)
    {
        textBox.draw(window);
        messageText.draw(window);
        choiceBox.draw(window);
    }
    for (unsigned int i = 0; i<playingAnims.size(); ++i)
    {
        playingAnims[i].second.draw(window);
    }
}
