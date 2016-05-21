#include "World/People/SpinningController.hpp"
#include "World/People/Character.hpp"
#include "Globals.hpp"
using namespace std;

SpinningController::SpinningController(Character* c, File* input) : Controller(c)
{
    lastTime = 0;
    motion = input->get<uint8_t>();
}

SpinningController::SpinningController(Character* c, int b) : Controller(c)
{
    lastTime = 0;
    motion = b;
}

void SpinningController::act(Game* g)
{
	if (paused)
		return;

    if (gameClock.getTimeStamp()-lastTime>=1000)
    {
        lastTime = gameClock.getTimeStamp();
        int nextDir = 0;

        if (motion==0)
        {
            nextDir = owner->getDir()+1;
            if (nextDir>3)
                nextDir = 0;
        }
        else if (motion==1)
        {
            nextDir = owner->getDir()-1;
            if (nextDir<0)
                nextDir = 3;
        }
        else
        {
            while (nextDir==owner->getDir())
                nextDir = Random(0,3);
        }
        owner->move(g,nextDir);
    }
}
