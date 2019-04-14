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
    backgroundTxtr = imagePool.loadResource(Properties::MenuImagePath+"mapBgnd.png");
    townTxtr = imagePool.loadResource(Properties::MenuImagePath+"mapTown.png");
    crossHairTxtr = imagePool.loadResource(Properties::MenuImagePath+"mapPointer.png");
    sideBoxTxtr = imagePool.loadResource(Properties::MenuImagePath+"mapSideBox.png");
    if (game->player.getGender()=="Boy")
		playerTxtr = imagePool.loadResource(Properties::MenuImagePath+"boyHead.png");
	else
		playerTxtr = imagePool.loadResource(Properties::MenuImagePath+"girlHead.png");


    background.setTexture(*backgroundTxtr);
    town.setTexture(*townTxtr);
    crossHair.setTexture(*crossHairTxtr);
	player.setTexture(*playerTxtr);
	sideBox.setTexture(*sideBoxTxtr);
	sideBox.setPosition(594,0);
	townImage.setPosition(594+25,157);
	if (game->world.getName()=="Worldmap")
	{
		Vector2f pos(float(game->player.getMapPos().x)/float(game->world.getSize().x),float(game->player.getMapPos().y)/float(game->world.getSize().y));
		player.setPosition(pos.x*594,pos.y*600);
	}
	else
		player.setPosition(1000,1000);
	player.setOrigin(playerTxtr->getSize().x/2,playerTxtr->getSize().y/2);

	townName.setPosition(Vector2f(594+18,35));
	townDesc.setPosition(Vector2f(594+25,366));
	townName.setProps(Color::Red,22);
	townDesc.setProps(Color::Black,16);

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
        temp.description = file.getString();
        temp.image = imagePool.loadResource(Properties::MenuImagePath+file.getString());
        temp.spId = file.get<uint16_t>();
        temp.visited = game->world.mapVisited(temp.referenceName);
        towns.push_back(temp);
    }
}

string MapState::wordWrap(string str)
{
    Text temp;
    temp.setFont(Properties::PrimaryMenuFont);
    temp.setCharacterSize(16);
    temp.setString(str);
    for (unsigned int i = 0; i<str.size(); ++i)
    {
        if (temp.findCharacterPos(i).x>=165)
        {
            for (int j = i; j>=0; j--)
            {
                if (str[j]==' ')
                {
                    str.insert(j,"\n");
                    temp.setString(str);
                    break;
                }
            }
        }
    }
    return str;
}

bool MapState::execute()
{
    const int ScreenWidth = 594;
    const int ScreenHeight = 600;

    int snapTime = gameClock.getTimeStamp();
    navPos = Vector2i(ScreenWidth/2, ScreenHeight/2);

    while (!finishFrame())
    {
        if (gameClock.getTimeStamp()-snapTime>300) {
            if (user.isInputActive(PlayerInput::Up) && navPos.y>18)
                navPos.y -= 4;
            if (user.isInputActive(PlayerInput::Right) && navPos.x<ScreenWidth-12)
                navPos.x += 4;
            if (user.isInputActive(PlayerInput::Down) && navPos.y<ScreenHeight-26)
                navPos.y += 4;
            if (user.isInputActive(PlayerInput::Left) && navPos.x>18)
                navPos.x -= 4;
        }

        if (user.isInputActive(PlayerInput::Interact))
        {
            if (navPos.x>=ScreenWidth-64 && navPos.y>=ScreenHeight-64)
                return false;
            for (unsigned int i = 0; i<towns.size(); ++i)
            {
                IntRect townBox(towns[i].pos.x-12, towns[i].pos.y-12, 24, 24);
                if (townBox.contains(navPos))
                {
                    if (towns[i].visited && canFly)
                    {
                        game->data.loadMapFlag = true;
                        game->data.nextMapName = towns[i].mapName;
                        game->data.nextSpawnId = towns[i].spId;
                        game->data.pauseGameFlag = false;
                        return false;
                    }
                }
            }
        }

        if (user.isInputActive(PlayerInput::Run))
			return false;

		townName.setText("");
        townImage.setPosition(1000,1000);
        townDesc.setText("");
		for (unsigned int i = 0; i<towns.size(); ++i)
		{
		    IntRect townBox(towns[i].pos.x-12, towns[i].pos.y-12, 24, 24);
            if (townBox.contains(navPos))
			{
			    if (gameClock.getTimeStamp()-snapTime>750) {
                    navPos.x = towns[i].pos.x;
                    navPos.y = towns[i].pos.y;
                    snapTime = gameClock.getTimeStamp();
			    }

				townName.setText(towns[i].displayName);
				if (towns[i].visited)
				{
					townImage.setTexture(*towns[i].image);
					townImage.setPosition(594+25,157);
					townDesc.setText(wordWrap(towns[i].description));
				}
			}
		}

        crossHair.setPosition(navPos.x-crossHair.getGlobalBounds().width/2,navPos.y-crossHair.getGlobalBounds().height/2);
        game->mainWindow.clear();
        game->mainWindow.draw(background);
        game->mainWindow.draw(sideBox);
        townName.draw(&game->mainWindow);
        game->mainWindow.draw(townImage);
        townDesc.draw(&game->mainWindow);
        game->mainWindow.draw(player);
        game->mainWindow.draw(crossHair);
        game->mainWindow.display();
        sleep(milliseconds(10));
    }

    return true;
}
