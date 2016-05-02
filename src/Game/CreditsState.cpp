#include "Game/CreditsState.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
using namespace std;
using namespace sf;

CreditsObject::CreditsObject(File* file, int lastPos)
{
    isImage = file->get<uint8_t>()==0;
    int x = file->get<uint16_t>();
    yPos = lastPos+file->get<uint16_t>();
    if (isImage)
    {
        txtr = imagePool.loadResource(Properties::CreditsPath+file->getString());
        spr.setTexture(*txtr);
        spr.setPosition(x,0);
    }
    else
    {
        string t = file->getString();
        int r = file->get<uint8_t>();
        int g = file->get<uint8_t>();
        int b = file->get<uint8_t>();
        int s = file->get<uint16_t>();
        text.setFont(Properties::PrimaryMenuFont);
        text.setString(t);
        text.setColor(Color(r,g,b));
        text.setCharacterSize(s);
        text.setPosition(x,0);
    }
}

void CreditsObject::draw(RenderWindow* window, int yOff)
{
    if (isImage)
    {
        spr.setPosition(spr.getPosition().x,yPos-yOff);
        window->draw(spr);
    }
    else
    {
        text.setPosition(text.getPosition().x,yPos-yOff);
        window->draw(text);
    }
}

int CreditsObject::getPos()
{
    return yPos+text.getCharacterSize();
}

CreditsState::CreditsState(Game* g) : Gamestate(g)
{
    File file(Properties::DataPath+"credits.cr");
    int size = file.get<uint16_t>();
    int y = 0;
    for (int i = 0; i<size; ++i)
    {
        objects.push_back(CreditsObject(&file,y));
        y = objects[objects.size()-1].getPos();
    }
    file.close();
    goToPos = y+10;
}

bool CreditsState::execute()
{
    int y = -Properties::ScreenHeight;

    while (!finishFrame())
    {
        game->mainWindow.clear();
        for (unsigned int i = 0; i<objects.size(); ++i)
        {
            objects[i].draw(&game->mainWindow,y);
        }
        game->mainWindow.display();

        y += 2;
        if (y>=goToPos)
            return false;

        sleep(milliseconds(30));
    }
    return true;
}
