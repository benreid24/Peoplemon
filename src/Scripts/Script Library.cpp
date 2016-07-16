#include "Scripts/Script Interpreter.hpp"
#include "Scripts/Script Environment.hpp"
#include "Game/Game.hpp"
#include "World/People/Trainer.hpp"
#include "World/People/Npc.hpp"
#include "World/Item.hpp"
#include "World/People/PathFinder.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "SFML.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
#include <cmath>
using namespace std;
using namespace sf;

namespace {
	vector<string> functionNames(
				   {"abs",
					"addLight",
					"addSaveEntry",
					"addSaveEntry",
					"alterMoney",
					"cameraXPos",
					"cameraYPos",
					"choiceBox",
					"chooseStarter",
					"clearLights",
					"controlPressed",
					"drawAnim",
					"editTile",
					"getSaveEntry",
					"getSaveEntry",
					"giveItem",
					"givePeoplemon",
					"hasItem",
					"healPeoplemon",
					"interact",
					"intToString",
					"loadMap",
					"mapVisited",
					"messageBox",
					"moveNPC",
					"movePlayer",
					"moveTrainer",
					"npcExists",
					"npcTalkedTo",
					"npcToLocation",
					"npcXPos",
					"npcYPos",
					"openStorageSystem",
					"openStore",
					"playerDir",
					"playerGender",
					"playerName",
					"playerToLocation",
					"playerXScreenPos",
					"playerXWorldPos",
					"playerYScreenPos",
					"playerYWorldPos",
					"playMapAnim",
					"playSound",
					"pow",
					"print",
					"random",
					"removeLight",
					"removeNPC",
					"removeTrainer",
					"runScript",
					"saveGame",
					"setCollisions",
					"setLightingOverride",
					"setMusic",
					"setNPCLock",
					"setPlayerLock",
					"setTrainerLock",
					"setWeather",
					"shiftNPC",
					"shiftPlayer",
					"shiftTrainer",
					"showCredits",
					"sleep",
					"spawnItem",
					"spawnNPC",
					"spawnTrainer",
					"sqrt",
					"startBattle",
					"stringToInt",
					"takeItem",
					"trainerDefeated",
					"trainerExists",
					"trainerToLocation",
					"trainerXPos",
					"trainerYPos",
					"whiteOut"});

	int checkString(string target, string s)
	{
		for (unsigned int i = 0; i<target.size() && i<s.size(); ++i)
		{
			if (toupper(target[i])<toupper(s[i]))
				return -1;
			if (toupper(target[i])>toupper(s[i]))
				return 1;
		}
		return 0;
	}
}

bool Script::isLibraryFunction(string name)
{
	return find(functionNames.begin(),functionNames.end(),name)!=functionNames.end();
	int center, width;
	center = width = functionNames.size()/2;

	while (width>=1)
	{
		if (functionNames[center]==name)
			return true;
		else
		{
			center += (width/2)*checkString(name,functionNames[center]);
			width /= 2;
		}
	}

	return false;
}

