#include "Media/Playlist.hpp"
#include "Util/File.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
using namespace sf;
using namespace std;

Playlist::Playlist(Game* g)
{
	game = g;
    curSong = 0;
    started = false;
    wasMutedLast = game->data.gameMuted;
}

Playlist::~Playlist()
{
    if (started && audio.getStatus()==Music::Playing)
	{
		while (audio.getVolume()>2)
		{
			audio.setVolume(audio.getVolume()-1);
			sleep(milliseconds(5));
		}
	}
}

void Playlist::load(string file, bool savePrev)
{
	if (file==curList)
		return;
	stop();

    prevList = "";
    if (savePrev)
        prevList = curList;
    curList = file;

    File input(Properties::PlaylistPath+file);
    int size = input.get<uint16_t>();

    audio.stop();
    songs.clear();
    curSong = 0;
    started = false;
    songs.resize(size);
    for (int i = 0; i<size; ++i)
        songs[i] = input.getString();
    if (songs.size()>0)
        audio.openFromFile(Properties::MusicPath+songs[0]);
    audio.setLoop(false);

    vector<int> temp;
    for (unsigned int i = 0; i<songs.size(); ++i)
    {
        temp.push_back(i);
    }
    order.clear();
    while (temp.size()!=0)
    {
        int i = Random(0,temp.size()-1);
        order.push_back(temp[i]);
        temp.erase(temp.begin()+i);
    }
}

void Playlist::play()
{
    if (songs.size()>0 && !game->data.gameMuted && !started)
    {
        audio.play();
        started = true;
    }
    else if (game->data.gameMuted)
		started = true;
}

void Playlist::stop()
{
	if (started && audio.getStatus()==Music::Playing)
	{
		while (audio.getVolume()>2)
		{
			audio.setVolume(audio.getVolume()-1);
			sleep(milliseconds(3));
		}
	}
    audio.stop();
    audio.setVolume(100);
    started = false;
}

void Playlist::update()
{
    if (audio.getStatus()==Music::Stopped && started && songs.size()>0 && !game->data.gameMuted)
    {
        curSong++;
        if (unsigned(curSong)>=order.size())
            curSong = 0;
        audio.openFromFile(Properties::MusicPath+songs[order[curSong]]);
        audio.play();
    }
    if (wasMutedLast && !game->data.gameMuted && started)
		play();
	if (game->data.gameMuted)
		audio.pause();
	wasMutedLast = game->data.gameMuted;
}

void Playlist::previous()
{
    if (prevList.size()>0)
        load(prevList);
    else
        stop();
}
