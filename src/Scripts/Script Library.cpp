#include "Scripts/Script Interpreter.hpp"
#include "Scripts/Script Environment.hpp"
#include "Game/Game.hpp"
#include "Game/BattleState.hpp"
#include "Game/StorageSystem.hpp"
#include "Game/StoreState.hpp"
#include "Game/CreditsState.hpp"
#include "World/People/Trainer.hpp"
#include "World/People/Npc.hpp"
#include "World/Item.hpp"
#include "World/People/PathFinder.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "SFML.hpp"
#include <iostream>
#include <exception>
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
					"clearLights",
					"controlPressed",
					"drawAnim",
					"editTile",
					"getDaytime",
					"getTimeHours",
					"getTimeMinutes",
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
					"runScriptAtTime",
					"resetPlayer",
					"saveGame",
					"setCollisions",
					"setLightingOverride",
					"setMusic",
					"setNPCLock",
					"setPlayerLock",
					"setTrainerLock",
					"setTrainerBeaten",
					"setWeather",
					"shiftNPC",
					"shiftPlayer",
					"shiftTrainer",
					"showCredits",
					"sleep",
					"sleepToTime",
					"spaceFree",
					"spawnItem",
					"spawnNPC",
					"spawnTrainer",
					"sqrt",
					"startBattle",
					"startTrainerBattle",
					"stringToInt",
					"takeItem",
					"trainerDefeated",
					"trainerExists",
					"trainerToLocation",
					"trainerXPos",
					"trainerYPos",
					"whiteOut"});
}

