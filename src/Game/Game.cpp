#include "SFML.hpp"
#include "Game/Game.hpp"
#include "Game/MainMenuState.hpp"
#include "Game/EvolveState.hpp"
#include "Properties.hpp"
#include "Util/File.hpp"
#include "Globals.hpp"
#include "version.h"
using namespace sf;
using namespace std;

Game::Game() : scriptEnvironment(this), soundEngine(this), player(this), world(this), hud(this), music(this)
{
    File itemFile("Resources/Data/items.db");
    int size;

    size = itemFile.get<uint16_t>();
    for (int i = 0; i<size; ++i)
    {
        ItemDBEntry temp;
        int id = itemFile.get<uint16_t>();
        temp.name = itemFile.getString();
        temp.description = itemFile.getString();
        temp.price = itemFile.get<uint32_t>();
        itemList[id] = temp;
    }

    File moveFile("Resources/Data/Peoplemon/moves.db");
    size = moveFile.get<uint16_t>();
    for (int i = 0; i<size; ++i)
    {
        Move temp;
        temp.id = moveFile.get<uint16_t>();
        temp.name = moveFile.getString();
        temp.description = moveFile.getString();
        temp.isSpecial = bool(moveFile.get<uint8_t>());
        temp.dmg = moveFile.get<uint16_t>();
        temp.acc = moveFile.get<uint16_t>();
        temp.priority = moveFile.get<uint16_t>();
        temp.pp = moveFile.get<uint16_t>();
        temp.type = Type(moveFile.get<uint8_t>());
        temp.effect = Move::Effect(moveFile.get<uint8_t>());
        temp.chanceOfEffect = moveFile.get<uint8_t>();
        temp.intensityOfEffect = moveFile.get<uint8_t>();
        temp.targetIsSelf = bool(moveFile.get<uint8_t>());
        temp.attackerAnim = moveFile.getString();
        temp.defenderAnim = moveFile.getString();
        temp.classification = ReactionPreference::ReactionType(moveFile.get<uint8_t>());
        temp.effectScore = float(moveFile.get<uint8_t>())/100.0;
        moveList[temp.id] = temp;
    }

    File pFile("Resources/Data/Peoplemon/peoplemon.db");
    size = pFile.get<uint16_t>();
    for (int i = 0; i<size; ++i)
    {
        Peoplemon temp;
        temp.id = pFile.get<uint16_t>();
        temp.name = pFile.getString();
        temp.description = pFile.getString();
        temp.type = Type(pFile.get<uint8_t>());
        temp.specialAbilityId = Peoplemon::SpecialAbility(pFile.get<uint8_t>());
        temp.baseStats.hp = pFile.get<uint16_t>();
        temp.baseStats.atk = pFile.get<uint16_t>();
        temp.baseStats.def = pFile.get<uint16_t>();
        temp.baseStats.acc = pFile.get<uint16_t>();
        temp.baseStats.evade = pFile.get<uint16_t>();
        temp.baseStats.spd = pFile.get<uint16_t>();
        temp.baseStats.crit = pFile.get<uint16_t>();
        temp.baseStats.spAtk = pFile.get<uint16_t>();
        temp.baseStats.spDef = pFile.get<uint16_t>();
        int n = pFile.get<uint16_t>();
        for (int j = 0; j<n; ++j)
        {
            int id = pFile.get<uint16_t>();
            temp.validMoves.push_back(id);
        }
        n = pFile.get<uint16_t>();
        for (int j = 0; j<n; ++j)
        {
            int lvl = pFile.get<uint8_t>();
            int id = pFile.get<uint16_t>();
            temp.learnSet.push_back(make_pair(lvl,id));
        }
        temp.evolveLevel = pFile.get<uint8_t>();
        temp.evolveNewId = pFile.get<uint8_t>();
        temp.xpYield = pFile.get<uint16_t>();
        temp.xpGroup = pFile.get<uint8_t>();
        temp.evAward.hp = pFile.get<uint16_t>();
        temp.evAward.atk = pFile.get<uint16_t>();
        temp.evAward.def = pFile.get<uint16_t>();
        temp.evAward.acc = pFile.get<uint16_t>();
        temp.evAward.evade = pFile.get<uint16_t>();
        temp.evAward.spd = pFile.get<uint16_t>();
        temp.evAward.crit = pFile.get<uint16_t>();
        temp.evAward.spAtk = pFile.get<uint16_t>();
        temp.evAward.spDef = pFile.get<uint16_t>();
        temp.numSeen = 0;
        temp.numCaught = 0;
        peoplemonList[temp.id] = temp;
    }

    music.load("menu.plst");

    typeList[0] = "None";
    typeList[1] = "Normal";
    typeList[2] = "Intelligent";
    typeList[3] = "Funny";
    typeList[4] = "Athletic";
    typeList[5] = "Quiet";
    typeList[6] = "Awkward";
    typeList[7] = "Party Animal";
    typeList[8] = "Funny Party Animal";
    typeList[9] = "Athletic and Intelligent??";
    typeList[10] = "Normal and Intelligent";
    typeList[11] = "Normal and Quiet";
    typeList[12] = "Awkward and Funny";
    typeList[13] = "Intelligent and Funny";
    typeList[14] = "Athletic and Normal";
    typeList[15] = "Normal and Funny";
    typeList[16] = "Normal and Awkward";
    typeList[17] = "Quiet and Athletic";
    typeList[18] = "Intelligent and Awkward";
    ailmentList[0] = "None";
    ailmentList[1] = "Burned";
    ailmentList[2] = "Frozen";
    ailmentList[3] = "Confused";
    ailmentList[4] = "Seeded";
    ailmentList[5] = "Paralyzed";
    ailmentList[6] = "Poisoned";

    for (int i = 0; i<7; ++i)
    {
        for (int j = 0; j<18; ++j)
        {
            Peoplemon::typeMultipliers[i][j] = 1;
        }
    }
    Peoplemon::typeMultipliers[1][6] = 0;
    Peoplemon::typeMultipliers[1][12] = 0;
    Peoplemon::typeMultipliers[1][16] = 0;
    Peoplemon::typeMultipliers[1][18] = 0;
    Peoplemon::typeMultipliers[2][2] = 0.5;
    Peoplemon::typeMultipliers[2][3] = 0.5;    Peoplemon::typeMultipliers[2][4] = 2;
    Peoplemon::typeMultipliers[2][8] = 0.5;
    Peoplemon::typeMultipliers[2][10] = 0.5;
    Peoplemon::typeMultipliers[2][12] = 0.5;
    Peoplemon::typeMultipliers[2][13] = 0.25;
    Peoplemon::typeMultipliers[2][14] = 2;
    Peoplemon::typeMultipliers[2][15] = 0.5;
    Peoplemon::typeMultipliers[2][17] = 2;
    Peoplemon::typeMultipliers[2][18] = 0.5;
    Peoplemon::typeMultipliers[3][2] = 2;
    Peoplemon::typeMultipliers[3][4] = 0.5;
    Peoplemon::typeMultipliers[3][5] = 0.5;
    Peoplemon::typeMultipliers[3][6] = 0.5;
    Peoplemon::typeMultipliers[3][7] = 2;
    Peoplemon::typeMultipliers[3][8] = 2;
    Peoplemon::typeMultipliers[3][10] = 2;
    Peoplemon::typeMultipliers[3][11] = 0.5;
    Peoplemon::typeMultipliers[3][12] = 0.5;
    Peoplemon::typeMultipliers[3][13] = 2;
    Peoplemon::typeMultipliers[3][14] = 0.5;
    Peoplemon::typeMultipliers[3][16] = 0.5;
    Peoplemon::typeMultipliers[3][17] = 0.25;
    Peoplemon::typeMultipliers[4][1] = 2;
    Peoplemon::typeMultipliers[4][2] = 0.5;
    Peoplemon::typeMultipliers[4][3] = 2;
    Peoplemon::typeMultipliers[4][4] = 2;
    Peoplemon::typeMultipliers[4][6] = 0;
    Peoplemon::typeMultipliers[4][7] = 0.5;
    Peoplemon::typeMultipliers[4][11] = 2;
    Peoplemon::typeMultipliers[4][12] = 0;
    Peoplemon::typeMultipliers[4][14] = 4;
    Peoplemon::typeMultipliers[4][15] = 4;
    Peoplemon::typeMultipliers[4][16] = 0;
    Peoplemon::typeMultipliers[4][17] = 2;
    Peoplemon::typeMultipliers[4][18] = 0;
    Peoplemon::typeMultipliers[5][6] = 2;
    Peoplemon::typeMultipliers[5][7] = 2;
    Peoplemon::typeMultipliers[5][8] = 2;
    Peoplemon::typeMultipliers[5][12] = 2;
    Peoplemon::typeMultipliers[5][16] = 2;
    Peoplemon::typeMultipliers[5][18] = 2;
    Peoplemon::typeMultipliers[6][1] = 0;
    Peoplemon::typeMultipliers[6][4] = 0;
    Peoplemon::typeMultipliers[6][6] = 2;
    Peoplemon::typeMultipliers[6][7] = 0.5;
    Peoplemon::typeMultipliers[6][8] = 0.5;
    Peoplemon::typeMultipliers[6][9] = 0;
    Peoplemon::typeMultipliers[6][10] = 0;
    Peoplemon::typeMultipliers[6][11] = 0;
    Peoplemon::typeMultipliers[6][12] = 2;
    Peoplemon::typeMultipliers[6][14] = 0;
    Peoplemon::typeMultipliers[6][15] = 0;
    Peoplemon::typeMultipliers[6][16] = 0;
    Peoplemon::typeMultipliers[6][17] = 0;
    Peoplemon::typeMultipliers[6][18] = 2;
    Peoplemon::typeMultipliers[7][3] = 0.5;
    Peoplemon::typeMultipliers[7][4] = 2;
    Peoplemon::typeMultipliers[7][5] = 2;
    Peoplemon::typeMultipliers[7][8] = 0.5;
    Peoplemon::typeMultipliers[7][9] = 2;
    Peoplemon::typeMultipliers[7][11] = 2;
    Peoplemon::typeMultipliers[7][12] = 0.5;
    Peoplemon::typeMultipliers[7][13] = 0.5;
    Peoplemon::typeMultipliers[7][14] = 2;
    Peoplemon::typeMultipliers[7][15] = 0.5;
    Peoplemon::typeMultipliers[7][17] = 4;
}

