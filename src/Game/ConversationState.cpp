#include "Game/ConversationState.hpp"
#include "Globals.hpp"
#include "World/People/Player.hpp"
#include "Game/Game.hpp"
#include "World/People/Conversation.hpp"
#include "World/People/Trainer.hpp"
using namespace std;
using namespace sf;

ConversationState::ConversationState(Game* g, Character* c, Conversation* cv, Gamestate* n) : Gamestate(g,n)
{
    convo = cv;
    person = c;
    if (!dynamic_cast<Trainer*>(person))
		person->setLock(true,true);
    g->player.setLock(true,true);
}

ConversationState::~ConversationState()
{
    person->resetLock();
    game->player.resetLock();
    game->hud.setAlwaysShow(false);
    game->world.resetMotionTimers();
    user.pause(250); //to prevent player input from "carrying over"
}

void ConversationState::handleData(vector<string> d)
{
    if (d.size()==0)
        return;
    if (d.size()==1)
        game->hud.displayMessage(d[0]);
    else
    {
        string t = d[0];
        d.erase(d.begin());
        game->hud.getChoice(t,d,false);
    }
}

int ConversationState::choiceIndex(vector<string> d, string c)
{
    for (unsigned int i = 1; i<d.size(); ++i)
    {
        if (c==d[i])
            return i-1;
    }
    return -1;
}

bool ConversationState::execute()
{
	game->hud.setAlwaysShow(true);
    convo->reset();
    vector<string> stuff = convo->update(game, &game->player,person,&game->scriptEnvironment);
    bool waitingForChoice = stuff.size()>1;
    handleData(stuff);
    sleep(milliseconds(200));
    int sTime = gameClock.getTimeStamp();
    int rTime = gameClock.getTimeStamp();
    int nextCheckTime = gameClock.getTimeStamp();

    while (!finishFrame())
    {
        if (waitingForChoice && gameClock.getTimeStamp()>=nextCheckTime)
        {
            string temp = game->hud.getChoice();
            if (temp.size()>0)
            {
                stuff = convo->update(game, &game->player, person, &game->scriptEnvironment, choiceIndex(stuff,temp));
                waitingForChoice = stuff.size()>1;
                handleData(stuff);
                nextCheckTime = gameClock.getTimeStamp()+150;
            }
        }
        else if (game->hud.messageFinished() && gameClock.getTimeStamp()>=nextCheckTime)
        {
            stuff = convo->update(game, &game->player, person, &game->scriptEnvironment);
            waitingForChoice = stuff.size()>1;
            handleData(stuff);
            nextCheckTime = gameClock.getTimeStamp()+150;
        }
        if (stuff.size()==0)
            return false;

        game->world.update();
        game->hud.update();

		if (gameClock.getTimeStamp()-rTime>=16)
        {
        	game->mainWindow.clear();
			game->world.draw(&game->mainWindow);
			game->hud.draw(&game->mainWindow);
			game->mainWindow.display();
			rTime = gameClock.getTimeStamp();
        }

        sleep(milliseconds(8-(gameClock.getTimeStamp()-sTime)));
        sTime = gameClock.getTimeStamp();
    }

    return true;
}
