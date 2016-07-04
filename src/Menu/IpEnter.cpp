#include "Menu/IpEnter.hpp"
#include "Globals.hpp"
#include "World/People/PlayerInput.hpp"
#include <cmath>
using namespace sf;
using namespace std;

IpEnter::IpEnter() : address{0,0,0,0,0,0,0,0,0,0,0,0,0,0}
{
	curSpot = 0;
	finished = false;
	background.setImage("ipEnter.png");
	buttonHighlight.setImage("ipEnterButtonHighlight.png");
	numberHighlight.setImage("ipEnterNumberHighlight.png");

	instructions.setText("Enter the IP address and port of the person you want to connect to\nUse the left and right controls to switch digits, and use the up and down controls to modify them\nGo to 22ndcg.com if you need help setting up your game");
	instructions.setProps(Color::White, 26);

	background.setPosition(Vector2f(0,400));
	instructions.setPosition(Vector2f(20,100));
	buttonHighlight.setPosition(Vector2f(332,513));
    for (int i = 0; i<14; ++i)
		digits[i].setPosition(Vector2f(14+i*boxWidth,410));
}

void IpEnter::update()
{
	bool changed = false;
	if (user.isInputActive(PlayerInput::Up) && curSpot<14)
	{
		address[curSpot]++;
		if (address[curSpot]>9)
			address[curSpot] = 0;
		digits[curSpot].setText(numLookup[address[curSpot]]);
		changed = true;
	}
	else if (user.isInputActive(PlayerInput::Down) && curSpot<14)
	{
		address[curSpot]--;
		if (address[curSpot]<0)
			address[curSpot] = 9;
		digits[curSpot].setText(numLookup[address[curSpot]]);
		changed = true;
	}
	else if (user.isInputActive(PlayerInput::Right))
	{
		if (curSpot==14)
			curSpot = 13;
		else
			curSpot++;
		changed = true;
	}
	else if (user.isInputActive(PlayerInput::Left))
	{
		if (curSpot==14)
			curSpot = 0;
		else if (curSpot==0)
			curSpot = 14;
		else
			curSpot--;
		changed = true;
	}

	if (user.isInputActive(PlayerInput::Interact) && curSpot==14)
	{
		bool good = false;
		for (int i = 0; i<14; ++i)
		{
			if (address[i]!=0)
				good = true;
		}
		if (good)
			finished = true;
	}

	if (changed)
		numberHighlight.setPosition(Vector2f(curSpot*boxWidth,0));
}

bool IpEnter::done()
{
	return finished;
}

void IpEnter::reset()
{
	finished = false;
}

IpAddress IpEnter::getIp()
{
	int sections[4] = {0,0,0,0};

	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<3; ++j)
		{
			sections[i] += address[i*3+j]*pow(10,2-j);
		}
	}

	return IpAddress(sections[0],sections[1],sections[2],sections[3]);
}

int IpEnter::getPort()
{
	int port = 0;

	for (int i = 0; i<5; ++i)
		port += address[9+i]*pow(10,4-i);

	return port;
}

void IpEnter::draw(sf::RenderWindow* window)
{
	if (curSpot==14)
		buttonHighlight.draw(window);
	else
		numberHighlight.draw(window);
	background.draw(window);
	instructions.draw(window);
	for (int i = 0; i<14; ++i)
		digits[i].draw(window);
}
