#include "Menu/MenuText.hpp"
#include "Properties.hpp"
using namespace sf;
using namespace std;

MenuText::MenuText()
{
    text.setFont(Properties::PrimaryMenuFont);
    m = true;
}

MenuText::MenuText(string t)
{
    text.setFont(Properties::PrimaryMenuFont);
    text.setString(t);
}

void MenuText::useSecondaryFont()
{
    text.setFont(Properties::SecondaryMenuFont);
    m = false;
}

void MenuText::setText(string t)
{
    text.setString(t);
    if (m)
		text.setFont(Properties::PrimaryMenuFont);
	else
		text.setFont(Properties::SecondaryMenuFont);
}

void MenuText::setProps(Color col, int size)
{
    text.setColor(col);
    text.setCharacterSize(size);
}

void MenuText::setPosition(Vector2f pos)
{
    text.setPosition(pos);
}

void MenuText::draw(sf::RenderWindow* window)
{
    window->draw(text);
}
