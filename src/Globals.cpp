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

#ifdef Windows
const string Properties::GameSavePath = string(getenv("APPDATA"))+"/Peoplemon/";
#endif
#ifdef Linux
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
const string Properties::GameSavePath = string(getpwuid(getuid())->pw_dir)+"/.config/Peoplemon/";
#endif

#ifdef OSX
#include "ResourcePath.hpp"
const string Properties::GameSavePath = "/Users/"+string(getenv("USER"))+"/Library/Application Support/Peoplemon/";
const string Properties::ResourcesPath = resourcePath();
const string Properties::MapPath = resourcePath()+"Data/Maps/";
const string Properties::DataPath = resourcePath()+"Data/";
const string Properties::TilesetPath = resourcePath()+"Data/Tilesets/";
const string Properties::PeoplePath = resourcePath()+"Data/People/";
const string Properties::PeoplemonBasePath = resourcePath()+"Data/Peoplemon/";
const string Properties::WildPeoplemonPath = resourcePath()+"Data/Peoplemon/Wilds/";
const string Properties::OwnedPeoplemonPath = resourcePath()+"Data/Peoplemon/Owned/";
const string Properties::MapTilePath = resourcePath()+"Media/Images/World/Tiles/";
const string Properties::AnimationPath = resourcePath()+"Data/Animations/World/";
const string Properties::PeoplemonAnimationPath = resourcePath()+"Data/Animations/Peoplemon/";
const string Properties::PeoplemonImagePath = resourcePath()+"Media/Images/Peoplemon/";
const string Properties::BattleImagePath = resourcePath()+"Media/Images/Battle/";
const string Properties::MoveAnimationPath = resourcePath()+"Data/Animations/Moves/";
const string Properties::MiscAnimationPath = resourcePath()+"Data/Animations/Misc/";
const string Properties::MiscImagePath = resourcePath()+"Media/Images/World/Misc/";
const string Properties::SpriteSheetPath = resourcePath()+"Media/Images/SpriteSheets/";
const string Properties::ScriptPath = resourcePath()+"Data/Scripts/";
const string Properties::FontPath = resourcePath()+"Media/Fonts/";
const string Properties::PlaylistPath = resourcePath()+"Data/Playlists/";
const string Properties::MusicPath = resourcePath()+"Media/Music/";
const string Properties::AudioPath = resourcePath()+"Media/Audio/";
const string Properties::ConversationPath = resourcePath()+"Data/People/Conversations/";
const string Properties::TrainerPath = resourcePath()+"Data/People/Trainers/";
const string Properties::NpcPath = resourcePath()+"Data/People/NPCs/";
const string Properties::MenuImagePath = resourcePath()+"Media/Images/Menu/";
const string Properties::CreditsPath = resourcePath()+"Media/Images/Credits/";

#else
const string Properties::ResourcesPath = "Resources/";
const string Properties::MapPath = "Resources/Data/Maps/";
const string Properties::DataPath = "Resources/Data/";
const string Properties::TilesetPath = "Resources/Data/Tilesets/";
const string Properties::PeoplePath = "Resources/Data/People/";
const string Properties::PeoplemonBasePath = "Resources/Data/Peoplemon/";
const string Properties::WildPeoplemonPath = "Resources/Data/Peoplemon/Wilds/";
const string Properties::OwnedPeoplemonPath = "Resources/Data/Peoplemon/Owned/";
const string Properties::MapTilePath = "Resources/Media/Images/World/Tiles/";
const string Properties::AnimationPath = "Resources/Data/Animations/World/";
const string Properties::PeoplemonAnimationPath = "Resources/Data/Animations/Peoplemon/";
const string Properties::PeoplemonImagePath = "Resources/Media/Images/Peoplemon/";
const string Properties::BattleImagePath = "Resources/Media/Images/Battle/";
const string Properties::MoveAnimationPath = "Resources/Data/Animations/Moves/";
const string Properties::MiscAnimationPath = "Resources/Data/Animations/Misc/";
const string Properties::MiscImagePath = "Resources/Media/Images/World/Misc/";
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
#endif

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

string keyToString(Keyboard::Key key)
{
	for (unsigned int i = 0; i<keymap.size(); ++i)
	{
		if (keymap[i].first==key)
			return keymap[i].second;
	}
	return "Unknown";
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
					total.push_back(dir+tmp);
            }
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

VideoMode getBestVideoMode()
{
	vector<VideoMode> modes = VideoMode::getFullscreenModes();
	unsigned int hX = 0, hI = -1;

	for (unsigned int i = 0; i<modes.size(); ++i)
	{
		if (modes[i].width>hX)
		{
            hX = modes[i].width;
            hI = i;
		}
	}

	return modes[hI];
}

View getView(float w, float h)
{
	View view(FloatRect(0,0,Properties::ScreenWidth,Properties::ScreenHeight));
	float newWidth = w;
	float newHeight = h;
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
	return view;
}