Value Script::executeLibraryFunction(string name, vector<Value> args)
{
	Value ret;
	ret.type = Value::Integer;

	try
	{
		if (name=="print")
		{
			for (unsigned int i = 0; i<args.size(); ++i)
			{
				if (args.at(i).type==Value::Integer)
					cout << args.at(i).iValue;
				else
					cout << args.at(i).sValue;
			}
		}
		else if (name=="playerName")
		{
			ret.type = Value::String;
			ret.sValue = environment->getGame()->player.getName();
		}
		else if (name=="playerXWorldPos")
			ret.iValue = environment->getGame()->player.getPosition().x;
		else if (name=="playerYWorldPos")
			ret.iValue = environment->getGame()->player.getPosition().y;
		else if (name=="playerXScreenPos")
			ret.iValue = environment->getGame()->player.getPosition().x-environment->getGame()->world.getCamera().x*32;
		else if (name=="playerYScreenPos")
			ret.iValue = environment->getGame()->player.getPosition().y-environment->getGame()->world.getCamera().y*32;
		else if (name=="cameraXPos")
			ret.iValue = environment->getGame()->world.getCamera().x;
		else if (name=="cameraYPos")
			ret.iValue = environment->getGame()->world.getCamera().y;
		else if (name=="playerGender")
		{
			ret.type = Value::String;
			ret.sValue = environment->getGame()->player.getGender();
		}
		else if (name=="playerDir")
			ret.iValue = environment->getGame()->player.getDir();
		else if (name=="movePlayer")
			environment->getGame()->player.move(environment->getGame(),args.at(0).iValue,bool(args.at(1).iValue),bool(args.at(2).iValue),true);
		else if (name=="playerToLocation")
		{
			if (args.at(2).iValue==0)
			{
				PathFinder finder(environment->getGame(), environment->getGame()->player.getMapPos(), Vector2i(args.at(0).iValue,args.at(1).iValue), environment->getGame()->player.getDir());
				vector<int> moves = finder.getPath();
				for (unsigned int i = 0; i<moves.size(); ++i)
					environment->getGame()->player.move(environment->getGame(),moves[i],false,true,true);
			}
			else
			{
				vector<int> moves = {1};
				while (moves.size()>0)
				{
					PathFinder finder(environment->getGame(), environment->getGame()->player.getMapPos(), Vector2i(args.at(0).iValue,args.at(1).iValue), environment->getGame()->player.getDir());
					moves = finder.getPath();
					if (moves.size()>0)
						environment->getGame()->player.move(environment->getGame(),moves[0]);
				}
			}
		}
		else if (name=="shiftPlayer")
			environment->getGame()->player.shift(args.at(0).iValue,args.at(1).iValue);
		else if (name=="interact")
			environment->getGame()->data.interactFlag = true;
		else if (name=="controlPressed")
		{
			switch (int(args.at(0).iValue+0.1))
			{
			case 0:
				ret.iValue = user.isInputActive(PlayerInput::Up);
				break;
			case 1:
				ret.iValue = user.isInputActive(PlayerInput::Right);
				break;
			case 2:
				ret.iValue = user.isInputActive(PlayerInput::Down);
				break;
			case 3:
				ret.iValue = user.isInputActive(PlayerInput::Left);
				break;
			case 4:
				ret.iValue = user.isInputActive(PlayerInput::Run);
				break;
			case 5:
				ret.iValue = user.isInputActive(PlayerInput::Interact);
				break;
			default:
				ret.iValue = 0;
			}
		}
		else if (name=="setPlayerLock")
			environment->getGame()->player.setLock(bool(args.at(0).iValue));
		else if (name=="giveItem")
		{
			environment->getGame()->player.giveItem(args.at(0).iValue);
			if (args.size()>1)
				environment->getGame()->hud.displayMessage(args.at(1).sValue,"");
		}
		else if (name=="takeItem")
		{
			bool take = true;
			if (args.size()>1)
				take = environment->getGame()->hud.getChoice(args.at(1).sValue, vector<string>({"Yes","No"}),true)=="Yes";
			if (take)
				environment->getGame()->player.takeItem(args.at(0).iValue);
		}
		else if (name=="alterMoney")
		{
			bool r = environment->getGame()->player.alterMoney(args.at(0).iValue);
			ret.iValue = r?(1):(0);
		}
		else if (name=="givePeoplemon")
		{
            PeoplemonRef ppl;
            ppl.load(environment->getGame(),Properties::OwnedPeoplemonPath+args.at(0).sValue);
            if (environment->getGame()->player.getCurrentPeoplemon()->size()>=6)
			{
				environment->getGame()->hud.displayMessage(ppl.name+" was transferred to the PC");
				environment->getGame()->player.addStoredPeoplemon(ppl);
			}
			else
			{
				environment->getGame()->hud.displayMessage("Received the "+ppl.name+"!");
				environment->getGame()->player.getCurrentPeoplemon()->push_back(ppl);
			}
		}
		else if (name=="healPeoplemon")
            environment->getGame()->player.whiteout();
		else if (name=="whiteOut")
		{
			if (args.size()>0)
				environment->getGame()->hud.displayMessage(args.at(0).sValue);
			if (args.size()>1)
				environment->getGame()->player.alterMoney(-args.at(1).iValue);
			environment->getGame()->data.whiteoutFlag = true;
		}
		else if (name=="spawnTrainer")
		{
			Trainer* t = new Trainer(environment->getGame(),Properties::TrainerPath+args.at(0).sValue,false);
			t->spawn(Vector2f(args.at(1).iValue*32,args.at(2).iValue*32),args.at(3).iValue);
			environment->getGame()->world.addObject(t);
		}
		else if (name=="moveTrainer")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			bool ignoreCols = (args.size()>1)?(args.at(1).iValue!=0):(false);
			bool playAnims = (args.size()>2)?(args.at(2).iValue!=0):(true);
			ret.iValue = 0;
			if (t)
				ret.iValue = t->move(environment->getGame(),args.at(1).iValue,ignoreCols,playAnims,true);
		}
		else if (name=="trainerXPos")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			ret.iValue = 0;
			if (t)
				ret.iValue = t->getMapPos().x;
		}
		else if (name=="trainerYPos")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			ret.iValue = 0;
			if (t)
				ret.iValue = t->getMapPos().y;
		}
		else if (name=="trainerToLocation")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			if (t)
			{
				if (args.at(3).iValue==0)
				{
					PathFinder finder(environment->getGame(), t->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), t->getDir());
					vector<int> moves = finder.getPath();
					for (unsigned int i = 0; i<moves.size(); ++i)
						t->move(environment->getGame(),moves[i],false,true,true);
				}
				else
				{
					vector<int> moves = {1};
					while (moves.size()>0)
					{
						PathFinder finder(environment->getGame(), t->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), t->getDir());
						moves = finder.getPath();
						if (moves.size()>0)
							t->move(environment->getGame(),moves[0]);
					}
				}
			}
		}
		else if (name=="shiftTrainer")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			if (t)
				t->shift(args.at(1).iValue,args.at(2).iValue);
		}
		else if (name=="setTrainerLock")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			if (t)
				t->setLock(args.at(1).iValue);
		}
		else if (name=="removeTrainer")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			if (t)
				environment->getGame()->world.removeObject(t);
		}
		else if (name=="trainerExists")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			ret.iValue = t!=NULL;
		}
		else if (name=="trainerDefeated")
			ret.iValue = environment->getGame()->world.checkTalkedTo(args.at(0).sValue);
		else if (name=="spawnNPC")
		{
			Npc* n = new Npc(Properties::NpcPath+args.at(0).sValue,false);
			n->spawn(Vector2f(args.at(1).iValue*32,args.at(2).iValue*32),args.at(3).iValue);
			environment->getGame()->world.addObject(n);
		}
		else if (name=="moveNPC")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			bool ignoreCols = (args.size()>1)?(args.at(1).iValue!=0):(false);
			bool playAnims = (args.size()>2)?(args.at(2).iValue!=0):(true);
			ret.iValue = 0;
			if (n)
				ret.iValue = n->move(environment->getGame(),args.at(1).iValue,ignoreCols,playAnims,true);
		}
		else if (name=="npcXPos")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			ret.iValue = 0;
			if (n)
				ret.iValue = n->getMapPos().x;
		}
		else if (name=="npcYPos")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			ret.iValue = 0;
			if (n)
				ret.iValue = n->getMapPos().y;
		}
		else if (name=="npcToLocation")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			if (n)
			{
				if (args.at(3).iValue==0)
				{
					PathFinder finder(environment->getGame(), n->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), n->getDir());
					vector<int> moves = finder.getPath();
					for (unsigned int i = 0; i<moves.size(); ++i)
						n->move(environment->getGame(),moves[i],false,true,true);
				}
				else
				{
					vector<int> moves = {1};
					while (moves.size()>0)
					{
						PathFinder finder(environment->getGame(), n->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), n->getDir());
						moves = finder.getPath();
						if (moves.size()>0)
							n->move(environment->getGame(),moves[0]);
					}
				}
			}
		}
		else if (name=="shiftNPC")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			if (n)
				n->shift(args.at(1).iValue,args.at(2).iValue);
		}
		else if (name=="setNPCLock")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			if (n)
				n->setLock(args.at(1).iValue);
		}
		else if (name=="removeNPC")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			if (n)
				environment->getGame()->world.removeObject(n);
		}
		else if (name=="npcExists")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			ret.iValue = n!=NULL;
		}
		else if (name=="npcTalkedTo")
			ret.iValue = environment->getGame()->world.checkTalkedTo(args.at(0).sValue);
		else if (name=="hasItem")
			ret.iValue = environment->getGame()->player.hasItem(args.at(0).iValue);
		else if (name=="spawnItem")
		{
			Item* i = new Item(&environment->getGame()->world, args.at(0).iValue,-1,Vector2f(args.at(1).iValue,args.at(2).iValue));
			environment->getGame()->world.addObject(i);
		}
		else if (name=="loadMap")
		{
			environment->getGame()->data.nextMapName = args.at(0).sValue;
			environment->getGame()->data.nextSpawnId = args.at(1).iValue;
			environment->getGame()->data.loadMapFlag = true;
		}
		else if (name=="mapVisited")
			ret.iValue = environment->getGame()->world.mapVisited(args.at(0).sValue);
		else if (name=="startBattle")
		{
			environment->getGame()->data.nextBattleBgnd = args.at(2).sValue;
			environment->getGame()->data.nextBattleMusic = args.at(1).sValue;
			environment->getGame()->data.nextBattlePplmon = args.at(0).sValue;
		}
		else if (name=="saveGame")
			environment->getGame()->data.saveGameFlag = true;
		else if (name=="runScript")
		{
			shared_ptr<Script> scr(new Script(args.at(0).sValue));
			if (args.at(1).iValue)
				environment->runScript(scr);
			else
				scr->run(environment);
		}
		else if (name=="setMusic")
		{
			environment->getGame()->music.stop();
			environment->getGame()->music.load(args.at(0).sValue);
			environment->getGame()->music.play();
		}
		else if (name=="playSound")
			environment->getGame()->soundEngine.playSound(args.at(0).sValue,args.at(1).iValue);
		else if (name=="playMapAnim")
			environment->getGame()->world.moveOntoTile(Vector2i(args.at(0).iValue,args.at(1).iValue));
		else if (name=="drawAnim")
			environment->getGame()->hud.playAnimation(args.at(0).sValue,args.at(1).iValue,args.at(2).iValue,args.at(3).iValue);
		else if (name=="setWeather")
			environment->getGame()->data.nextWeather = args.at(0).iValue;
		else if (name=="addLight")
			environment->getGame()->world.addLight(args.at(0).iValue,args.at(1).iValue,args.at(2).iValue);
		else if (name=="removeLight")
			environment->getGame()->world.removeLight(args.at(0).iValue,args.at(1).iValue);
		else if (name=="clearLights")
			environment->getGame()->world.removeAllLights();
		else if (name=="setLightingOverride")
			environment->getGame()->world.setLightingOverride(args.at(0).iValue);
		else if (name=="editTile")
			environment->getGame()->world.editTile(args.at(0).iValue,args.at(1).iValue,args.at(2).iValue,args.at(3).iValue);
		else if (name=="setCollisions")
		{
			for (int x = 0; x<args.at(2).iValue; ++x)
			{
				for (int y = 0; y<args.at(3).iValue; ++y)
				{
					environment->getGame()->world.setCollision(args.at(0).iValue+x,args.at(1).iValue+y,args.at(4).iValue);
				}
			}
		}
		else if (name=="messageBox")
		{
			environment->getGame()->hud.setAlwaysShow(true);
			for (unsigned int i = 0; i<args.size(); ++i)
				environment->getGame()->hud.displayMessage(args.at(i).sValue,"",true);
			environment->getGame()->hud.setAlwaysShow(false);
		}
		else if (name=="choiceBox")
		{
			ret.type = Value::String;
			vector<string> cs;
			for (unsigned int i = 1; i<args.size(); ++i)
				cs.push_back(args.at(i).sValue);
			ret.sValue = environment->getGame()->hud.getChoice(args.at(0).sValue,cs);
		}
		else if (name=="openStorageSystem")
			environment->getGame()->data.openStorageSystemFlag = true;
		else if (name=="openStore")
		{
			environment->getGame()->data.storeItems.clear();
			environment->getGame()->data.storePrompt = args.at(0).sValue;
			environment->getGame()->data.storeError = args.at(1).sValue;
			for (unsigned int i = 2; i<args.size(); i+=2)
			{
				environment->getGame()->data.storeItems.push_back(make_pair(args.at(i).iValue,args[i+1].iValue));
			}
			environment->getGame()->data.openStoreFlag = true;
		}
		else if (name=="showCredits")
			environment->getGame()->data.playCreditsFlag = true;
		else if (name=="chooseStarter")
			environment->getGame()->data.chooseStarterFlag = true;
		else if (name=="stringToInt")
		{
			stringstream conv(args.at(0).sValue);
			conv >> ret.iValue;
		}
		else if (name=="intToString")
		{
			stringstream conv;
			conv << args.at(0).iValue;
			ret.type = Value::String;
			ret.sValue = conv.str();
		}
		else if (name=="sleep")
			sleep(milliseconds(args.at(0).iValue));
		else if (name=="addSaveEntry")
		{
			if (args.at(1).type==Value::Integer)
				environment->intSaveEntries[args.at(0).sValue] = args.at(1).iValue;
			else
				environment->stringSaveEntries[args.at(0).sValue] = args.at(1).sValue;
		}
		else if (name=="getSaveEntry")
		{
			ret.iValue = 0;
			if (environment->intSaveEntries.find(args.at(0).sValue)!=environment->intSaveEntries.end())
				ret.iValue = environment->intSaveEntries[args.at(0).sValue];
			else if (environment->stringSaveEntries.find(args.at(0).sValue)!=environment->stringSaveEntries.end())
			{
				ret.type = Value::String;
				ret.sValue = environment->stringSaveEntries[args.at(0).sValue];
			}
		}
		else if (name=="abs")
			ret.iValue = abs(args.at(0).iValue);
		else if (name=="pow")
			ret.iValue = pow(args.at(0).iValue,args.at(1).iValue);
		else if (name=="sqrt")
			ret.iValue = sqrt(args.at(0).iValue);
		else if (name=="random")
			ret.iValue = rand()%int(args.at(1).iValue-args.at(0).iValue)+args.at(0).iValue;
	}
	catch (out_of_range e)
	{
		throw runtime_error("Not enough arguments in call to "+name);
	}

	return ret;
}
