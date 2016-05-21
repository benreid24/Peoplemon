#include "Media/Animation.hpp"
#include "Util/File.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
#include "Resources/ResourcePool.hpp"
#include <iostream>
using namespace std;
using namespace sf;

AnimationSource::AnimationSource()
{
    loop = true;
}

AnimationSource::AnimationSource(string file)
{
    load(file);
}

AnimationSource::~AnimationSource()
{
    //dtor
}

void AnimationSource::load(string file)
{
    File input(file);
    AnimationFrame temp;
    int maxL = 0;
    string tempStr = File::getPath(file);

    file = input.getString();
    if (FileExists(Properties::SpriteSheetPath+file))
		tempStr = Properties::SpriteSheetPath+file;
	else
		tempStr += file;
    sheet = imagePool.loadResource(tempStr);
    loop = bool(input.get<uint8_t>());
    int numFrames = input.get<uint16_t>();
    frames.resize(numFrames);
    for (int i = 0; i<numFrames; ++i)
    {
        temp.length = input.get<uint32_t>();
        int n = input.get<uint16_t>();
        for (int j = 0; j<n; ++j)
        {
        	temp.sourcePos.x = input.get<uint32_t>();
			temp.sourcePos.y = input.get<uint32_t>();
			temp.size.x = input.get<uint32_t>();
			temp.size.y = input.get<uint32_t>();
			temp.scaleX = double(input.get<uint32_t>())/100;
			temp.scaleY = double(input.get<uint32_t>())/100;
			temp.renderOffset.x = input.get<int32_t>();
			temp.renderOffset.y = input.get<int32_t>();
			temp.rotation = input.get<uint32_t>();
			temp.alpha = input.get<uint8_t>();
			frames[i].push_back(temp);
        }
        if (n>maxL)
			maxL = n;
    }
    sprites.reserve(maxL);
}

bool AnimationSource::isLooping()
{
    return loop;
}

std::vector<sf::Sprite>& AnimationSource::getFrame(int i, Vector2f pos)
{
    if (i<0 || unsigned(i)>=frames.size())
        return sprites;

	sprites.resize(frames[i].size());
	for (unsigned int j = 0; j<frames[i].size(); ++j)
    {
    	Sprite sp;
    	sp.setTexture(*sheet,true);
    	sp.setTextureRect(IntRect(frames[i][j].sourcePos.x, frames[i][j].sourcePos.y, frames[i][j].size.x, frames[i][j].size.y));
		sp.setOrigin(frames[i][j].size.x/2,frames[i][j].size.y/2);
		sp.setRotation(frames[i][j].rotation);
		sp.setPosition(pos+frames[i][j].renderOffset+Vector2f(frames[i][j].size.x/2,frames[i][j].size.y/2));
		sp.setColor(Color(255,255,255,frames[i][j].alpha));
		sp.setScale(frames[i][j].scaleX,frames[i][j].scaleY);
		sprites[j] = sp;
    }
    return sprites;
}

int AnimationSource::incFrame(int cFrm, int lTime)
{
    if (cFrm<0 || unsigned(cFrm)>=frames.size())
        return 0;

	if (frames[cFrm].size()==0) //current frame is empty, go to next
	{
		if (unsigned(cFrm+1)<frames.size())
			return cFrm+1;
		return cFrm;
	}

    if (gameClock.getTimeStamp()-lTime>=frames[cFrm][0].length)
    {
        if (unsigned(cFrm+1)<frames.size())
            return cFrm+1;
        else if (loop)
            return 0;
        else
            return cFrm;
    }

    return cFrm;
}

int AnimationSource::numFrames()
{
    return frames.size();
}

Animation::Animation()
{
    curFrm = lastFrmChangeTime = 0;
    playing = false;
}

Animation::Animation(AnimationReference ref) : Animation()
{
    animSrc = ref;
}

Animation::~Animation()
{
    //dtor
}

void Animation::setSource(AnimationReference src)
{
    animSrc = src;
    curFrm = 0;
    lastFrmChangeTime = gameClock.getTimeStamp();
}

void Animation::update()
{
    if (!animSrc)
        return;

    int t = curFrm;
    if (playing || animSrc->isLooping())
        curFrm = animSrc->incFrame(curFrm,lastFrmChangeTime);
    if (t!=curFrm)
        lastFrmChangeTime = gameClock.getTimeStamp();

    if (playing && curFrm==animSrc->numFrames())
    {
        playing = false;
        setFrame(0);
    }
}

void Animation::setFrame(int frm)
{
    curFrm = frm;
    lastFrmChangeTime = gameClock.getTimeStamp();
}

bool Animation::finished()
{
    if (!animSrc)
        return false;

    return (!animSrc->isLooping() && curFrm==animSrc->numFrames()-1) || animSrc->numFrames()==1;
}

bool Animation::isLooping()
{
    if (!animSrc)
        return false;

    return animSrc->isLooping();
}

void Animation::play()
{
    if (!animSrc)
        return;

    if (!animSrc->isLooping())
    {
        setFrame(0);
        playing = true;
    }
}

void Animation::setPosition(Vector2f pos)
{
    position = pos;
}

void Animation::draw(sf::RenderWindow* window)
{
    if (!animSrc)
        return;

    std::vector<Sprite>& t = animSrc->getFrame(curFrm, position);
    for (unsigned int i = 0; i<t.size(); ++i)
		window->draw(t[i]);
}
