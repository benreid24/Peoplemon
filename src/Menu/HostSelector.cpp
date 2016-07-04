#include "Menu/HostSelector.hpp"
#include "World/People/PlayerInput.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

HostSelector::HostSelector()
{
	curHost = 0;
	finished = false;
	box.setImage("hostBut.png");
	selBox.setImage("hostButLight.png");
	title.setText("Games on the local network:");
	title.setPosition(Vector2f(200,25));
	title.setProps(Color::Cyan, 36);
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
		if (hosts[i].first>=10)
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

	if (user.isInputActive(PlayerInput::Up) && curHost>0)
		curHost--;
	else if (user.isInputActive(PlayerInput::Down) && curHost<hosts.size()-1)
		curHost++;

	if (user.isInputActive(PlayerInput::Interact))
		finished = true;

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
	for (unsigned int i = 0; i<hosts.size(); ++i)
	{
		box.setPosition(Vector2f(0,100+i*120));
		selBox.setPosition(Vector2f(0,100+i*120));
		name.setPosition(Vector2f(50,120+i*120));
		ip.setPosition(Vector2f(200,120+i*120));
		port.setPosition(Vector2f(400,120+i*120));
		name.setText(hosts[i].second.name);
        ip.setText(hosts[i].second.ip.toString());
        port.setText(intToString(hosts[i].second.port));
        if (i==curHost)
			selBox.draw(window);
		else
			box.draw(window);
		name.draw(window);
		ip.draw(window);
		port.draw(window);
	}
}