Game::~Game()
{
    //dtor
}

void Game::start()
{
    mainWindow.create(VideoMode(Properties::ScreenWidth,Properties::ScreenHeight,32), "Peoplemon v"+string(AutoVersion::FULLVERSION_STRING), Style::Titlebar|Style::Close);
	mainWindow.setMouseCursorVisible(false);
	mainWindow.setVerticalSyncEnabled(true);

    Gamestate* state = new MainMenuState(this);
    state->run();
    delete state;
}

bool Game::runState(Gamestate* s, bool delWhenDone)
{
    bool ret = s->run();
    if (delWhenDone)
        delete s;
    return ret;
}

void Game::runStateUnderPriveldged(Gamestate* s, bool delWhenDone)
{
    data.gameClosedFlag = s->run();
    if (delWhenDone)
        delete s;
}

void Game::save()
{
    File output(Properties::GameSavePath+player.getName()+".sav", File::Out);
    ClockTime t = gameClock.getClockTime();
    output.write<uint8_t>(t.hour);
    output.write<uint8_t>(t.minute);
    output.write<uint8_t>(Properties::upKey);
    output.write<uint8_t>(Properties::rightKey);
    output.write<uint8_t>(Properties::downKey);
    output.write<uint8_t>(Properties::leftKey);
    output.write<uint8_t>(Properties::pauseKey);
    output.write<uint8_t>(Properties::interactKey);
    output.write<uint8_t>(Properties::runKey);
    output.write<uint8_t>(data.gameMuted);
    output.write<uint8_t>(data.fullscreen);
    output.write<uint16_t>(peoplemonList.size());
    for (auto i = peoplemonList.begin(); i!=peoplemonList.end(); ++i)
    {
        output.write<uint16_t>(i->second.id);
        output.write<uint16_t>(i->second.numSeen);
        output.write<uint16_t>(i->second.numCaught);
    }
    player.save(&output);
    world.saveGame(&output);
    scriptEnvironment.save(&output);
}

