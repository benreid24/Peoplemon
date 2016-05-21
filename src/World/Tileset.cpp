#include "World/Tileset.hpp"
#include "Util/File.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

Tileset::Tileset()
{
    //ctor
}

Tileset::Tileset(string file)
{
    load(file);
}

void Tileset::load(string file)
{
	if (file==lastFile)
		return;
	lastFile = file;
    File input(Properties::TilesetPath+file);
    vector<pair<int,string> > files;
    int size, tMax = 0;

    size = input.get<uint16_t>();
    for (int i = 0; i<size; ++i)
    {
        pair<int,string> t;
        t.first = input.get<uint16_t>();
        t.second = input.getString();
        files.push_back(t);
        if (t.first>tMax)
            tMax = t.first;
    }
    textures.resize(tMax+1,TextureReference());
    for (unsigned int i = 0; i<files.size(); ++i)
    {
        textures[files[i].first] = imagePool.loadResource(Properties::MapTilePath+files[i].second);
    }
    files.clear();
    tMax = 0;

    size = input.get<uint16_t>();
    for (int i = 0; i<size; ++i)
    {
        pair<int,string> t;
        t.first = input.get<uint16_t>();
        t.second = input.getString();
        files.push_back(t);
        if (t.first>tMax)
            tMax = t.first;
    }
    animations.resize(tMax+1,AnimationReference(NULL));
    for (unsigned int i = 0; i<files.size(); ++i)
    {
        animations[files[i].first] = animPool.loadResource(Properties::AnimationPath+files[i].second);
    }
}

TextureReference Tileset::getTile(int index)
{
    if (unsigned(index)>=textures.size())
        throw runtime_error("Tile index out of range");
    return textures[index];
}

AnimationReference Tileset::getAnim(int index)
{
    if (unsigned(index)>=animations.size())
        throw runtime_error("Animation index out of range");
    return animations[index];
}

int Tileset::numAnims()
{
	return animations.size();
}
