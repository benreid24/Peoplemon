#include "World/People/Character.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
#include <cmath>
using namespace sf;
using namespace std;

Character::Character()
{
    behavior = nullptr;
    isMoving = false;
    isLocked = false;
    updatedOnce = false;
    dir = 0;
    lastTime = 0;
    controlWaitTime = 0;
}

Character::~Character()
{
    if (behavior)
        delete behavior;
}

void Character::loadAnim(Animation* a, string file)
{
    AnimationReference ref = animPool.loadResource(Properties::AnimationPath+file+"/up.anim");
    a[0].setSource(ref);
    ref = animPool.loadResource(Properties::AnimationPath+file+"/right.anim");
    a[1].setSource(ref);
    ref = animPool.loadResource(Properties::AnimationPath+file+"/down.anim");
    a[2].setSource(ref);
    ref = animPool.loadResource(Properties::AnimationPath+file+"/left.anim");
    a[3].setSource(ref);
}

void Character::spawn(Vector2f pos, int d)
{
    position = pos;
    mapPos.x = pos.x/32;
    mapPos.y = pos.y/32;
    lastPos = mapPos;
    dir = d;
}

string Character::getName()
{
    return name;
}

void Character::update(Game* game)
{
    double dif = double(gameClock.getMicros()-lastTime)*0.00008080808;
    lastTime = gameClock.getMicros();

    if (mapPos.x*32>position.x)
        position.x += dif;
    else if (mapPos.x*32<position.x)
        position.x -= dif;
    if (mapPos.y*32>position.y)
        position.y += dif;
    else if (mapPos.y*32<position.y)
        position.y -= dif;

	if (dif>32)
	{
		position.x = mapPos.x*32;
		position.y = mapPos.y*32;
	}

    if (abs(position.x-mapPos.x*32)<0.5)
        position.x = mapPos.x*32;
    if (abs(position.y-mapPos.y*32)<0.5)
        position.y = mapPos.y*32;

    if ((lastPos!=mapPos && mapPos.x*32==position.x && mapPos.y*32==position.y) || !updatedOnce)
    {
        game->world.setSpaceOccupied(lastPos,nullptr);
        game->world.setSpaceOccupied(mapPos,this);
        lastPos = mapPos;
        isMoving = false;
        updatedOnce = true;
    }

    if (mapPos.x*32==position.x && mapPos.y*32==position.y && !isLocked && queuedInput.size()==0)
        behavior->act(game);

	if (queuedInput.size()>0)
	{
		if (move(game,queuedInput.front(),false,true,false))
			queuedInput.pop();
	}
}

void Character::draw(RenderWindow* window, Vector2f camPos)
{
	camPos.y += 6;
    if (isMoving)
        walking[dir].update();
    else
        walking[dir].setFrame(0);

    walking[dir].setPosition(position-camPos);
    walking[dir].draw(window);
}

int Character::getDir()
{
    return dir;
}

Vector2i Character::getMapPos()
{
    return mapPos;
}

bool Character::move(Game* game, int d, bool ignoreCols, bool playAnims, bool queueInput, bool ignoreLock)
{
    if ((isLocked && !queueInput && !ignoreLock) || game->data.pauseGameFlag)
        return false;

    if (mapPos.x*32==position.x && mapPos.y*32==position.y && mapPos==lastPos)
    {
    	if (dir==d && gameClock.getTimeStamp() >= controlWaitTime)
        {
        	if (dir==0 && (game->world.spaceFree(mapPos+Vector2i(0,-1),mapPos) || ignoreCols))
			{
				lastPos = mapPos;
				isMoving = playAnims;
				mapPos.y--;
				game->world.setSpaceOccupied(mapPos,this);
				game->world.moveOntoTile(mapPos);
				game->world.updateObjectRenderLocation(this,lastPos.y,mapPos.y);
			}
			else if (dir==1 && (game->world.spaceFree(mapPos+Vector2i(1,0),mapPos) || ignoreCols))
			{
				lastPos = mapPos;
				isMoving = playAnims;
				mapPos.x++;
				game->world.setSpaceOccupied(mapPos,this);
				game->world.moveOntoTile(mapPos);
			}
			else if (dir==2 && (game->world.spaceFree(mapPos+Vector2i(0,1),mapPos) || ignoreCols))
			{
				lastPos = mapPos;
				isMoving = playAnims;
				mapPos.y++;
				game->world.setSpaceOccupied(mapPos,this);
				game->world.moveOntoTile(mapPos);
				game->world.updateObjectRenderLocation(this,lastPos.y,mapPos.y);
			}
			else if (dir==3 && (game->world.spaceFree(mapPos+Vector2i(-1,0),mapPos) || ignoreCols))
			{
				lastPos = mapPos;
				isMoving = playAnims;
				mapPos.x--;
				game->world.setSpaceOccupied(mapPos,this);
				game->world.moveOntoTile(mapPos);
			}
			else
			{
				isMoving = false;
				return false;
			}

			if (this==(Character*)(&game->player))
			{
				if (game->data.repelStepsLeft>0)
					game->data.repelStepsLeft--;
			}
        }
        else if (dir != d)
		{
			dir = d;
			if (dynamic_cast<Player*>(this) != nullptr && !isMoving)
                controlWaitTime = gameClock.getTimeStamp() + 120;
            isMoving = false;
		}
    }
    else if (queueInput)
		queuedInput.push(d);
	else
		return false;

    return true;
}

void Character::shift(int x, int y)
{
    mapPos.x += x;
    mapPos.y += y;
    position.x = mapPos.x*32;
    position.y = mapPos.y*32;
    isMoving = false;
}

void Character::setLock(bool l, bool r)
{
    if (r)
        prevLockStates.push(isLocked);
	else
	{
		while (prevLockStates.size()>0)
			prevLockStates.pop();
	}
    isLocked = l;
    if (l)
		forceStop();
}

void Character::resetLock()
{
	if (prevLockStates.size()>0)
	{
		isLocked = prevLockStates.top();
		prevLockStates.pop();
	}
	else
		isLocked = false;
}

void Character::resetCollisions(Game* g)
{
	g->world.setSpaceOccupied(mapPos,nullptr);
	g->world.setSpaceOccupied(lastPos,nullptr);
}

void Character::forceStop()
{
	isMoving = false;
	for (int i = 0; i<4; ++i)
		walking[i].setFrame(0);
}

void Character::resetMotionTimer()
{
	lastTime = gameClock.getMicros();
}

void Character::clearQueue()
{
	while (!queuedInput.empty())
		queuedInput.pop();
}

string Character::getIdentifier()
{
    return "Character Base!!";
}
