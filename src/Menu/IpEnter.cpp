#include "Menu/IpEnter.hpp"
#include "Globals.hpp"
#include "World/People/PlayerInput.hpp"
#include <cmath>
using namespace sf;
using namespace std;

IpEnter::IpEnter() : address{0,0,0,0,0,0,0,0,0,0,0,0,0,0}
{
	curSpot = 0;
	lastTime = 0;
	finished = false;
	background.setImage("ipEnter.png");
	buttonHighlight.setImage("ipEnterButtonHighlight.png");
	numberHighlight.setImage("ipEnterNumberHighlight.png");

	instructions.setText("Enter the IP address and port of the person you want to connect to\nUse the left and right controls to switch digits,\n        and use the up and down controls to modify them\nGo to 22ndcg.com if you need help setting up your game");
	instructions.setProps(Color::White, 26);

	background.setPosition(Vector2f(0,400));
	instructions.setPosition(Vector2f(20,30));
	buttonHighlight.setPosition(Vector2f(310,450));
	numberHighlight.setPosition(Vector2f(3,400));
    for (int i = 0; i<17; ++i)
	{
		digits[i].setPosition(Vector2f(25+i*boxWidth,415));
		digits[i].setProps(Color::Black, 28);
	}
}

void IpEnter::update()
{
	if (gameClock.getTimeStamp()-lastTime<150)
		return;

	if (user.isInputActive(PlayerInput::Up) && curSpot<17)
	{
		address[curSpot]++;
		if (address[curSpot]>9 || ((curSpot==0 || curSpot==3 || curSpot==6 || curSpot==9) && address[curSpot]>2) || (curSpot==12 && address[curSpot]>6))
			address[curSpot] = 0;
		lastTime = gameClock.getTimeStamp();
		digits[curSpot].setText(numLookup[address[curSpot]]);
		if (curSpot<12)
		{
			for (int i = curSpot; i%3!=0 || i==curSpot; ++i)
				digits[i].setText(numLookup[address[i]]);
		}
		else
		{
			for (int i = curSpot; i<17; ++i)
				digits[i].setText(numLookup[address[i]]);
		}
	}
	else if (user.isInputActive(PlayerInput::Down) && curSpot<17)
	{
		address[curSpot]--;
		if (address[curSpot]<0)
		{
			if (curSpot==0 || curSpot==3 || curSpot==6 || curSpot==9)
				address[curSpot] = 2;
			else if (curSpot==12)
				address[curSpot] = 6;
			else
				address[curSpot] = 9;
		}
		lastTime = gameClock.getTimeStamp();
		digits[curSpot].setText(numLookup[address[curSpot]]);
		if (curSpot<12)
		{
			for (int i = curSpot; i%3!=0 || i==curSpot; ++i)
				digits[i].setText(numLookup[address[i]]);
		}
		else
		{
			for (int i = curSpot; i<17; ++i)
				digits[i].setText(numLookup[address[i]]);
		}
	}
	else if (user.isInputActive(PlayerInput::Right))
	{
		if (curSpot==17)
			curSpot = 16;
		else
			curSpot++;
		numberHighlight.setPosition(Vector2f(curSpot*boxWidth+3,400));
		lastTime = gameClock.getTimeStamp();
	}
	else if (user.isInputActive(PlayerInput::Left))
	{
		if (curSpot==17)
			curSpot = 0;
		else if (curSpot==0)
			curSpot = 17;
		else
			curSpot--;
		numberHighlight.setPosition(Vector2f(curSpot*boxWidth+3,400));
		lastTime = gameClock.getTimeStamp();
	}

	if (user.isInputActive(PlayerInput::Interact) && curSpot==17)
	{
		bool good = false;
		for (int i = 0; i<17; ++i)
		{
			if (address[i]!=0)
				good = true;
		}
		if (good)
			finished = true;
		lastTime = gameClock.getTimeStamp();
	}
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
	string ip;

	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<3; ++j)
			ip.push_back(address[i*3+j]+48);
		ip.push_back('.');
	}

	return IpAddress(ip);
}

int IpEnter::getPort()
{
	string port;

	for (int i = 0; i<5; ++i)
		port.push_back(address[12+i]+48);

	return stringToInt(port);
}

void IpEnter::draw(sf::RenderWindow* window)
{
	if (curSpot==17)
		buttonHighlight.draw(window);
	else
		numberHighlight.draw(window);
	background.draw(window);
	instructions.draw(window);
	for (int i = 0; i<17; ++i)
		digits[i].draw(window);
}
