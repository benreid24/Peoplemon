#include "Menu/BagSectionSelector.hpp"
#include "Properties.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

BagSectionSelector::BagSectionSelector()
{
    sections[0] = "Items";
    sections[1] = "Key Items";
    sections[2] = "TM's";
    index = 0;
    curSection.setProps(Color::Red, 28);
    curSection.setText("Items");
    curSection.setPosition(Vector2f(115,345));
}

void BagSectionSelector::update()
{
    if (user.isInputActive(PlayerInput::Left))
    {
        index--;
        if (index<0)
            index = 2;
    }
    if (user.isInputActive(PlayerInput::Right))
    {
        index++;
        if (index>2)
            index = 0;
    }
    curSection.setText(sections[index]);
}

void BagSectionSelector::draw(RenderWindow* window)
{
    curSection.draw(window);
}

string BagSectionSelector::getCurrentSection()
{
    return sections[index];
}