void Game::load(string name)
{
    File input(Properties::GameSavePath+name+".sav");
    ClockTime t;
    t.hour = input.get<uint8_t>();
    t.minute = input.get<uint8_t>();
    Properties::upKey = Keyboard::Key(input.get<uint8_t>());
    Properties::rightKey = Keyboard::Key(input.get<uint8_t>());
    Properties::downKey = Keyboard::Key(input.get<uint8_t>());
    Properties::leftKey = Keyboard::Key(input.get<uint8_t>());
    Properties::pauseKey = Keyboard::Key(input.get<uint8_t>());
    Properties::interactKey = Keyboard::Key(input.get<uint8_t>());
    Properties::runKey = Keyboard::Key(input.get<uint8_t>());
    data.gameMuted = bool(input.get<uint8_t>());
    data.fullscreen = bool(input.get<uint8_t>());
    int s = input.get<uint16_t>();
    for (int i = 0; i<s; ++i)
    {
        int id = input.get<uint16_t>();
        peoplemonList[id].numSeen = input.get<uint16_t>();
        peoplemonList[id].numCaught = input.get<uint16_t>();
    }
    player.load(&input);
    world.loadGame(&input);
    scriptEnvironment.load(&input);
    gameClock.setClockTime(t);
    if (data.fullscreen)
	{
		mainWindow.close();
		mainWindow.create(VideoMode(Properties::ScreenWidth,Properties::ScreenHeight,32), "Peoplemon v"+string(AutoVersion::FULLVERSION_STRING), Style::Fullscreen);
		mainWindow.setMouseCursorVisible(false);
		mainWindow.setVerticalSyncEnabled(true);
	}
}
