#include "Game/Gamestate.hpp"
#include "Game/Game.hpp"
#include <iostream>
using namespace sf;
using namespace std;

Gamestate::Gamestate(Game* g, Gamestate* n)
{
    game = g;
    next = n;
}

Gamestate::~Gamestate()
{
    sleep(milliseconds(250));
}

bool Gamestate::finishFrame()
{
    Event event;
    while (game->mainWindow.pollEvent(event))
    {
        if (event.type==Event::Closed)
        {
            game->mainWindow.close();
            return true;
        }
        if (event.type==Event::GainedFocus)
			game->inFocus = true;
		if (event.type==Event::LostFocus)
		{
			game->inFocus = false;
			game->data.pauseGameFlag = true;
		}
    }
    return false;
}

bool Gamestate::run()
{
    if (execute())
        return true;
    if (next)
        return next->run();
    return false;
}
