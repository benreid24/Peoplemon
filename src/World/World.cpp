#include "World/World.hpp"
#include "Game/Game.hpp"
#include "Util/File.hpp"
#include "World/People/Trainer.hpp"
#include "World/People/Npc.hpp"
#include "World/Item.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Game/BattleState.hpp"
#include "Battle/AI/RandomBattler.hpp"
using namespace sf;
using namespace std;

World::World(Game* g) : light(TrianglesFan, 362), weather(g)
{
    game = g;
    lightTxtr.create(800,600);
    lightSpr.setTexture(lightTxtr.getTexture());
    pcMap = "Ghettopolis/Worldmap";
    pcSpawn = 10; //TODO - init these to actual default
}

World::~World()
{
    clear();
}

string World::getName()
{
	return name;
}

Vector2i World::getSize()
{
	return size;
}

void World::load(string file, int spId, bool trans)
{
	stopAnimations();
	if (trans)
	{
		RectangleShape cover;
		cover.setSize(Vector2f(800,600));
		double a = 0;
		int lTime = gameClock.getTimeStamp();
		while (a!=255)
		{
			a += double(gameClock.getTimeStamp()-lTime)*0.181818;
			lTime = gameClock.getTimeStamp();
			if (a>255)
				a = 255;
            cover.setFillColor(Color(0,0,0,a));
            draw(&game->mainWindow);
            game->mainWindow.draw(cover);
            game->mainWindow.display();
            sleep(milliseconds(2));
		}
	}

	if (name.size()>0 && unloadScript)
		unloadScript->run(&game->scriptEnvironment);
    clear();
    if (file=="LastMap")
		file = lastMap;
	lastMap = curMap;
    curMap = file;
    if (spId!=0 && spId!=-1)
    {
        lastPos = game->player.getPosition();
        lastDir = game->player.getDir()+2;
        if (lastDir>3)
            lastDir -= 4;
    }

    cout << "Loading map: " << file << "(" << spId << ")\n";

    File input(Properties::MapPath+file+".map");
    int tInt;
    string loadScript;

    name = input.getString();
    file = input.getString();
    tileset.load(file);
    file = input.getString();
    game->music.load(file);
    addVisitedMap(name);

    if (name=="Peoplemon Center")
    {
        pcMap = file;
        pcSpawn = spId;
    }

    loadScript = input.getString();
    file = input.getString();
	if (File::getExtension(file)=="psc")
		unloadScript.reset(new Script(Properties::ScriptPath+file));
	else
		unloadScript.reset(new Script(file));

    size.x = input.get<uint32_t>();
    size.y = input.get<uint32_t>();
    tInt = input.get<uint16_t>();
    layers.resize(tInt);
    collisions.setSize(size.x,size.y);
    catchables.setSize(size.x,size.y);
    for (int i = 0; i<tInt; ++i)
    {
        layers[i].setSize(size.x,size.y);
    }

    firstYSortLayer = input.get<uint16_t>();
    firstTopLayer = input.get<uint16_t>();
    tInt = input.get<uint8_t>();
    weather.init(Weather::Type(tInt));
    ambientLightOverride = input.get<uint16_t>();

    ySortedObjects.resize(size.y+1);
    ySortedTiles.resize(firstTopLayer-firstYSortLayer);
    for (unsigned int i = 0; i<ySortedTiles.size(); ++i)
    {
        ySortedTiles[i].setSize(size.x,size.y);
    }

    tInt = input.get<uint16_t>();
    PeoplemonZone tempZone;
    for (int i = 0; i<tInt; ++i)
    {
    	tempZone.area.left = input.get<uint32_t>();
    	tempZone.area.top = input.get<uint32_t>();
    	tempZone.area.width = input.get<uint32_t>();
    	tempZone.area.height = input.get<uint32_t>();
    	int s = input.get<uint16_t>();
    	for (int j = 0; j<s; ++j)
        {
        	file = input.getString();
			WildPeoplemon temp;
			temp.load(Properties::WildPeoplemonPath+"/"+file);
			tempZone.peoplemon.push_back(temp);
        }
        zones.push_back(tempZone);
    }

    for (int x = 0; x<size.x; ++x)
    {
        for (int y = 0; y<size.y; ++y)
        {
            collisions(x,y) = input.get<uint8_t>();
        }
    }
    for (int x = 0; x<size.x; ++x)
    {
        for (int y = 0; y<size.y; ++y)
        {
            catchables(x,y) = input.get<uint8_t>();
        }
    }

	map<int,Animation*> tempAnimTable;
	anims.reserve(tileset.numAnims()+1);
    for (unsigned int i = 0; i<layers.size(); ++i)
    {
        for (int x = 0; x<size.x; ++x)
        {
            for (int y = 0; y<size.y; ++y)
            {
                layers[i](x,y).isAnim = bool(input.get<uint8_t>());
                int id = input.get<uint16_t>();
                layers[i](x,y).nonZero = id!=0;
                layers[i](x,y).anim = NULL;
                if (layers[i](x,y).isAnim)
				{
					if (tileset.getAnim(id))
					{
						layers[i](x,y).delA = true;
						if (tileset.getAnim(id)->isLooping()) //Animation object is shared between all (sync's water, etc)
						{
							if (tempAnimTable.find(id)==tempAnimTable.end())
							{
								anims.push_back(Animation(tileset.getAnim(id)));
								tempAnimTable[id] = &anims[anims.size()-1];
							}
							layers[i](x,y).anim = tempAnimTable[id];
							layers[i](x,y).delA = false;
						}
						else //Animation object is unique (so all grass doesn't move at once, etc)
							layers[i](x,y).anim = new Animation(tileset.getAnim(id));
					}
					else
						layers[i](x,y).nonZero = false;
				}
                else if (id!=0)
				{
					TextureReference t = tileset.getTile(id);
					if (t)
						layers[i](x,y).spr.setTexture(*tileset.getTile(id));
				}

                if (i>=unsigned(firstYSortLayer) && i<unsigned(firstTopLayer))
                {
                    if (layers[i](x,y).nonZero)
                    {
                        int eY = y+layers[i](x,y).spr.getGlobalBounds().height/64+1;
                        if (eY>=size.y)
                            eY = size.y-1;
                        ySortedTiles[i-firstYSortLayer](x,eY) = make_pair(y,&layers[i](x,y));
                    }
                }
            }
        }
    }

    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i)
    {
        Vector2f pos;
        int d, id;
        id = input.get<uint16_t>();
        pos.x = input.get<uint32_t>();
        pos.y = input.get<uint32_t>();
        pos.x += 32;
        pos.y += 32;
        d = input.get<uint8_t>();
        if (id==spId)
            game->player.spawn(pos,d);
    }
    if (spId==0)
	{
		game->player.spawn(lastPos,lastDir);
		game->player.shift(0,1); //to move down past the door. if spawning in walls, check this line
	}
    setRenderPosition(game->player.getPosition());

    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i)
    {
        Character* c;
        Vector2f pos;
        int dir;
        file = input.getString();
        pos.x = input.get<uint32_t>();
        pos.y = input.get<uint32_t>();
        dir = input.get<uint8_t>();
        if (File::getExtension(file)=="tnr")
        {
            c = new Trainer(game,Properties::TrainerPath+file, false);
            for (unsigned int j = 0; j<beatenTrainers.size(); ++j)
            {
                if (c->getName()==beatenTrainers[j])
					dynamic_cast<Trainer*>(c)->setBeaten();
            }
        }
        else
        {
            c = new Npc(Properties::NpcPath+file,false);
            for (unsigned int i = 0; i<talkedToNpcs.size(); ++i)
            {
                if (c->getName()==talkedToNpcs[i])
					dynamic_cast<Npc*>(c)->setTalked();
            }
        }
        c->spawn(pos+Vector2f(32,32),dir);
        objects.push_back(c);
    }

    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i)
    {
        int itemId = input.get<uint16_t>();
        int mapId = input.get<uint16_t>();
        Vector2f pos;
        pos.x = input.get<uint32_t>();
        pos.y = input.get<uint32_t>();
        for (unsigned int j = 0; j<pickedUpItems[name].size(); ++j)
        {
            if (pickedUpItems[name][j]==mapId)
                itemId = -1;
        }
        if (itemId!=-1)
            objects.push_back(new Item(this, itemId, mapId, pos));
    }

    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i)
    {
        Light temp;
        temp.position.x = input.get<uint32_t>();
        temp.position.y = input.get<uint32_t>();
        temp.radius = input.get<uint16_t>();
        lights.push_back(temp);
    }

    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i)
    {
        MapEvent evt;
        file = input.getString();
        evt.position.x = input.get<uint32_t>();
        evt.position.y = input.get<uint32_t>();
        evt.size.x = input.get<uint16_t>();
        evt.size.y = input.get<uint16_t>();
        evt.maxRuns = input.get<uint8_t>();
        evt.trigger = input.get<uint8_t>();
        evt.runs = 0;
        evt.script.reset(new Script());
        if (File::getExtension(file)=="psc")
            evt.script->load(Properties::ScriptPath+file);
        else
            evt.script->load(file);

        if (evt.trigger==0)
            game->scriptEnvironment.runScript(evt.script);

        events.push_back(evt);
    }

    objects.push_back(&game->player);
    for (unsigned int i = 0; i<objects.size(); ++i)
        ySortedObjects[objects[i]->getPosition().y/32].push_back(objects[i]);

    game->music.play();

	calculateLighting();
	RectangleShape cover;
	cover.setSize(Vector2f(800,600));
	double a = 255;
	int lTime = gameClock.getTimeStamp();
	game->player.forceStop();
	while (a!=0)
	{
		calculateLighting();
		a -= double(gameClock.getTimeStamp()-lTime)*0.181818;
		lTime = gameClock.getTimeStamp();
		if (a<0)
			a = 0;
		cover.setFillColor(Color(0,0,0,a));
		draw(&game->mainWindow);
		game->mainWindow.draw(cover);
		game->mainWindow.display();
		sleep(milliseconds(2));
	}
	if (File::getExtension(loadScript)=="psc")
		loadScript = Properties::ScriptPath+loadScript;
    game->scriptEnvironment.runScript(shared_ptr<Script>(new Script(loadScript)));
}

