#include "World/People/Player.hpp"
#include "Game/PausedState.hpp"
#include "Util/File.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

Player::Player(Game* g) : Character()
{
    game = g;
    name = "Ben";
    isBoy = true;
    isRunning = false;
    loadAnim(walking, "BoyPlayer/Walk");
    loadAnim(running, "BoyPlayer/Run");
    for (int i = 0; i<6; ++i)
        items.push_back(5);
}

Player::~Player()
{
    //dtor
}

void Player::newGame(bool b, string n)
{
    isBoy = b;
    name = n;
    if (isBoy)
    {
        loadAnim(walking, "BoyPlayer/Walk");
        loadAnim(running, "BoyPlayer/Run");
    }
    else
    {
        loadAnim(walking, "GirlPlayer/Walk");
        loadAnim(running, "GirlPlayer/Run");
    }
    items.clear();
    curPeoplemon.clear();
    storedPeoplemon.clear();
    money = 500;

    //TODO - remove after demo
    PeoplemonRef temp;
    temp.load(game,Properties::OwnedPeoplemonPath+"Vince.ppl");
    temp.curHp = temp.stats.hp;
    curPeoplemon.push_back(temp);
    temp.load(game,Properties::OwnedPeoplemonPath+"kyler2.ppl");
    temp.curHp = temp.stats.hp;
    storedPeoplemon.push_back(StoredPeoplemon(0,Vector2i(0,0),temp));
}

void Player::save(File* saveFile)
{
    saveFile->writeString(name);
    saveFile->write<uint8_t>(isBoy);
    saveFile->write<uint32_t>(mapPos.x);
    saveFile->write<uint32_t>(mapPos.y);
    saveFile->write<uint8_t>(dir);
    saveFile->write<uint32_t>(money);
    saveFile->write<uint16_t>(items.size());
    for (unsigned int i = 0; i<items.size(); ++i)
    {
        saveFile->write<uint16_t>(items[i]);
    }
    saveFile->write<uint16_t>(curPeoplemon.size()+storedPeoplemon.size());
    for (unsigned int i = 0; i<curPeoplemon.size(); ++i)
    {
        saveFile->write<uint8_t>(1);
        curPeoplemon[i].save(saveFile);
    }
    for (unsigned int i = 0; i<storedPeoplemon.size(); ++i)
    {
        saveFile->write<uint8_t>(0);
        saveFile->write<uint16_t>(storedPeoplemon[i].position.x);
        saveFile->write<uint16_t>(storedPeoplemon[i].position.y);
        saveFile->write<uint8_t>(storedPeoplemon[i].boxId);
        storedPeoplemon[i].data.save(saveFile);
    }
}

void Player::load(File* saveFile)
{
    items.clear();
    curPeoplemon.clear();
    storedPeoplemon.clear();

    name = saveFile->getString();
    isBoy = saveFile->get<uint8_t>();
    mapPos.x = saveFile->get<uint32_t>();
    mapPos.y = saveFile->get<uint32_t>();
    dir = saveFile->get<uint8_t>();
    money = saveFile->get<uint32_t>();
    int sz = saveFile->get<uint16_t>();
    for (int i = 0; i<sz; ++i)
        items.push_back(saveFile->get<uint16_t>());
    sz = saveFile->get<uint16_t>();
    for (int i = 0; i<sz; ++i)
    {
        PeoplemonRef t;
        bool s = saveFile->get<uint8_t>();

        if (s)
        {
            t.load(game, saveFile);
            curPeoplemon.push_back(t);
        }
        else
        {
            int x = saveFile->get<uint16_t>();
            int y = saveFile->get<uint16_t>();
            int box = saveFile->get<uint8_t>();
            t.load(game, saveFile);
            storedPeoplemon.push_back(StoredPeoplemon(box, Vector2i(x,y),t));
        }
    }

    if (isBoy)
    {
        loadAnim(walking, "BoyPlayer/Walk");
        loadAnim(running, "BoyPlayer/Run");
    }
    else
    {
        loadAnim(walking, "GirlPlayer/Walk");
        loadAnim(running, "GirlPlayer/Run");
    }
    lastPos = mapPos;
    position.x = mapPos.x*32;
    position.y = mapPos.y*32;
}

void Player::interact(Game* game)
{
    Object* o = game->world.getFirstObject(mapPos,dir,1);
    if (o)
    {
        cout << "Player interact object: " << o->getIdentifier() << endl;
        o->interact(game);
    }
}

