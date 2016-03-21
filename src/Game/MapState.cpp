#include "Game/MapState.hpp"
#include "Globals.hpp"
#include "Util/File.hpp"
#include "Properties.hpp"
#include "Game/Game.hpp"
#include <iostream>
using namespace std;
using namespace sf;

MapState::MapState(Game* g, bool cF) : Gamestate(g)
{
    canFly = cF;
    //TODO - figure out how to initialize navPos with scaled player position from the world. maybe have an icon there
    backgroundTxtr = imagePool.loadResource(Properties::MenuImagePath+"mapBgnd.png");
    townTxtr = imagePool.loadResource(Properties::MenuImagePath+"mapTown.png");
    crossHairTxtr = imagePool.loadResource(Properties::MenuImagePath+"mapPointer.png");
    background.setTexture(*backgroundTxtr);
    town.setTexture(*townTxtr);
    crossHair.setTexture(*crossHairTxtr);

    MapObject temp;
    File file(Properties::DataPath+"map.db");

    int size = file.get<uint16_t>();
    for (int i = 0; i<size; ++i)
    {
        temp.pos.x = file.get<uint16_t>();
        temp.pos.y = file.get<uint16_t>();
        temp.displayName = file.getString();
        temp.referenceName = file.getString();
        temp.mapName = file.getString();
        temp.spId = file.get<uint16_t>();
        towns.push_back(temp);
        //TODO - init visited from map data
    }
}

bool MapState::execute()
{
    //TODO - assume that the "map" is 32 by 32 increments for now, figure out a scale and a way to convert player world pos to travel map pos
    navPos = Vector2i(15,15);

    while (!finishFrame())
    {
        if (user.isInputActive(PlayerInput::Up) && navPos.y>0)
            navPos.y--;
        if (user.isInputActive(PlayerInput::Right) && navPos.x<31)
            navPos.x++;
        if (user.isInputActive(PlayerInput::Down) && navPos.y<31)
            navPos.y++;
        if (user.isInputActive(PlayerInput::Left) && navPos.x>0)
            navPos.x--;

        if (user.isInputActive(PlayerInput::Interact))
        {
            if (navPos.x>=30 && navPos.y==31)
            {
                game->data.pauseGameFlag = false;
                return false;
            }
            for (unsigned int i = 0; i<towns.size(); ++i)
            {
                if (navPos.x==towns[i].pos.x && navPos.y==towns[i].pos.y)
                {
                    if (towns[i].visited && canFly)
                    {
                        game->world.load(towns[i].mapName,towns[i].spId);
                        game->data.pauseGameFlag = false;
                        return false;
                    }
                }
            }
        }

        if (user.isInputActive(PlayerInput::Run))
		{
			game->data.pauseGameFlag = false;
			return false;
		}

        crossHair.setPosition(Properties::ScreenWidth/32*navPos.x+16,Properties::ScreenHeight/32*navPos.y+16);
        game->mainWindow.draw(background);
        for (unsigned int i = 0; i<towns.size(); ++i)
        {
            town.setPosition(Properties::ScreenWidth/32*towns[i].pos.x,Properties::ScreenHeight/32*towns[i].pos.y);
            game->mainWindow.draw(town);
        }
        game->mainWindow.draw(crossHair);
        game->mainWindow.display();
        sleep(milliseconds(60));
    }

    return true;
}