void World::clear()
{
	for (unsigned int i = 0; i<layers.size(); ++i)
	{
		for (int x = 0; x<size.x; ++x)
		{
			for (int y = 0; y<size.y; ++y)
			{
				if (layers[i](x,y).isAnim && layers[i](x,y).delA)
					delete layers[i](x,y).anim;
			}
		}
	}

    layers.clear();
	anims.clear();
    collisions.clear();
    catchables.clear();
    for (unsigned int i = 0; i<objects.size(); ++i)
    {
        if (objects[i]!=&game->player)
            delete objects[i];
    }
    objects.clear();
    ySortedObjects.clear();
    ySortedTiles.clear();
    lights.clear();
    game->scriptEnvironment.stopAll();
    events.clear();
    zones.clear();
}

void World::addVisitedMap(string m)
{
    for (unsigned int i = 0; i<visitedMaps.size(); ++i)
    {
        if (visitedMaps[i]==m)
            return;
    }
    visitedMaps.push_back(m);
}

void World::update()
{
	FloatRect bounds(game->player.getPosition()-Vector2f(2400, 1800), Vector2f(4800, 3600));
	for (int i = signed(objects.size())-1; i>=0 && objects.size()>0; i--)
	{
		if (bounds.contains(objects[i]->getPosition()))
			objects[i]->update(game);

		for (unsigned int t = 0; t<objDelQueue.size(); t++)
		{
			if (objects[i]==objDelQueue[t])
			{
				Character* c = dynamic_cast<Character*>(objects[i]);
				if (c)
					c->resetCollisions(game);

				delete objects[i];
				objects.erase(objects.begin()+i);
				i--;
				break;
			}
		}
	}
    weather.update();

    for (unsigned int i = 0; i<objDelQueue.size(); ++i)
    {
        for (unsigned int j = 0; j<ySortedObjects.size(); ++j)
        {
            for (unsigned int k = 0; k<ySortedObjects[j].size(); ++k)
			{
                if (ySortedObjects[j][k]==objDelQueue[i])
				{
                    ySortedObjects[j].erase(ySortedObjects[j].begin()+k);
                    break;
				}
			}
        }
    }
    objDelQueue.clear();
    for (unsigned int i = 0; i<objAddQueue.size(); ++i)
    {
        objects.push_back(objAddQueue[i]);
        ySortedObjects[objAddQueue[i]->getPosition().y/32].push_back(objAddQueue[i]);
    }
    objAddQueue.clear();

    calculateLighting();

    for (unsigned int i = 0; i<anims.size(); ++i)
	{
		anims[i].update();
	}
}