vector<pair<Keyboard::Key,string> > keymap({make_pair(Keyboard::A,"A"),
											make_pair(Keyboard::B,"B"),
											make_pair(Keyboard::C,"C"),
											make_pair(Keyboard::D,"D"),
											make_pair(Keyboard::E,"E"),
											make_pair(Keyboard::F,"F"),
											make_pair(Keyboard::G,"G"),
											make_pair(Keyboard::H,"H"),
											make_pair(Keyboard::I,"I"),
											make_pair(Keyboard::J,"J"),
											make_pair(Keyboard::K,"K"),
											make_pair(Keyboard::L,"L"),
											make_pair(Keyboard::M,"M"),
											make_pair(Keyboard::N,"N"),
											make_pair(Keyboard::O,"O"),
											make_pair(Keyboard::P,"P"),
											make_pair(Keyboard::Q,"Q"),
											make_pair(Keyboard::R,"R"),
											make_pair(Keyboard::S,"S"),
											make_pair(Keyboard::T,"T"),
											make_pair(Keyboard::U,"U"),
											make_pair(Keyboard::V,"V"),
											make_pair(Keyboard::W,"W"),
											make_pair(Keyboard::X,"X"),
											make_pair(Keyboard::Y,"Y"),
											make_pair(Keyboard::Z,"Z"),
											make_pair(Keyboard::Num0,"0"),
											make_pair(Keyboard::Num1,"1"),
											make_pair(Keyboard::Num2,"2"),
											make_pair(Keyboard::Num3,"3"),
											make_pair(Keyboard::Num4,"4"),
											make_pair(Keyboard::Num5,"5"),
											make_pair(Keyboard::Num6,"6"),
											make_pair(Keyboard::Num7,"7"),
											make_pair(Keyboard::Num8,"8"),
											make_pair(Keyboard::Num9,"9"),
											make_pair(Keyboard::Escape,"ESC"),
											make_pair(Keyboard::LControl,"LControl"),
											make_pair(Keyboard::LShift,"LShift"),
											make_pair(Keyboard::LAlt,"LAlt"),
											make_pair(Keyboard::LSystem,"LSystem"),
											make_pair(Keyboard::RControl,"RControl"),
											make_pair(Keyboard::RShift,"RShift"),
											make_pair(Keyboard::RAlt,"RAlt"),
											make_pair(Keyboard::RSystem,"RSystem"),
											make_pair(Keyboard::Menu,"Menu"),
											make_pair(Keyboard::LBracket,"LBracket"),
											make_pair(Keyboard::RBracket,"RBracket"),
											make_pair(Keyboard::SemiColon,"SemiColon"),
											make_pair(Keyboard::Comma,"Comma"),
											make_pair(Keyboard::Period,"Period"),
											make_pair(Keyboard::Quote,"Quote"),
											make_pair(Keyboard::Slash,"Slash"),
											make_pair(Keyboard::BackSlash,"Backslash"),
											make_pair(Keyboard::Tilde,"Tilde"),
											make_pair(Keyboard::Equal,"Equals"),
											make_pair(Keyboard::Dash,"Dash"),
											make_pair(Keyboard::Space,"Space"),
											make_pair(Keyboard::Return,"Enter"),
											make_pair(Keyboard::BackSpace,"Backspace"),
											make_pair(Keyboard::Tab,"Tab"),
											make_pair(Keyboard::PageUp,"PageUp"),
											make_pair(Keyboard::PageDown,"PageDown"),
											make_pair(Keyboard::End,"End"),
											make_pair(Keyboard::Home,"Home"),
											make_pair(Keyboard::Insert,"Insert"),
											make_pair(Keyboard::Delete,"Delete"),
											make_pair(Keyboard::Add,"Add"),
											make_pair(Keyboard::Subtract,"Subtract"),
											make_pair(Keyboard::Multiply,"Multiply"),
											make_pair(Keyboard::Divide,"Divide"),
											make_pair(Keyboard::Left,"Left Arrow"),
											make_pair(Keyboard::Right,"Right Arrow"),
											make_pair(Keyboard::Up,"Up Arrow"),
											make_pair(Keyboard::Down,"Down Arrow"),
											make_pair(Keyboard::Numpad0,"Pad 0"),
											make_pair(Keyboard::Numpad1,"Pad 1"),
											make_pair(Keyboard::Numpad2,"Pad 2"),
											make_pair(Keyboard::Numpad3,"Pad 3"),
											make_pair(Keyboard::Numpad4,"Pad 4"),
											make_pair(Keyboard::Numpad5,"Pad 5"),
											make_pair(Keyboard::Numpad6,"Pad 6"),
											make_pair(Keyboard::Numpad7,"Pad 7"),
											make_pair(Keyboard::Numpad8,"Pad 8"),
											make_pair(Keyboard::Numpad9,"Pad 9"),
											make_pair(Keyboard::F1,"F1"),
											make_pair(Keyboard::F2,"F2"),
											make_pair(Keyboard::F3,"F3"),
											make_pair(Keyboard::F4,"F4"),
											make_pair(Keyboard::F5,"F5"),
											make_pair(Keyboard::F6,"F6"),
											make_pair(Keyboard::F7,"F7"),
											make_pair(Keyboard::F8,"F8"),
											make_pair(Keyboard::F9,"F9"),
											make_pair(Keyboard::F10,"F10"),
											make_pair(Keyboard::F11,"F11"),
											make_pair(Keyboard::F12,"F12"),
											make_pair(Keyboard::F13,"F13"),
											make_pair(Keyboard::F14,"F14"),
											make_pair(Keyboard::F15,"F15"),
											make_pair(Keyboard::Pause,"Pause")});
