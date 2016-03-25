#include "World/People/Character.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
using namespace sf;
using namespace std;

Character::Character()
{
    behavior = NULL;
    isMoving = false;
    isLocked = false;
    dir = 0;
    lastTime = 0;
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
    dir = d;
}

string Character::getName()
{
    return name;
}

void Character::update(Game* game)
{
    double dif = double(gameClock.getMicros()-lastTime)*0.00006060606;
    lastTime = gameClock.getMicros();

    if (mapPos.x*32>position.x)
        position.x += dif;
    else if (mapPos.x*32<position.x)
        position.x -= dif;
    if (mapPos.y*32>position.y)
        position.y += dif;
    else if (mapPos.y*32<position.y)
        position.y -= dif;

    if (abs(position.x-mapPos.x*32)<0.5)
        position.x = mapPos.x*32;
    if (abs(position.y-mapPos.y*32)<0.5)
        position.y = mapPos.y*32;

    if (lastPos!=mapPos && mapPos.x*32==position.x && mapPos.y*32==position.y)
    {
        game->world.setSpaceOccupied(lastPos,false);
        game->world.setSpaceOccupied(mapPos,true);
        lastPos = mapPos;
        isMoving = false;
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

bool Character::move(Game* game, int d, bool ignoreCols, bool playAnims, bool queueInput)
{
    if (isLocked && !queueInput)
        return false;

    if (mapPos.x*32==position.x && mapPos.y*32==position.y)
    {
    	if (dir==d)
        {
        	if (dir==0 && (game->world.spaceFree(mapPos+Vector2i(0,-1)) || ignoreCols))
			{
				lastPos = mapPos;
				isMoving = playAnims;
				mapPos.y--;
				game->world.setSpaceOccupied(mapPos,true);
				game->world.moveOntoTile(mapPos);
				game->world.updateObjectRenderLocation(this,lastPos.y,mapPos.y);
			}
			else if (dir==1 && (game->world.spaceFree(mapPos+Vector2i(1,0)) || ignoreCols))
			{
				lastPos = mapPos;
				isMoving = playAnims;
				mapPos.x++;
				game->world.setSpaceOccupied(mapPos,true);
				game->world.moveOntoTile(mapPos);
			}
			else if (dir==2 && (game->world.spaceFree(mapPos+Vector2i(0,1)) || ignoreCols))
			{
				lastPos = mapPos;
				isMoving = playAnims;
				mapPos.y++;
				game->world.setSpaceOccupied(mapPos,true);
				game->world.moveOntoTile(mapPos);
				game->world.updateObjectRenderLocation(this,lastPos.y,mapPos.y);
			}
			else if (dir==3 && (game->world.spaceFree(mapPos+Vector2i(-1,0)) || ignoreCols))
			{
				lastPos = mapPos;
				isMoving = playAnims;
				mapPos.x--;
				game->world.setSpaceOccupied(mapPos,true);
				game->world.moveOntoTile(mapPos);
			}
			else
			{
				isMoving = false;
				return false;
			}
        }
        else
		{
			dir = d;
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

void Character::forceStop()
{
	isMoving = false;
	for (int i = 0; i<4; ++i)
		walking[i].setFrame(0);
}

string Character::getIdentifier()
{
    return "Character Base!!";
}