void World::calculateLighting()
{
	if (ambientLightOverride!=255)
        currentLighting = ambientLightOverride;
    else
    {
        ClockTime t = gameClock.getClockTime();
        const int x = t.hour*60+t.minute;
        const double n = 0.7;
        currentLighting = (70*cos(3.1415926*x/720)+70)*((1-n)*(720-x)*(720-x)/518400+n) + weather.getLightModifier();
        if (currentLighting<0)
            currentLighting = 0;
    }
}

bool World::mapVisited(string m)
{
    for (unsigned int i = 0; i<visitedMaps.size(); ++i)
    {
        if (visitedMaps[i]==m)
            return true;
    }
    return false;
}

void World::draw(sf::RenderWindow* window)
{
    for (int i = 0; i<firstYSortLayer; ++i)
    {
        for (int x = camPosTiles.x-10; x<camPosTiles.x+Properties::TilesWide+10; ++x)
        {
            if (x>=0 && x<size.x)
            for (int y = camPosTiles.y-10; y<camPosTiles.y+Properties::TilesTall+10; ++y)
            {
                if (y>=0 && y<size.y)
                {
                    if (layers[i](x,y).isAnim && layers[i](x,y).nonZero)
                    {
                        layers[i](x,y).anim->setPosition(Vector2f(x*32-camPos.x,y*32-camPos.y));
                        layers[i](x,y).anim->draw(window);
                    }
                    else if (layers[i](x,y).nonZero)
                    {
                        layers[i](x,y).spr.setPosition(x*32-camPos.x+32,y*32-camPos.y+32);
                        window->draw(layers[i](x,y).spr);
                    }
                }
            }
        }
    }

    for (int y = camPosTiles.y-10; y<camPosTiles.y+Properties::TilesTall+10; ++y)
    {
        if (y>=0 && y<size.y)
        {
            for (int i = 0; i<firstTopLayer-firstYSortLayer; ++i)
            {
                for (int x = camPosTiles.x-10; x<camPosTiles.x+Properties::TilesWide+10; ++x)
                {
                    if (x>=0 && x<size.x && ySortedTiles[i](x,y).second)
                    {
                        if (ySortedTiles[i](x,y).second->isAnim && layers[i](x,y).nonZero)
                        {
                            ySortedTiles[i](x,y).second->anim->setPosition(Vector2f(x*32-camPos.x,ySortedTiles[i](x,y).first*32-camPos.y));
                            ySortedTiles[i](x,y).second->anim->draw(window);
                        }
                        else if (ySortedTiles[i](x,y).second->nonZero)
                        {
                            ySortedTiles[i](x,y).second->spr.setPosition(x*32-camPos.x+32,ySortedTiles[i](x,y).first*32-camPos.y+32);
                            window->draw(ySortedTiles[i](x,y).second->spr);
                        }
                    }
                }
            }
            for (unsigned int i = 0; i<ySortedObjects.at(y+1).size(); ++i)
            {
                ySortedObjects.at(y+1).at(i)->draw(window,camPos);
            }
        }
    }

    weather.draw(&game->mainWindow);
    if (currentLighting>40)
    {
        IntRect t(camPos.x-400, camPos.y-300,1600,1200);
        lightTxtr.clear(Color(0,0,0,currentLighting));
        for (unsigned int i = 0; i<lights.size(); ++i)
        {
            if (t.contains(Vector2i(lights[i].position)))
            {
                light[0].position = lights[i].position - camPos;
                light[0].position.y = 600-light[0].position.y;
                light[0].color = Color::Transparent;
                for (unsigned int j = 1; j<362; ++j)
                {
                    light[j].position = lights[i].position + Vector2f(lights[i].radius*cos(double(j)/180*3.1415926)-camPos.x+32,lights[i].radius*sin(double(j)/180*3.1415926)-camPos.y+32);
                    light[j].color = Color(0,0,0,currentLighting);
                    light[j].position.y = 600-light[j].position.y;
                }
                lightTxtr.draw(light, BlendNone);
            }
        }
        window->draw(lightSpr);
    }

    for (unsigned int i = firstTopLayer; i<layers.size(); ++i)
    {
        for (int x = camPosTiles.x-10; x<camPosTiles.x+Properties::TilesWide+10; ++x)
        {
            if (x>=0 && x<size.x)
            for (int y = camPosTiles.y-10; y<camPosTiles.y+Properties::TilesTall+10; ++y)
            {
                if (y>=0 && y<size.y)
                {
                    if (layers[i](x,y).isAnim && layers[i](x,y).nonZero)
                    {
                        layers[i](x,y).anim->setPosition(Vector2f(x*32-camPos.x,y*32-camPos.y));
                        layers[i](x,y).anim->draw(window);
                    }
                    else if (layers[i](x,y).nonZero)
                    {
                        layers[i](x,y).spr.setPosition(x*32-camPos.x+32,y*32-camPos.y+32);
                        window->draw(layers[i](x,y).spr);
                    }
                }
            }
        }
    }
}

