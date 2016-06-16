#include "Game/Gamestate.hpp"
#include "Game/Game.hpp"
#include "SFML.hpp"
#include "Properties.hpp"
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

		if (event.type==Event::Resized)
		{
			View view(FloatRect(0,0,Properties::ScreenWidth,Properties::ScreenHeight));
			float newWidth = event.size.width;
			float newHeight = event.size.height;
			float xScale = newWidth/float(Properties::ScreenWidth);
			float yScale = newHeight/float(Properties::ScreenHeight);
			float xView = 1;
			float yView = 1;
			float xBase = 0;
			float yBase = 0;

			if (xScale>yScale) //keep y scale
			{
				xView = Properties::ScreenWidth*yScale/newWidth;
				xBase = (1-xView)/2;
			}
			else
			{
				yView = Properties::ScreenHeight*xScale/newHeight;
				yBase = (1-yView)/2;
			}

			view.setViewport(FloatRect(xBase,yBase,xView,yView));
			game->mainWindow.setView(view);
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
