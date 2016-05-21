#include "Menu/MenuImage.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace sf;
using namespace std;

MenuImage::MenuImage()
{
    //ctor
}

MenuImage::MenuImage(string file, sf::Vector2f pos)
{
    setImage(file);
    spr.setPosition(pos);
}

void MenuImage::setImage(string file, bool prefixPAth, Vector2f size)
{
    if (prefixPAth)
        file = Properties::MenuImagePath+file;
    txtr = imagePool.loadResource(file);
    spr.setTexture(*txtr);
    if (size.x>0 && size.y>0)
        spr.setScale(size.x/txtr->getSize().x, size.y/txtr->getSize().y);
}

void MenuImage::setSize(Vector2f size)
{
    spr.setScale(size.x/txtr->getSize().x, size.y/txtr->getSize().y);
}

void MenuImage::setOrigin(Vector2f orig)
{
    spr.setOrigin(orig);
}

void MenuImage::setPosition(Vector2f pos)
{
    spr.setPosition(pos);
}

void MenuImage::rotate(int dir)
{
    spr.setRotation(dir);
}

void MenuImage::draw(sf::RenderWindow* window)
{
    window->draw(spr);
}