void World::setRenderPosition(Vector2f playerPos)
{
    camPos = playerPos - Vector2f(Properties::ScreenWidth/2,Properties::ScreenHeight/2);
    if (camPos.x<32)
        camPos.x = 32;
    else if (camPos.x>size.x*32-Properties::ScreenWidth)
        camPos.x = size.x*32-Properties::ScreenWidth+32;
    if (camPos.y<32)
        camPos.y = 32;
    else if (camPos.y>size.y*32-Properties::ScreenHeight)
        camPos.y = size.y*32-Properties::ScreenHeight+32;

    if (Properties::ScreenWidth>=size.x*32)
        camPos.x = size.x*16-Properties::ScreenWidth/2+32;
    if (Properties::ScreenHeight>=size.y*32)
        camPos.y = size.y*16-Properties::ScreenHeight/2+32;

    camPosTiles.x = camPos.x/32;
    camPosTiles.y = camPos.y/32;
}

void World::moveOntoTile(Vector2i playerPos, Vector2i lastPos)
{
    moveOntoTile(playerPos);
    playerPos.x--;
    playerPos.y--;
    lastPos.x--;
    lastPos.y--;

    for (unsigned int i = 0; i<events.size(); ++i)
    {
    	int minX = events[i].position.x/32;
    	int minY = events[i].position.y/32;
    	int maxX = minX+events[i].size.x;
    	int maxY = minY+events[i].size.y;
        bool inNow = playerPos.x>=minX && playerPos.x<maxX && playerPos.y>=minY && playerPos.y<maxY;
        bool wasIn = lastPos.x>=minX && lastPos.x<maxX && lastPos.y>=minY && lastPos.y<maxY;
        if ((events[i].trigger==1 && inNow && !wasIn) || (events[i].trigger==2 && !inNow && wasIn) || (events[i].trigger==3 && inNow!=wasIn) || (events[i].trigger==4 && inNow))
        {
            if (events[i].runs<events[i].maxRuns || events[i].maxRuns==0)
                game->scriptEnvironment.runScript(events[i].script);
            events[i].runs++;
        }
    }

    if (catchables(playerPos.x,playerPos.y)!=0 && playerPos!=lastPos)
    {
        if (Random(0,100)<15*game->player.getRandomMultiplier())
        {
            vector<PeoplemonRef> ppl = getWildPeoplemon();
            if (ppl.size()>0)
			{
				game->runStateUnderPriveldged((new BattleState(game, new RandomBattler(&ppl,vector<int>()), "WILD "+ppl[0].name,"",0,true)));
				game->music.previous();
			}
            else
				cout << "Catch tile at location (" << playerPos.x << ", " << playerPos.y << ") but no zone is present there!\n";
        }
    }
}

