#include "Globals.hpp"
#include "World/People/Player.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Properties.hpp"
#include <cstdlib>
#include <dirent.h>
#include <sstream>
#include <cstdlib>
using namespace std;
using namespace sf;

#ifdef WINDOWS
const string Properties::GameSavePath = string(getenv("APPDATA"))+"/Peoplemon/";
#endif

const string Properties::MapPath = "Resources/Data/Maps/";
const string Properties::TilesetPath = "Resources/Data/Tilesets/";
const string Properties::PeoplePath = "Resources/Data/People/";
const string Properties::WildPeoplemonPath = "Resources/Data/Peoplemon/Wilds/";
const string Properties::OwnedPeoplemonPath = "Resources/Data/Peoplemon/Owned/";
const string Properties::MapTilePath = "Resources/Media/Images/World/Tiles/";
const string Properties::AnimationPath = "Resources/Data/Animations/World/";
const string Properties::PeoplemonAnimationPath = "Resources/Data/Animations/Peoplemon/";
const string Properties::PeoplemonImagePath = "Resources/Media/Images/Peoplemon/";
const string Properties::BattleImagePath = "Resources/Media/Images/Battle/";
const string Properties::MoveAnimationPath = "Resources/Data/Animations/Moves/";
const string Properties::MiscAnimationPath = "Resources/Data/Animations/Misc/";
const string Properties::SpriteSheetPath = "Resources/Media/Images/SpriteSheets/";
const string Properties::ScriptPath = "Resources/Data/Scripts/";
const string Properties::FontPath = "Resources/Media/Fonts/";
const string Properties::PlaylistPath = "Resources/Data/Playlists/";
const string Properties::MusicPath = "Resources/Media/Music/";
const string Properties::AudioPath = "Resources/Media/Audio/";
const string Properties::ConversationPath = "Resources/Data/People/Conversations/";
const string Properties::TrainerPath = "Resources/Data/People/Trainers/";
const string Properties::NpcPath = "Resources/Data/People/NPCs/";
const string Properties::MenuImagePath = "Resources/Media/Images/Menu/";
const string Properties::CreditsPath = "Resources/Media/Images/Credits/";
Font Properties::PrimaryMenuFont;
Font Properties::SecondaryMenuFont;
Font Properties::ConversationFont;
Font Properties::BattleFont;
Keyboard::Key Properties::upKey = Keyboard::Up;
Keyboard::Key Properties::rightKey = Keyboard::Right;
Keyboard::Key Properties::downKey = Keyboard::Down;
Keyboard::Key Properties::leftKey = Keyboard::Left;
Keyboard::Key Properties::runKey = Keyboard::Space;
Keyboard::Key Properties::interactKey = Keyboard::V;
Keyboard::Key Properties::pauseKey = Keyboard::Return;
const pair<string,string> Peoplemon::abilityTexts[18] = {make_pair("None",""),
                                                        make_pair("Engage", "The opposing Peoplemon cannot switch out"),
                                                        make_pair("Lax", "Cannot receive critical hits"),
                                                        make_pair("Late Riser", "Never wakes up on the first turn it is asleep"),
                                                        make_pair("Observant", "Accuracy cannot be lowered"),
                                                        make_pair("Opinionated", "20% chance to annoy on physical contact with another Peoplemon"),
                                                        make_pair("Forgiving", "Is cured of all ailments when switched out"),
                                                        make_pair("Sassy", "Physical moves cause the attacker to take 1/16th of their health in damage"),
                                                        make_pair("Edumicator", "Intelligent moves done 10% damage more"),
                                                        make_pair("Run Away", "Always able to get away from battle"),
                                                        make_pair("Questionable", "Occasionally finds items when exploring"),
                                                        make_pair("Extroverted", "Twice as likely to encounter wild Peoplemon"),
                                                        make_pair("Introverted", "Half as likely to encounter wild peoplemon"),
                                                        make_pair("Don't Poke", "You shouldn't poke this peoplemon. Also, this ability does nothing"),
                                                        make_pair("Bud", "Copies the enemies ability"),
                                                        make_pair("Share", "Gives all received ailments to the attacker as well"),
                                                        make_pair("Negotiator", "All purchases are 10% cheaper"),
                                                        make_pair("Easy Going", "Cannot be killed by One Hit KO's")};

GameClock gameClock;
PlayerInput user;
ResourcePool<sf::Texture> imagePool;
ResourcePool<sf::SoundBuffer> audioPool;
ResourcePool<AnimationSource> animPool;

int Random(int min, int max)
{
    if (max<min)
        swap(max,min);
    if (max-min+1==0)
        return min;
    return rand()%(max-min+1)+min;
}

void setPlayerControl(Keyboard::Key* k, string c)
{
    if (c=="Z")
        *k = Keyboard::Z;
    if (c=="X")
        *k = Keyboard::X;
    if (c=="C")
        *k = Keyboard::C;
    if (c=="V (Default)")
        *k = Keyboard::V;
    if (c=="B")
        *k = Keyboard::B;
    if (c=="N")
        *k = Keyboard::N;
    if (c=="M")
        *k = Keyboard::M;
    if (c=="L")
        *k = Keyboard::L;
    if (c=="K")
        *k = Keyboard::K;
    if (c=="J")
        *k = Keyboard::J;
}

vector<string> getDirectory(string dir, std::string ext)
{
    DIR* cDir;
    struct dirent* cFile;
    vector<string> total;

    cDir = opendir(dir.c_str());
    if (cDir!=NULL)
    {
        while ((cFile = readdir(cDir)))
        {
            string tmp = cFile->d_name;
            if (tmp.find(".")!=string::npos)
            {
                if (tmp.size()>2 && File::getExtension(tmp)==ext)
                    total.push_back(dir+"/"+tmp);
            }
            else
                total.push_back(tmp);
        }
    }
    return total;
}

string intToString(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

int stringToInt(string s)
{
    stringstream ss;
    ss << s;
    int i = 0;
    ss >> i;
    return i;
}