void Player::update(Game* game)
{
    double dif = double(gameClock.getMicros()-lastTime)*0.00006060606;
    lastTime = gameClock.getMicros();
    if (isRunning)
		dif *= 2;

    if (mapPos.x*32>position.x)
    {
        position.x += dif;
        game->world.setRenderPosition(position);
        if (mapPos.x*32-position.x<dif)
			position.x = mapPos.x*32;
    }
    else if (mapPos.x*32<position.x)
    {
        position.x -= dif;
        game->world.setRenderPosition(position);
        if (position.x-mapPos.x*32<dif)
			position.x = mapPos.x*32;
    }
    if (mapPos.y*32>position.y)
    {
        position.y += dif;
        game->world.setRenderPosition(position);
        if (mapPos.y*32-position.y<dif)
			position.y = mapPos.y*32;
    }
    else if (mapPos.y*32<position.y)
    {
        position.y -= dif;
        game->world.setRenderPosition(position);
        if (position.y-mapPos.y*32<dif)
			position.y = mapPos.y*32;
    }

    if (lastPos!=mapPos && mapPos.x*32==position.x && mapPos.y*32==position.y)
    {
        game->world.moveOntoTile(mapPos,lastPos); //for all events
        game->world.setSpaceOccupied(lastPos,false);
        lastPos = mapPos;
    }
    if (mapPos==lastPos) //for events that trigger every time you are in range
        game->world.moveOntoTile(mapPos,lastPos);

	if (isLocked && queuedInput.size()==0)
        return;

	if (queuedInput.size()>0)
	{
		if (move(game,queuedInput.front(),false,true,false))
			queuedInput.pop();
	}
    else if (mapPos.x*32==position.x && mapPos.y*32==position.y)
    {
        if (user.isInputActive(PlayerInput::Up))
            move(game,0);
        else if (user.isInputActive(PlayerInput::Right))
            move(game,1);
        else if (user.isInputActive(PlayerInput::Down))
             move(game,2);
        else if (user.isInputActive(PlayerInput::Left))
            move(game,3);
        else
        {
            isMoving = false;
            isRunning = false;
        }
        if (isMoving)
			pickupRandom();
        if (user.isInputActive(PlayerInput::Run) && isMoving)
            isRunning = true;
        else
            isRunning = false;

        if (user.isInputActive(PlayerInput::Interact))
            interact(game);
        if (user.isInputActive(PlayerInput::Pause))
            game->data.pauseGameFlag = true;
    }
}

void Player::draw(RenderWindow* window, Vector2f camPos)
{
	camPos.y += 6;
    if (isRunning)
    {
        running[dir].update();
        running[dir].setPosition(position-camPos);
        running[dir].draw(window);
    }
    else if (isMoving)
    {
        walking[dir].update();
        walking[dir].setPosition(position-camPos);
        walking[dir].draw(window);
    }
    else
    {
        walking[dir].setFrame(0);
        walking[dir].setPosition(position-camPos);
        walking[dir].draw(window);
    }
}

bool Player::isOccupied()
{
    return isLocked;
}

string Player::getGender()
{
    return (isBoy)?"Boy":"Girl";
}

int Player::hasItem(int i)
{
    int r = 0;
    for (unsigned int j = 0; j<items.size(); ++j)
    {
        if (items[j]==i)
            r++;
    }
    return r;
}

void Player::giveItem(int i)
{
    items.push_back(i);
}

void Player::takeItem(int i, int q)
{
    int t = 0;
    for (unsigned int j = 0; j<items.size() && t<q; ++j)
    {
        if (items[j]==i)
        {
            t++;
            items.erase(items.begin()+j);
            j--;
        }
    }
}

bool Player::alterMoney(int m)
{
    if (money+m>=0)
    {
        money += m;
        return true;
    }
    return false;
}

int Player::getMoney()
{
    return money;
}

vector<PeoplemonRef>* Player::getCurrentPeoplemon()
{
    return &curPeoplemon;
}

vector<StoredPeoplemon>* Player::getStoredPeoplemon()
{
    return &storedPeoplemon;
}

vector<int>* Player::getItems()
{
    return &items;
}

bool Player::canFly()
{
    return hasItem(105);
}

bool Player::pricesLowered()
{
	for (unsigned int i = 0; i<curPeoplemon.size(); ++i)
	{
		if (game->peoplemonList[curPeoplemon[i].id].specialAbilityId==Peoplemon::Negotiator)
			return true;
	}
	return false;
}