void World::moveOntoTile(Vector2i pos)
{
	if (pos.x-1<size.x && pos.y-1<size.y && pos.x-1>=0 && pos.y-1>=0)
    {
    	for (unsigned int i = 0; i<layers.size(); ++i)
		{
			if (layers[i](pos.x-1,pos.y-1).isAnim && layers[i](pos.x-1,pos.y-1).nonZero)
			{
				if (layers[i](pos.x-1,pos.y-1).anim)
				{
					if (!layers[i](pos.x-1,pos.y-1).anim->isLooping())
						layers[i](pos.x-1,pos.y-1).anim->play();
				}
			}
		}
    }
}

Vector2f World::getCamera()
{
    return camPos;
}

bool World::spaceFree(Vector2i pos)
{
	if (pos.x<=0 || pos.x>size.x || pos.y<=0 || pos.y>size.y)
		return false;
    return collisions(pos.x-1,pos.y-1)==1;
}

void World::setSpaceOccupied(Vector2i pos, bool o)
{
    collisions(pos.x-1,pos.y-1) = int(!o);
}

void World::removeObject(Object* o)
{
    if (o!=&game->player)
        objDelQueue.push_back(o);
}

void World::addObject(Object* o)
{
    objAddQueue.push_back(o);
}

void World::setItemPickedUp(int id)
{
    pickedUpItems[name].push_back(id);
}

