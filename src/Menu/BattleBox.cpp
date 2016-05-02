#include "Menu/BattleBox.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

BattleBox::BattleBox() : hpBar(Vector2f(0,10)), xpBar(Vector2f(0,7))
{
    isPlayer = false;
    position = Vector2f(0,50);
    hpBar.setPosition(position+Vector2f(100,50));
    xpBar.setFillColor(Color::Blue);
    boxTxtr = imagePool.loadResource(Properties::BattleImagePath+"opBox.png");
    box.setTexture(*boxTxtr,true);
    box.setPosition(position);
    name.useSecondaryFont();
    level.useSecondaryFont();
    level.useSecondaryFont();
    name.setPosition(position+Vector2f(5,18));
    name.setProps(Color::Black,24);
    level.setPosition(position+Vector2f(167,14));
    level.setProps(Color::Red,22);
    hpTargetW = 0;
    xpTargetW = 0;
}

void BattleBox::setIsPlayer()
{
    isPlayer = true;
    position = Vector2f(540,360);
    hpBar.setPosition(position+Vector2f(149,54));
    xpBar.setPosition(position+Vector2f(52,88));
    boxTxtr = imagePool.loadResource(Properties::BattleImagePath+"pBox.png");
    box.setTexture(*boxTxtr,true);
    box.setPosition(position);
    name.setPosition(position+Vector2f(38,23));
    level.setPosition(position+Vector2f(215,18));
    curHp.setPosition(position+Vector2f(155,60));
    curHp.setProps(Color::Black,20);
}

void BattleBox::update(PeoplemonRef ppl, bool grad)
{
    name.setText(ppl.name);
    level.setText(intToString(ppl.level));
    hpString = intToString(ppl.curHp)+"/"+intToString(ppl.stats.hp);
    curHp.setText(hpString);
    hpTargetW = double(ppl.curHp)/double(ppl.stats.hp)*96;
    xpTargetW = double(ppl.curXp)/double(ppl.nextLvlXp)*192;
    if (hpTargetW<0)
        hpTargetW = 0;
    if (hpTargetW>96)
        hpTargetW = 96;
    if (xpTargetW<0)
        xpTargetW = 0;
    if (xpTargetW>192)
        xpTargetW = 192;
    if (!grad)
    {
        hpBar.setSize(Vector2f(hpTargetW, hpBar.getSize().y));
        xpBar.setSize(Vector2f(xpTargetW, xpBar.getSize().y));
    }
    double hp = hpBar.getSize().x/96.0;
    if (hp<0.25)
        hpBar.setFillColor(Color::Red);
    else if (hp<0.4)
        hpBar.setFillColor(Color::Yellow);
    else
        hpBar.setFillColor(Color::Green);
}

void BattleBox::update()
{
	curHp.setText(hpString);
    if (abs(hpBar.getSize().x-hpTargetW)>1)
    {
        if (hpBar.getSize().x<hpTargetW)
            hpBar.setSize(hpBar.getSize()+Vector2f(3,0));
        else
            hpBar.setSize(hpBar.getSize()+Vector2f(-3,0));

		if (hpBar.getSize().x<0)
			hpBar.setSize(Vector2f(0,hpBar.getSize().y));
		if (hpBar.getSize().x>96)
			hpBar.setSize(Vector2f(96,hpBar.getSize().y));
		if (abs(hpBar.getSize().x-hpTargetW)<3)
			hpBar.setSize(Vector2f(hpTargetW,hpBar.getSize().y));
    }
    if (abs(xpBar.getSize().x-xpTargetW)>1)
    {
        if (xpBar.getSize().x<xpTargetW)
            xpBar.setSize(xpBar.getSize()+Vector2f(3,0));
        else
            xpBar.setSize(xpBar.getSize()+Vector2f(-3,0));

		if (xpBar.getSize().x<0)
			xpBar.setSize(Vector2f(0,xpBar.getSize().y));
		if (xpBar.getSize().x>192)
			xpBar.setSize(Vector2f(192,xpBar.getSize().y));
		if (abs(xpBar.getSize().x-xpTargetW)<3)
			xpBar.setSize(Vector2f(xpTargetW,xpBar.getSize().y));
    }
    double hp = hpBar.getSize().x/96;
    if (hp<0.25)
        hpBar.setFillColor(Color::Red);
    else if (hp<0.4)
        hpBar.setFillColor(Color::Yellow);
    else
        hpBar.setFillColor(Color::Green);
}

bool BattleBox::barGood()
{
	if (!isPlayer)
		return int(hpBar.getSize().x+0.5)==hpTargetW;
	return int(hpBar.getSize().x+0.5)==hpTargetW && int(xpBar.getSize().x+0.5)==xpTargetW;
}

void BattleBox::draw(RenderWindow* window)
{
    window->draw(hpBar);
    if (isPlayer)
        window->draw(xpBar);
    window->draw(box);
    level.draw(window);
    name.draw(window);
    if (isPlayer)
        curHp.draw(window);
}
