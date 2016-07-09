#include "Menu/HostSelector.hpp"
#include "World/People/PlayerInput.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

HostSelector::HostSelector()
{
	curHost = 0;
	lastTime = 0;
	finished = false;
	box.setImage("hostBut.png");
	selBox.setImage("hostButLight.png");
	title.setText("Games on the local network:");
	tip.setText("Press the run button to go back");
	title.setPosition(Vector2f(200,25));
	title.setProps(Color::Cyan, 36);
	tip.setPosition(Vector2f(240,70));
	tip.setProps(Color::Cyan, 26);
}

void HostSelector::setNetwork(Network* n)
{
	network = n;
}

void HostSelector::updateHosts()
{
	vector<HostSettings> vec = network->pollLocalHosts();

	//determine misses
	for (unsigned int i = 0; i<hosts.size(); ++i)
	{
		for (unsigned int j = 0; j<vec.size(); ++j)
		{
			if (vec[j].name==hosts[i].second.name && vec[j].ip==hosts[i].second.ip)
			{
				vec.erase(vec.begin()+j);
				goto noInc;
			}
		}
        hosts[i].first++;
        noInc:;
	}

	//add new hosts
	for (unsigned int i = 0; i<vec.size(); ++i)
		hosts.push_back(make_pair(0,vec[i]));

	//remove hosts with too many misses
    for (unsigned int i = 0; i<hosts.size(); ++i)
	{
		if (hosts[i].first>=300)
		{
			if (curHost>i)
				curHost--;
			hosts.erase(hosts.begin()+i);
			i--;
			if (curHost>=hosts.size())
				curHost = hosts.size()-1;
		}
	}
}

void HostSelector::update()
{
	updateHosts();

	if (gameClock.getTimeStamp()-lastTime>150)
	{
		if (user.isInputActive(PlayerInput::Up) && curHost>0)
		{
			curHost--;
			lastTime = gameClock.getTimeStamp();
		}
		else if (user.isInputActive(PlayerInput::Down) && curHost<hosts.size()-1)
		{
			curHost++;
			lastTime = gameClock.getTimeStamp();
		}

		if (user.isInputActive(PlayerInput::Interact))
		{
			finished = true;
			lastTime = gameClock.getTimeStamp();
		}
	}

	if (curHost>=hosts.size())
		curHost = hosts.size()-1;
}

void HostSelector::reset()
{
	finished = false;
}

bool HostSelector::done()
{
	return finished;
}

HostSettings HostSelector::getSelectedHost()
{
	return hosts[curHost].second;
}

void HostSelector::draw(RenderWindow* window)
{
	title.draw(window);
	tip.draw(window);
	for (unsigned int i = 0; i<hosts.size(); ++i)
	{
		box.setPosition(Vector2f(0,100+i*80));
		selBox.setPosition(Vector2f(0,100+i*80));
		name.setPosition(Vector2f(80,135+i*80));
		ip.setPosition(Vector2f(230,135+i*80));
		port.setPosition(Vector2f(430,135+i*80));
		name.setText(hosts[i].second.name);
        ip.setText(hosts[i].second.ip.toString());
        port.setText(intToString(hosts[i].second.port));
        if (i==curHost)
			selBox.draw(window);
		box.draw(window);
		name.draw(window);
		ip.draw(window);
		port.draw(window);
	}
}
