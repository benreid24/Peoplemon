#include "World/People/WanderingController.hpp"
#include "World/People/PathController.hpp"
#include "World/People/StandingController.hpp"
#include "World/People/SpinningController.hpp"
#include "World/People/Character.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

WanderingController::WanderingController(Character* c, File* file) : Controller(c)
{
    radius = file->get<uint8_t>();
    initialized = false;
    state = Wander;
    subBehavior = NULL;
    timeOfNextState = 0;
    timeOfNextStep = 0;
}

int WanderingController::magnitudeSquared(Vector2i v)
{
    return v.x*v.x + v.y*v.y;
}

vector<int> WanderingController::generatePath(Game* g)
{
    Vector2i tPos = owner->getMapPos();
    int numSteps = Random(2,(radius==0)?(6):(radius/3));
    vector<int> path;

    for (int i = 0; i<numSteps; ++i)
    {
        int dir = bestDirFromPos(tPos);
        int numPaces = Random(1,(radius==0)?(10):(radius/2));
        Vector2i d(0,0);
        if (dir==0)
            d.y = -1;
        else if (dir==1)
            d.x = 1;
        else if (dir==2)
            d.y = 1;
        else
            d.x = -1;
        for (int j = 0; j<numPaces; ++j)
        {
            Vector2i t = tPos+d;
            if ((magnitudeSquared(Vector2i(tPos.x-startPos.x,tPos.y-startPos.y))<radius*radius || radius==0) && g->world.spaceFree(t,tPos))
            {
                path.push_back(dir);
                tPos = t;
            }
        }
    }

    return path;
}

int WanderingController::bestDirFromPos(Vector2i pos)
{
    if (radius==0)
        return Random(0,3);

    int xWeight = abs(pos.x-startPos.x);
    int yWeight = abs(pos.y-startPos.y);

    bool moveY = (abs(xWeight-yWeight)<=2)?Random(0,1):Random(0,yWeight+xWeight)<yWeight;
    if (moveY)
    {
        int chance = Random(0,1000);
        if (chance<530)
        {
            if (pos.y<startPos.y)
                return 2;
            else
                return 0;
        }
        else
        {
            if (pos.y<startPos.y)
            {
                if (magnitudeSquared(Vector2i(abs(pos.x-startPos.x),abs(pos.y-startPos.y)+1))<radius*radius || radius==0)
                    return 0;
                else
                    return 2;
            }
            else
            {
                if (magnitudeSquared(Vector2i(abs(pos.x-startPos.x),abs(pos.y-startPos.y)+1))<radius*radius || radius==0)
                    return 2;
                else
                    return 0;
            }
        }
    }
    else
    {
        int chance = Random(0,1000);
        if (chance<530)
        {
            if (pos.x<startPos.x)
                return 1;
            else
                return 3;
        }
        else
        {
            if (pos.x<startPos.x)
            {
                if (magnitudeSquared(Vector2i(abs(pos.x-startPos.x)+1,abs(pos.y-startPos.y)))<radius*radius || radius==0)
                    return 3;
                else
                    return 1;
            }
            else
            {
                if (magnitudeSquared(Vector2i(abs(pos.x-startPos.x)+1,abs(pos.y-startPos.y)))<radius*radius || radius==0)
                    return 1;
                else
                    return 3;
            }
        }
    }
}

void WanderingController::act(Game* g)
{
	if (paused)
		return;

    if (!initialized)
    {
        initialized = true;
        startPos.x = owner->getPosition().x/32;
        startPos.y = owner->getPosition().y/32;
        timeOfNextState = gameClock.getTimeStamp()+Random(5000,10000);
    }

    bool changeState = gameClock.getTimeStamp()>=timeOfNextState;
    if (subBehavior)
    {
        if (PathController* t = dynamic_cast<PathController*>(subBehavior))
            changeState = t->completed();
    }

    if (changeState)
    {
        State t = state;
        while (state==t)
            state = State(Random(0,3));

        if (subBehavior)
            delete subBehavior;

        if (state==Still)
        {
            subBehavior = new StandingController(owner);
            timeOfNextState = gameClock.getTimeStamp()+Random(2000,3500);
        }
        if (state==Spinning)
        {
            subBehavior = new SpinningController(owner, Random(0,2));
            timeOfNextState = gameClock.getTimeStamp()+Random(2500,5000);
        }
        if (state==Path)
        {
            vector<int> t = generatePath(g);
            if (t.size()==0)
                state = Wander;
            else
            {
                subBehavior = new PathController(owner, t);
                timeOfNextState = 4000000000;
            }
        }
        if (state==Wander)
        {
            subBehavior = NULL;
            timeOfNextState = gameClock.getTimeStamp()+Random(5000,15000);
        }
    }

    if (subBehavior)
        subBehavior->act(g);
    else
    {
        if (timeOfNextStep<=gameClock.getTimeStamp())
        {
            timeOfNextStep = gameClock.getTimeStamp()+Random(400,1700);
            int dir = bestDirFromPos(Vector2i(owner->getPosition().x/32,owner->getPosition().y/32));
            owner->move(g,dir);
            owner->move(g,dir);
        }
    }
}