void World::setTrainerBeaten(string nm)
{
	for (unsigned int i = 0; i<beatenTrainers.size(); ++i)
	{
		if (beatenTrainers[i]==nm)
			return;
	}
    beatenTrainers.push_back(nm);
}

void World::setNpcTalkedTo(string nm)
{
	for (unsigned int i = 0; i<talkedToNpcs.size(); ++i)
	{
		if (talkedToNpcs[i]==nm)
			return;
	}
	talkedToNpcs.push_back(nm);
}

Trainer* World::getTrainer(string nm)
{
    for (unsigned int i = 0; i<objects.size(); ++i)
    {
        if (Trainer* t = dynamic_cast<Trainer*>(objects[i]))
        {
            if (t->getName()==nm)
                return t;
        }
    }
    return NULL;
}

Npc* World::getNPC(string nm)
{
    for (unsigned int i = 0; i<objects.size(); ++i)
    {
        if (Npc* t = dynamic_cast<Npc*>(objects[i]))
        {
            if (t->getName()==nm)
                return t;
        }
    }
    return NULL;
}

void World::addLight(int x, int y, int r)
{
    Light t;
    t.position.x = x;
    t.position.y = y;
    t.radius = r;
    lights.push_back(t);
}

void World::removeLight(int x, int y)
{
    for (unsigned int i = 0; i<lights.size(); ++i)
    {
        if (abs(lights[i].position.x-x)<20 && abs(lights[i].position.y-y)<20)
        {
            lights.erase(lights.begin()+i);
            break;
        }
    }
}

void World::removeAllLights()
{
    lights.clear();
}

void World::setLightingOverride(int o)
{
    ambientLightOverride = o;
}

void World::setWeather(int t)
{
    weather.init(Weather::Type(t));
}

Object* World::getFirstObject(Vector2i pos, int dir, int range)
{
    Vector2i cur = pos, chg;
    if (dir==0)
        chg.y = -1;
    else if (dir==1)
        chg.x = 1;
    else if (dir==2)
        chg.y = 1;
    else
        chg.x = -1;
    cur += chg;

    for (int i = 0; i<=range; ++i)
    {
        for (unsigned int j = 0; j<objects.size(); ++j)
        {
        	if (int(objects[j]->getPosition().x/32+0.01)==cur.x && int(objects[j]->getPosition().y/32+0.01)==cur.y)
                return objects[j];
        }
        if (collisions(cur.x,cur.y)==1)
            return NULL;
        cur += chg;
    }

    return NULL;
}

void World::updateObjectRenderLocation(Object* o, int oldY, int newY)
{
    for (unsigned int i = 0; i<ySortedObjects[oldY].size(); ++i)
    {
        if (ySortedObjects[oldY][i]==o)
        {
            ySortedObjects[oldY].erase(ySortedObjects[oldY].begin()+i);
            break;
        }
    }
    ySortedObjects[newY].push_back(o);
}

void World::saveGame(File* file)
{
    file->writeString(curMap);
    file->writeString(lastMap);
    file->write<uint32_t>(lastPos.x);
    file->write<uint32_t>(lastPos.y);
    file->write<uint8_t>(lastDir);
    file->writeString(pcMap);
    file->write<uint16_t>(pcSpawn);
    file->write<uint16_t>(visitedMaps.size());
    for (unsigned int i = 0; i<visitedMaps.size(); ++i)
        file->writeString(visitedMaps[i]);
    file->write<uint16_t>(talkedToNpcs.size());
    for (auto i = talkedToNpcs.begin(); i!=talkedToNpcs.end(); ++i)
    {
        file->writeString(*i);
    }
    file->write<uint16_t>(beatenTrainers.size());
    for (auto i = beatenTrainers.begin(); i!=beatenTrainers.end(); ++i)
    {
        file->writeString(*i);
    }
    file->write<uint16_t>(pickedUpItems.size());
    for (auto i = pickedUpItems.begin(); i!=pickedUpItems.end(); ++i)
    {
        file->writeString(i->first);
        file->write<uint16_t>(i->second.size());
        for (unsigned int j = 0; j<i->second.size(); ++j)
            file->write<uint16_t>(i->second[j]);
    }
}

