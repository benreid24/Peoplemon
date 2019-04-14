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
        temp.spId = file.get<uint16_t>();
        temp.visited = game->world.mapVisited(temp.referenceName);
        towns.push_back(temp);
    }
    temp.pos.x = temp.pos.y = 12;
    temp.displayName = "Hidden Tribe";
    temp.mapName = "HomeTownDemo";
    temp.referenceName = "Home Town";
    temp.description = "A random tribe that makes bagels";
    temp.image = imagePool.loadResource(Properties::MenuImagePath+"tribe.png");
    temp.spId = 10;
    temp.visited = true;
    towns.push_back(temp);
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
    navPos = Vector2i(ScreenWidth/2, ScreenHeight/2);

    while (!finishFrame())
    {
        if (user.isInputActive(PlayerInput::Up) && navPos.y>2)
            navPos.y -= 2;
        if (user.isInputActive(PlayerInput::Right) && navPos.x<ScreenWidth-2)
            navPos.x += 2;
        if (user.isInputActive(PlayerInput::Down) && navPos.y<ScreenHeight-2)
            navPos.y += 2;
        if (user.isInputActive(PlayerInput::Left) && navPos.x>2)
            navPos.x -= 2;

        if (user.isInputActive(PlayerInput::Interact))
        {
            if (navPos.x>=ScreenWidth-32 && navPos.y>=ScreenHeight-32)
            {
                game->data.pauseGameFlag = false;
                return false;
            }
            for (unsigned int i = 0; i<towns.size(); ++i)
            {
                //TODO - snap?
                IntRect townBox(towns[i].pos.x-16, towns[i].pos.y-16, 32, 32);
                if (townBox.contains(navPos))
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

		bool townFound = false;
		for (unsigned int i = 0; i<towns.size(); ++i)
		{
            if (towns[i].pos.x==navPos.x && towns[i].pos.y==navPos.y)
			{
				townFound = true;
				townName.setText(towns[i].displayName);
				if (towns[i].visited)
				{
					townImage.setTexture(*towns[i].image);
					townImage.setPosition(594+25,157);
					townDesc.setText(wordWrap(towns[i].description));
				}
				else
				{
					townName.setText("");
					townImage.setPosition(1000,1000);
					townDesc.setText("");
				}
			}
		}
		if (!townFound)
		{
			townName.setText("");
			townImage.setPosition(1000,1000);
			townDesc.setText("");
		}

        crossHair.setPosition(navPos.x+16,navPos.y+16);
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
