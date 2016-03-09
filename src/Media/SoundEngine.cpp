#include "Media/SoundEngine.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
#include <iostream>
using namespace sf;
using namespace std;

SoundEngine::SoundEngine(Game* g) : updater(&SoundEngine::update,this)
{
	game = g;
    lastAssigned = 0;
    running = true;
    updater.launch();
}

SoundEngine::~SoundEngine()
{
    running = false;
    updater.wait();
}

AudioReference SoundEngine::playSound(string file, int loops)
{
    shared_ptr<Audio> t(new Audio());
    t->curPlays = 1;
    t->maxPlays = loops;
    t->ref = audioPool.loadResource(Properties::AudioPath+file);
    t->sound.setBuffer(*t->ref);
    t->sound.setLoop(false);
    if (!game->data.gameMuted)
		t->sound.play();
    lock.lock();
    sounds[lastAssigned+1] = t;
    lock.unlock();
    lastAssigned++;
    return lastAssigned;
}

void SoundEngine::stopSound(AudioReference r)
{
    lock.lock();
    auto i = sounds.find(r);
    if (i!=sounds.end())
    {
        for (unsigned int j = 0; j<fadeOuts.size(); ++j)
        {
            if (fadeOuts[j].first==i->first)
            {
                fadeOuts.erase(fadeOuts.begin()+j);
                j--;
            }
        }
        i->second->sound.stop();
        sounds.erase(i);
    }
    lock.unlock();
}

void SoundEngine::stopAll()
{
    lock.lock();
    for (auto i = sounds.begin(); i!=sounds.end(); ++i)
    {
        i->second->sound.stop();
    }
    sounds.clear();
    fadeOuts.clear();
    lock.unlock();
}

void SoundEngine::fadeOut(AudioReference r)
{
    lock.lock();
    auto i = sounds.find(r);
    if (i!=sounds.end())
        fadeOuts.push_back(make_pair(r,i->second));
    lock.unlock();
}

void SoundEngine::update()
{
    while (running)
    {
        lock.lock();
        startOver:
        for (auto i = sounds.begin(); i!=sounds.end(); ++i)
        {
            if (i->second->sound.getStatus()==Sound::Stopped)
            {
                if (i->second->curPlays<i->second->maxPlays || i->second->maxPlays==-1)
                {
                    i->second->curPlays++;
                    i->second->sound.play();
                }
                else
                {
                    for (unsigned int j = 0; j<fadeOuts.size(); ++j)
                    {
                        if (fadeOuts[j].first==i->first)
                        {
                            fadeOuts.erase(fadeOuts.begin()+j);
                            j--;
                        }
                    }
                    sounds.erase(i);
                    goto startOver;
                }
            }
        }
        for (unsigned int i = 0; i<fadeOuts.size(); ++i)
        {
            if (fadeOuts[i].second->sound.getVolume()<=0)
            {
                sounds.erase(fadeOuts[i].first);
                fadeOuts.erase(fadeOuts.begin()+i);
                i--;
            }
            else
            {
                int v = fadeOuts[i].second->sound.getVolume()-20;
                if (v<0)
                    v = 0;
                fadeOuts[i].second->sound.setVolume(v);
            }
        }
        lock.unlock();

        if (game->data.gameMuted)
			stopAll();

        sleep(milliseconds(150));
    }
}