void World::loadGame(File* file)
{
    pickedUpItems.clear();
    talkedToNpcs.clear();
    beatenTrainers.clear();
    visitedMaps.clear();
    string nm, lMap;
    int sz,t;

    name = file->getString();
    lastMap = file->getString();
    lMap = lastMap;
    lastPos.x = file->get<uint32_t>();
    lastPos.y = file->get<uint32_t>();
    lastDir = file->get<uint8_t>();
    pcMap = file->getString();
    pcSpawn = file->get<uint16_t>();
    sz = file->get<uint16_t>();
    for (int i = 0; i<sz; ++i)
        visitedMaps.push_back(file->getString());
	sz = file->get<uint16_t>();
	for (int i = 0; i<sz; ++i)
        talkedToNpcs.push_back(file->getString());
	sz = file->get<uint16_t>();
	for (int i = 0; i<sz; ++i)
		beatenTrainers.push_back(file->getString());
    sz = file->get<uint16_t>();
    for (int i = 0; i<sz; ++i)
    {
        nm = file->getString();
        t = file->get<uint16_t>();
        for (int j = 0; j<t; ++j)
            pickedUpItems[nm].push_back(file->get<uint16_t>());
    }

    load(name, -1);
    lastMap = lMap;
}

void World::lockAllPeople()
{
    for (unsigned int i = 0; i<objects.size(); ++i)
    {
        if (Character* c = dynamic_cast<Character*>(objects[i]))
            c->setLock(true,true);
    }
}

void World::unlockAllPeople()
{
    for (unsigned int i = 0; i<objects.size(); ++i)
    {
        if (Character* c = dynamic_cast<Character*>(objects[i]))
            c->resetLock();
    }
}

vector<PeoplemonRef> World::getWildPeoplemon()
{
    vector<int> ranges;
    vector<WildPeoplemon> ppl;
    Vector2i pos = game->player.getMapPos();
    int max = 0;
    int reg = -1;

    for (unsigned int i = 0; i<zones.size(); ++i)
	{
		if (zones[i].area.contains(pos))
		{
			reg = i;
			break;
		}
	}
	if (reg==-1)
		return vector<PeoplemonRef>();
	if (zones[reg].peoplemon.size()==0)
		return vector<PeoplemonRef>();

    for (unsigned int i = 0; i<zones[reg].peoplemon.size(); ++i)
    {
		ppl.push_back(zones[reg].peoplemon[i]);
		ranges.push_back(max+zones[reg].peoplemon[i].frequency);
		max += zones[reg].peoplemon[i].frequency;
    }
    int n = Random(0,max);
    for (unsigned int i = 0; i<ranges.size(); ++i)
    {
        if (n<ranges[i])
        {
            vector<PeoplemonRef> ret;
            ret.push_back(ppl[i].getPeoplemon(game));
            return ret;
        }
    }
    vector<PeoplemonRef> ret;
    ret.push_back(zones[reg].peoplemon[0].getPeoplemon(game));
    return ret;
}

void World::editTile(int x, int y, int layer, int nId)
{
	layers[layer](x,y).isAnim = false;
	if (tileset.getTile(nId))
		layers[layer](x,y).spr.setTexture(*tileset.getTile(nId));
	layers[layer](x,y).nonZero = nId!=0;
	if (layers[layer](x,y).delA)
	{
		delete layers[layer](x,y).anim;
		layers[layer](x,y).delA = false;
		layers[layer](x,y).anim = NULL;
	}
}

void World::whiteout()
{
	sleep(milliseconds(2000));
    load(pcMap,pcSpawn,false);
    game->player.whiteout();
}

bool World::checkTalkedTo(string nm)
{
	if (find(talkedToNpcs.begin(),talkedToNpcs.end(),nm)!=talkedToNpcs.end() || find(beatenTrainers.begin(),beatenTrainers.end(),nm)!=beatenTrainers.end())
		return true;
	return false;
}

void World::stopAnimations()
{
	for (unsigned int i = 0; i<objects.size(); ++i)
	{
		Character* c = dynamic_cast<Character*>(objects[i]);
        if (c)
			c->forceStop();
	}
}
