#include "Menu/Menu.hpp"
using namespace std;
using namespace sf;

Menu::Menu()
{
    //ctor
}

void Menu::add(MenuText* t)
{
    text.push_back(t);
}

void Menu::add(MenuImage* i)
{
    images.push_back(i);
}

void Menu::add(ChoiceBox* c)
{
    choices.push_back(c);
}

void Menu::add(ScrollChoiceBox* s)
{
    scrolls.push_back(s);
}

void Menu::draw(RenderWindow* window)
{
    for (unsigned int i = 0; i<images.size(); ++i)
    {
        images[i]->draw(window);
    }
    for (unsigned int i = 0; i<text.size(); ++i)
    {
        text[i]->draw(window);
    }
    for (unsigned int i = 0; i<choices.size(); ++i)
    {
        choices[i]->draw(window);
    }
    for (unsigned int i = 0; i<scrolls.size(); ++i)
    {
        scrolls[i]->draw(window);
    }
}
