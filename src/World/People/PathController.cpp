#include "World/People/PathController.hpp"
#include "World/People/Character.hpp"
#include "Globals.hpp"
using namespace std;

PathController::PathController(Character* c, File* file) : Controller(c)
{
    int numSteps = file->get<uint8_t>();
    runBackwards = file->get<uint8_t>()==1;
    for (int i = 0; i<numSteps; ++i)
    {
        int dir = file->get<uint8_t>();
        int numPaces = file->get<uint8_t>();
        for (int j = 0; j<numPaces; ++j)
        {
            path.push_back(dir);
        }
    }
    movingForward = true;
    curStep = 0;
    finishedOnce = false;
}

PathController::PathController(Character* c, vector<int> p) : Controller(c)
{
    path = p;
    movingForward = true;
    runBackwards = true;
    finishedOnce = false;
    curStep = 0;
}

int PathController::reverseDirection(int dir)
{
    dir += 2;
    if (dir>3)
        dir -= 4;
    return dir;
}

void PathController::act(Game* g)
{
    int dir = (movingForward)?path[curStep]:reverseDirection(path[curStep]);
    if (owner->getDir()!=dir)
        owner->move(g,dir);
    bool moved = owner->move(g,dir);

    if (moved)
    {
        if (movingForward)
        {
            curStep++;
            if (unsigned(curStep)>=path.size())
            {
                finishedOnce = true;
                if (runBackwards)
                {
                    movingForward = false;
                    curStep--;
                }
                else
                    curStep = 0;
            }
        }
        else
        {
            curStep--;
            if (curStep<0)
            {
                curStep = 0;
                movingForward = true;
            }
        }
    }
}

bool PathController::completed()
{
    return finishedOnce;
}