bool Script::isLibraryFunction(string name)
{
	return find(functionNames.begin(),functionNames.end(),name)!=functionNames.end();
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
			environment->getGame()->player.move(environment->getGame(),args.at(0).iValue,bool(args.at(1).iValue),bool(args.at(2).iValue),true,true);
		else if (name=="playerToLocation")
		{
			environment->getGame()->player.setLock(true,true);
			if (args.at(2).iValue==0)
			{
				PathFinder finder(environment->getGame(), environment->getGame()->player.getMapPos(), Vector2i(args.at(0).iValue,args.at(1).iValue), environment->getGame()->player.getDir());
				vector<int> moves = finder.getPath();
				for (unsigned int i = 0; i<moves.size(); ++i)
					environment->getGame()->player.move(environment->getGame(),moves[i],false,true,true,true);
			}
			else
			{
				PathFinder finder(environment->getGame(), environment->getGame()->player.getMapPos(), Vector2i(args.at(0).iValue,args.at(1).iValue), environment->getGame()->player.getDir());
				vector<int> moves = finder.getPath();

				int retries = 0;
				while (true)
				{
					if (moves.size()==0) {
						if (environment->getGame()->player.getMapPos()==Vector2i(args.at(0).iValue,args.at(1).iValue))
							break;
						else if (retries<3) {
							retries++;
							sleep(milliseconds(2000));
							PathFinder finder(environment->getGame(), environment->getGame()->player.getMapPos(), Vector2i(args.at(0).iValue,args.at(1).iValue), environment->getGame()->player.getDir());
							moves = finder.getPath();
						}
						else {
							cout << "Warning: Player failed to find path after 3 tries\n";
							break;
						}
					}
					else {
						if (!environment->getGame()->player.move(environment->getGame(),moves[0],false,true,false,true)) {
							PathFinder finder(environment->getGame(), environment->getGame()->player.getMapPos(), Vector2i(args.at(0).iValue,args.at(1).iValue), environment->getGame()->player.getDir());
							moves = finder.getPath();
						}
						else if (moves.size()>0)
							moves.erase(moves.begin());
					}
					sleep(milliseconds(10));
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
			environment->getGame()->player.setLock(bool(args.at(0).iValue), bool(args.at(0).iValue)); //pass the lock in to the save arg as well to handle issue with convo state reseting
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
		else if (name=="resetPlayer")
		{
			environment->getGame()->player.forceStop();
			environment->getGame()->player.setLock(false,false);
			environment->getGame()->player.clearQueue();
		}
		else if (name=="spawnTrainer")
		{
			Trainer* t = new Trainer(environment->getGame(),Properties::TrainerPath+args.at(0).sValue,false);
			if (environment->getGame()->world.trainerBeaten(t->getName()))
				t->setBeaten();
			t->spawn(Vector2f(args.at(1).iValue*32,args.at(2).iValue*32),args.at(3).iValue);
			environment->getGame()->world.addObject(t);
			if (args.size()==5)
			{
				while (environment->getGame()->world.getTrainer(t->getName())==nullptr && args.at(4).iValue==1)
					sleep(milliseconds(1));
			}
		}
		else if (name=="moveTrainer")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			bool ignoreCols = (args.size()>1)?(args.at(1).iValue!=0):(false);
			bool playAnims = (args.size()>2)?(args.at(2).iValue!=0):(true);
			ret.iValue = 0;
			if (t)
				ret.iValue = t->move(environment->getGame(),args.at(1).iValue,ignoreCols,playAnims,true,true);
			else
				cout << "Failed to find Trainer \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="trainerXPos")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			ret.iValue = 0;
			if (t)
				ret.iValue = t->getMapPos().x;
			else
				cout << "Failed to find Trainer \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="trainerYPos")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			ret.iValue = 0;
			if (t)
				ret.iValue = t->getMapPos().y;
			else
				cout << "Failed to find Trainer \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="trainerToLocation")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			if (t)
			{
				t->setLock(true,true);
				if (args.at(3).iValue==0)
				{
					PathFinder finder(environment->getGame(), t->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), t->getDir());
					vector<int> moves = finder.getPath();
					for (unsigned int i = 0; i<moves.size(); ++i)
						t->move(environment->getGame(),moves[i],false,true,true,true);
				}
				else
				{
					PathFinder finder(environment->getGame(), t->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), t->getDir());
					vector<int> moves = finder.getPath();

					int retries = 0;
					while (true)
					{
						if (moves.size()==0) {
							if (t->getMapPos()==Vector2i(args.at(1).iValue,args.at(2).iValue))
								break;
							else if (retries<3) {
								retries++;
								sleep(milliseconds(2000));
								PathFinder finder(environment->getGame(), t->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), t->getDir());
								moves = finder.getPath();
							}
							else {
								cout << "Warning: Trainer " << t->getName() << " failed to find path after 3 tries\n";
								break;
							}
						}
						else {
							if (!t->move(environment->getGame(),moves[0],false,true,false,true)) {
								PathFinder finder(environment->getGame(), t->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), t->getDir());
								moves = finder.getPath();
							}
							else if (moves.size()>0)
								moves.erase(moves.begin());
						}
						sleep(milliseconds(10));
					}
				}
			}
			else
				cout << "Failed to find Trainer \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="shiftTrainer")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			if (t)
				t->shift(args.at(1).iValue,args.at(2).iValue);
			else
				cout << "Failed to find Trainer \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="setTrainerLock")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			if (t)
				t->setLock(args.at(1).iValue);
			else
				cout << "Failed to find Trainer \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="setTrainerBeaten") {
			string name = args.at(0).sValue;
			bool beaten = args.at(1).iValue==1;
			environment->getGame()->world.setTrainerBeaten(name, beaten);
		}
		else if (name=="removeTrainer")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			if (t)
				environment->getGame()->world.removeObject(t);
			else
				cout << "Failed to find Trainer \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="trainerExists")
		{
			Trainer* t = environment->getGame()->world.getTrainer(args.at(0).sValue);
			ret.iValue = t!=nullptr;
		}
		else if (name=="trainerDefeated")
			ret.iValue = environment->getGame()->world.checkTalkedTo(args.at(0).sValue);
		else if (name=="spawnNPC")
		{
			Npc* n = new Npc(Properties::NpcPath+args.at(0).sValue,false);
			n->spawn(Vector2f(args.at(1).iValue*32,args.at(2).iValue*32),args.at(3).iValue);
			environment->getGame()->world.addObject(n);
			if (args.size()==5)
			{
				while (environment->getGame()->world.getNPC(n->getName())==nullptr && args.at(4).iValue==1)
					sleep(milliseconds(1));
			}
		}
		else if (name=="moveNPC")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			bool ignoreCols = (args.size()>1)?(args.at(1).iValue!=0):(false);
			bool playAnims = (args.size()>2)?(args.at(2).iValue!=0):(true);
			ret.iValue = 0;
			if (n)
				ret.iValue = n->move(environment->getGame(),args.at(1).iValue,ignoreCols,playAnims,true,true);
			else
				cout << "Failed to find NPC \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="npcXPos")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			ret.iValue = 0;
			if (n)
				ret.iValue = n->getMapPos().x;
			else
				cout << "Failed to find NPC \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="npcYPos")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			ret.iValue = 0;
			if (n)
				ret.iValue = n->getMapPos().y;
			else
				cout << "Failed to find NPC \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="npcToLocation")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			if (n)
			{
				n->setLock(true,true);
				if (args.at(3).iValue==0)
				{
					PathFinder finder(environment->getGame(), n->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), n->getDir());
					vector<int> moves = finder.getPath();
					for (unsigned int i = 0; i<moves.size(); ++i)
						n->move(environment->getGame(),moves[i],false,true,true,true);
				}
				else
				{
					PathFinder finder(environment->getGame(), n->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), n->getDir());
					vector<int> moves = finder.getPath();

					int retries = 0;
					while (true)
					{
						if (moves.size()==0) {
							if (n->getMapPos()==Vector2i(args.at(1).iValue,args.at(2).iValue))
								break;
							else if (retries<3) {
								retries++;
								sleep(milliseconds(2000));
								PathFinder finder(environment->getGame(), n->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), n->getDir());
								moves = finder.getPath();
							}
							else {
								cout << "Warning: NPC " << n->getName() << " failed to find path after 3 tries\n";
								break;
							}
						}
						else {
							if (!n->move(environment->getGame(),moves[0],false,true,false,true)) {
								PathFinder finder(environment->getGame(), n->getMapPos(), Vector2i(args.at(1).iValue,args.at(2).iValue), n->getDir());
								moves = finder.getPath();
							}
							else if (moves.size()>0)
								moves.erase(moves.begin());
						}
						sleep(milliseconds(10));
					}
				}
			}
			else
				cout << "Failed to find NPC \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="shiftNPC")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			if (n)
				n->shift(args.at(1).iValue,args.at(2).iValue);
			else
				cout << "Failed to find NPC \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="setNPCLock")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			if (n)
				n->setLock(args.at(1).iValue);
			else
				cout << "Failed to find NPC \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="removeNPC")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			if (n)
				environment->getGame()->world.removeObject(n);
			else
				cout << "Failed to find NPC \"" << args.at(0).sValue << "\"!\n";
		}
		else if (name=="npcExists")
		{
			Npc* n = environment->getGame()->world.getNPC(args.at(0).sValue);
			ret.iValue = n!=nullptr;
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
			string nextBattleBgnd = args.at(2).sValue;
			string nextBattleMusic = args.at(1).sValue;
			string nextBattlePplmon = args.at(0).sValue;
			bool block = (args.size()>3)?(args.at(3).iValue==1):(true);
			vector<PeoplemonRef> pplmon;
			PeoplemonRef ppl;
			ppl.load(environment->getGame(),Properties::WildPeoplemonPath+nextBattlePplmon);
			pplmon.push_back(ppl);
			environment->getGame()->data.nextState = new BattleState(environment->getGame(),createBattler(0,&pplmon,vector<int>()),ppl.name,"",0,false,nextBattleMusic,nextBattleBgnd);
			if (block)
			{
				while (environment->getGame()->data.nextState != nullptr)
					sleep(milliseconds(200));
			}
			ret.iValue = environment->getGame()->data.lastBattleWon;
		}
		else if (name=="startTrainerBattle")
		{
			string tnrFile = args.at(0).sValue;
			Trainer* tnr = new Trainer(environment->getGame(),Properties::TrainerPath+tnrFile,false);
			environment->getGame()->data.nextState = new BattleState(environment->getGame(),createBattler(tnr->aiType,&tnr->peoplemon,tnr->items),tnr->getName(),tnr->loserSay,tnr->prizeMoney,false,tnr->bMusic,tnr->bBgnd);
			while (environment->getGame()->data.nextState != nullptr)
				sleep(milliseconds(200));
			ret.iValue = environment->getGame()->data.lastBattleWon;
			delete tnr;
		}
		else if (name=="saveGame")
			environment->getGame()->data.saveGameFlag = true;
		else if (name=="runScript")
		{
			ScriptReference scr = scriptPool.loadResource(args.at(0).sValue);
			environment->runScript(scr,args.at(1).iValue!=0);
		}
		else if (name=="runScriptAtTime") {
			ClockTime t = gameClock.getClockTime();
            while (t.hour!=args.at(1).iValue && t.minute>args.at(2).iValue) {
                sleep(milliseconds(250));
                t = gameClock.getClockTime();
            }
            ScriptReference scr = scriptPool.loadResource(args.at(0).sValue);
			environment->runScript(scr,true);
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
		else if (name=="spaceFree")
		{
            int x = args.at(0).iValue;
            int y = args.at(1).iValue;
            int d = args.at(2).iValue;
            int dist = args.at(3).iValue;
            if (d==0)
				y -= dist;
			if (d==1)
				x += dist;
			if (d==2)
				y += dist;
			if (d==3)
				x -= dist;
			ret.iValue = double(!environment->getGame()->world.spaceFree(Vector2i(x,y)));
			cout << "space free (" << x << "," << y << ") = " << ret.iValue << endl;
		}
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
			environment->getGame()->data.nextState = new StorageSystem(environment->getGame(),environment->getGame()->player.getStoredPeoplemon());
		else if (name=="openStore")
		{
			vector<pair<int,int> > storeItems;
			string storePrompt = args.at(0).sValue;
			string storeError = args.at(1).sValue;
			for (unsigned int i = 2; i<args.size(); i+=2)
				storeItems.push_back(make_pair(args.at(i).iValue,args[i+1].iValue));
			environment->getGame()->data.nextState = new StoreState(environment->getGame(),storePrompt, storeError, storeItems);
		}
		else if (name=="showCredits")
			environment->getGame()->data.nextState = new CreditsState(environment->getGame());
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
		else if (name=="sleep") {
			int blocks = args.at(0).iValue/50;
			int remainder = int(args.at(0).iValue)%50;
			for (int i = 0; i<blocks; ++i) {
				sleep(milliseconds(50));
				if (stopping)
					break;
			}
			sleep(milliseconds(remainder));
		}
		else if (name=="sleepToTime") {
            ClockTime t = gameClock.getClockTime();
            while (t.hour!=args.at(0).iValue || t.minute!=args.at(1).iValue) {
                sleep(milliseconds(250));
                t = gameClock.getClockTime();
                if (stopping)
					break;
            }
		}
		else if (name=="getTimeMinutes") {
			ClockTime t = gameClock.getClockTime();
			ret.iValue = t.minute;
		}
		else if (name=="getTimeHours") {
			ClockTime t = gameClock.getClockTime();
			ret.iValue = t.hour;
		}
		else if (name=="getDaytime") {
			ClockTime t = gameClock.getClockTime();
			ret.iValue = t.hour*60+t.minute;
		}
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