double Player::getRandomMultiplier()
{
	int intro = 0, outro = 0;
	for (unsigned int i = 0; i<curPeoplemon.size(); ++i)
	{
		if (game->peoplemonList[curPeoplemon[i].id].specialAbilityId==Peoplemon::Introverted)
			intro++;
		else if (game->peoplemonList[curPeoplemon[i].id].specialAbilityId==Peoplemon::Extroverted)
			outro++;
	}
	if (intro>outro)
		return 0.5;
	else if (outro>intro)
		return 1.5;
	return 1;
}

void Player::pickupRandom()
{
	for (unsigned int i = 0; i<curPeoplemon.size(); ++i)
	{
		if (game->peoplemonList[curPeoplemon[i].id].specialAbilityId==Peoplemon::Questionable && curPeoplemon[i].holdItem==0 && Random(0,1000)<4)
		{
			int c = Random(0,1000);

			if (c<40)
				curPeoplemon[i].holdItem = 1; //potion
			else if (c<60)
				curPeoplemon[i].holdItem = 2; //super potion;
			else if (c<70)
				curPeoplemon[i].holdItem = 3; //mega potion;
			else if (c<71)
				curPeoplemon[i].holdItem = 4; //super mega ultra potion;
			else if (c<151)
				curPeoplemon[i].holdItem = 5; //teriball;
			else if (c<191)
				curPeoplemon[i].holdItem = 6; //peopleball;
			else if (c<211)
				curPeoplemon[i].holdItem = 7; //great ball;
			else if (c<221)
				curPeoplemon[i].holdItem = 8 ;//ultra ball;
			else if (c<231)
				curPeoplemon[i].holdItem = 9; //pair of balls;
			else if (c<251)
				curPeoplemon[i].holdItem = 10; //clock ball;
			else if (c<271)
				curPeoplemon[i].holdItem = 11; //intelliball;
			else if (c<291)
				curPeoplemon[i].holdItem = 12; //gulliball;
			else if (c<311)
				curPeoplemon[i].holdItem = 13; //abuse ball;
			else if (c<331)
				curPeoplemon[i].holdItem = 14; //bully ball;
			else if (c<336)
				curPeoplemon[i].holdItem = 15; //clone ball;
			else if (c<376)
				curPeoplemon[i].holdItem = 18; //pp pack;
			else if (c<396)
				curPeoplemon[i].holdItem = 19; //pp 6 pack;
			else if (c<416)
				curPeoplemon[i].holdItem = 20; //pp super pack;
			else if (c<426)
				curPeoplemon[i].holdItem = 21; //super pp 6 pack
			else if (c<506)
				curPeoplemon[i].holdItem = 33; //shady mushroom;
			else if (c<546)
				curPeoplemon[i].holdItem = 34; //big shady mushroom;
			else if (c<566)
				curPeoplemon[i].holdItem = 35; //go away spray;
			else if (c<581)
				curPeoplemon[i].holdItem = 36; //super go away spray;
			else if (c<586)
				curPeoplemon[i].holdItem = 37; //never come back spray;
			else if (c<626)
				curPeoplemon[i].holdItem = 56; //sketchy sack;
			else if (c<646)
				curPeoplemon[i].holdItem = 57; //goldfish cracker;
			else if (c<666)
				curPeoplemon[i].holdItem = 58; //speed juice;
			else if (c<686)
				curPeoplemon[i].holdItem = 59; //power juice;
			else if (c<706)
				curPeoplemon[i].holdItem = 60; //iced tea;
			else if (c<906)
				curPeoplemon[i].holdItem = 44; //log;
			else if (c<926)
				curPeoplemon[i].holdItem = 202; //i4got;
			else if (c<946)
				curPeoplemon[i].holdItem = 212; //frost;
			else if (c<966)
				curPeoplemon[i].holdItem = 213; //skimp out;
			else
				curPeoplemon[i].holdItem = 222; //nap;
		}
	}
}

void Player::whiteout()
{
    for (unsigned int i = 0; i<curPeoplemon.size(); ++i)
    {
        curPeoplemon[i].curHp = curPeoplemon[i].stats.hp;
        for (int j = 0; j<4; ++j)
        {
            curPeoplemon[i].curAils[j] = Peoplemon::None;
        }
    }
}

string Player::getIdentifier()
{
    return "Player";
}

void Player::forceStop()
{
	isMoving = false;
	isRunning = false;
	for (int i = 0; i<4; ++i)
		walking[i].setFrame(0);
}
