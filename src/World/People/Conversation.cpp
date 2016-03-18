#include "World/People/Conversation.hpp"
#include "Util/File.hpp"
#include "Scripts/Script Environment.hpp"
#include "World/People/Player.hpp"
#include "Properties.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include <memory>
using namespace std;

Conversation::Conversation()
{
    cLine = 0;
}

Conversation::Conversation(string file)
{
    load(file);
}

void Conversation::setLine(string l)
{
    for (unsigned int i = 0; i<lines.size(); ++i)
    {
        if (lines[i].code=='l' && lines[i].say==l)
        {
            cLine = i;
            return;
        }
    }
    cLine = lines.size();
}

void Conversation::load(string file)
{
    File input(Properties::ConversationPath+file);
    ConvoLine temp;
    int numLines = input.get<uint16_t>();

    lines.resize(numLines+1);
    lines[0] = temp; //start at 1 because [0] is never seen due to how update() was made
    cLine = 0;
    for (int i = 1; i<=numLines; ++i)
    {
        temp.code = input.get<uint8_t>();

        if (temp.code=='t' || temp.code=='l')
			temp.say = input.getString();
        else if (temp.code=='o')
        {
            temp.say = input.getString();
            int s = input.get<uint16_t>();
            temp.d1 = choices.size();
            for (int j = 0; j<s; ++j)
            {
                file = input.getString();
                string t = input.getString();
                choices.push_back(make_pair(t,file));
            }
            temp.d2 = choices.size();
        }
        else if (temp.code=='j' || temp.code=='z')
			temp.line = input.getString();
        else if (temp.code=='r')
        {
        	temp.d1 = input.get<uint8_t>();
            temp.d2 = input.get<uint16_t>();
            temp.line = input.getString();
        }
        else if (temp.code=='g')
        {
            temp.d1 = input.get<uint8_t>();
            temp.d2 = input.get<uint16_t>();
        }
        else if (temp.code=='s')
            temp.say = input.getString();
        else if (temp.code=='c')
        {
            temp.say = input.getString();
            temp.line = input.getString();
        }
        else if (temp.code=='w')
			temp.line = input.getString();
		lines[i] = temp;
    }
}

void Conversation::reset()
{
    cLine = 0;
}

vector<string> Conversation::update(Game* game, Player* player, Character* person, ScriptEnvironment* env, int choice)
{
    vector<string> ret;

    if (choice!=-1)
    {
        string str = choices.at(lines.at(cLine).d1+choice).first;
        setLine(str);
    }
    else
        cLine++;

   start:

    if (unsigned(cLine)>=lines.size())
        return ret;

    if (lines[cLine].code=='t')
    {
        ret.push_back(lines[cLine].say);
        return ret;
    }
    else if (lines[cLine].code=='o')
    {
        ret.push_back(lines[cLine].say);
        for (int i = lines[cLine].d1; i<lines[cLine].d2; ++i)
        {
            ret.push_back(choices.at(i).second);
        }
        return ret;
    }
    else if (lines[cLine].code=='g')
    {
        if (lines[cLine].d1==1)
		{
			player->giveItem(lines[cLine].d2);
			ret.push_back(player->getName()+" got the "+game->itemList[lines[cLine].d2].name+"!");
		}
        else
		{
			player->alterMoney(lines[cLine].d2);
			ret.push_back(player->getName()+" got "+intToString(lines[cLine].d2)+" money!");
		}
        return ret;
    }
    else if (lines[cLine].code=='r')
    {
    	if (lines[cLine].d1==0)
        {
        	if (!player->alterMoney(-lines[cLine].d2))
			{
				ret.push_back("You don't have enough money!");
				setLine(lines[cLine].line);
				return ret;
			}
			else
			{
				ret.push_back("Gave "+intToString(lines[cLine].d2)+" money!");
				return ret;
			}
        }
        else
		{
			if (!player->hasItem(lines[cLine].d2))
			{
				ret.push_back("You don't have the "+game->itemList[lines[cLine].d2].name+"!");
				setLine(lines[cLine].line);
				return ret;
			}
			else
			{
				player->takeItem(lines[cLine].d2,1);
				ret.push_back("Gave the "+game->itemList[lines[cLine].d2].name+"!");
				return ret;
			}
		}
    }
    else if (lines[cLine].code=='j')
    {
        setLine(lines[cLine].line);
        goto start;
    }
    else if (lines[cLine].code=='s')
    {
        env->intSaveEntries.insert(make_pair(lines[cLine].say,1));
        cLine++;
        goto start;
    }
    else if (lines[cLine].code=='c')
    {
        if (env->intSaveEntries.find(lines[cLine].say)==env->intSaveEntries.end())
            setLine(lines[cLine].line);
		else
			cLine++;
		goto start;
    }
    else if (lines[cLine].code=='l')
    {
        cLine++;
        goto start;
    }
    else if (lines[cLine].code=='w')
	{
        if (!env->getGame()->world.checkTalkedTo(person->getName()))
			setLine(lines[cLine].line);
		else
			cLine++;
		goto start;
	}
	else if (lines[cLine].code=='z')
	{
		env->runScript(shared_ptr<Script>(new Script(lines[cLine].line)));
		cLine++;
		goto start;
	}
    return ret;
}
