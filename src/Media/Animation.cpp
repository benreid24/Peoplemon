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
    if (FileExists(tempStr+file))
		tempStr += file;
	else
		tempStr = Properties::SpriteSheetPath+file;
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
    playing = false;
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

	update();

    std::vector<Sprite>& t = animSrc->getFrame(curFrm, position);
    for (unsigned int i = 0; i<t.size(); ++i)
		window->draw(t[i]);
}

AnimationReference AnimationSource::generateCaptureAnimation(AnimationSource& still)
{
    AnimationReference capture(new AnimationSource());
    capture->sheet = still.sheet;
    capture->loop = false;

    if (still.frames.size() < 1)
    {
        cout << "Warning: Attempted to generate Capture animation from a Still containing " << still.frames.size() << " frames!\n";
        return capture;
    }
    if (still.frames[0].size() != 1)
    {
        cout << "Warning: Attempted to generate Capture animation from a Still frame containing " << still.frames[0].size() << " pieces!\n";
        return capture;
    }

    AnimationFrame firstFrame = still.frames[0][0];
    firstFrame.length = 310;

    AnimationFrame lastFrame = firstFrame;
    lastFrame.alpha = 0;
    lastFrame.scaleX = lastFrame.scaleY = 0;
    lastFrame.renderOffset = Vector2f(621, 103);

    Vector2f origin, endOrigin = lastFrame.renderOffset;
    origin.x = firstFrame.renderOffset.x + firstFrame.size.x/2;
    origin.y = firstFrame.renderOffset.y + firstFrame.size.y/2;

    vector<AnimationFrame> frames = AnimationSource::interpolateFrames(firstFrame, lastFrame, origin, endOrigin, 340, 10);
    for (unsigned int i = 0; i<frames.size(); ++i) {
        capture->frames.push_back({frames[i]});
    }

    return capture;
}

AnimationReference AnimationSource::generateBreakoutAnimation(AnimationSource& still)
{
    AnimationReference capture(new AnimationSource());
    capture->sheet = still.sheet;
    capture->loop = false;

    if (still.frames.size() < 1)
    {
        cout << "Warning: Attempted to generate Breakout animation from a Still containing " << still.frames.size() << " frames!\n";
        return capture;
    }
    if (still.frames[0].size() != 1)
    {
        cout << "Warning: Attempted to generate Breakout animation from a Still frame containing " << still.frames[0].size() << " pieces!\n";
        return capture;
    }

    AnimationFrame lastFrame = still.frames[0][0];

    AnimationFrame firstFrame = lastFrame;
    firstFrame.length = 310;
    firstFrame.alpha = 0;
    firstFrame.scaleX = firstFrame.scaleY = 0;
    firstFrame.renderOffset = Vector2f(623, 188);

    Vector2f origin = firstFrame.renderOffset, endOrigin;
    endOrigin.x = lastFrame.renderOffset.x + lastFrame.size.x/2;
    endOrigin.y = lastFrame.renderOffset.y + lastFrame.size.y/2;

    vector<AnimationFrame> frames = AnimationSource::interpolateFrames(firstFrame, lastFrame, origin, endOrigin, 250, 10);
    for (unsigned int i = 0; i<frames.size(); ++i) {
        capture->frames.push_back({frames[i]});
    }

    return capture;
}

vector<AnimationFrame> AnimationSource::interpolateFrames(AnimationFrame first, AnimationFrame last, Vector2f origin, Vector2f endOrigin, int totalLength, int frameLen)
{
    vector<AnimationFrame> frames = {first};

    AnimationFrame frame = first;
    frame.length = frameLen;
    int frameCount = totalLength/frameLen;

    Vector2f originDiff = endOrigin - origin;
    originDiff.x /= frameCount;
    originDiff.y /= frameCount;

    double alphaDiff = (last.alpha-frame.alpha)/frameCount;
    double xScaleDiff = (last.scaleX-frame.scaleX)/frameCount;
    double yScaleDiff = (last.scaleY-frame.scaleY)/frameCount;

    for (int i = 0; i<frameCount; ++i)
    {
        frame.scaleX += xScaleDiff;
        frame.scaleY += yScaleDiff;
        frame.alpha += alphaDiff;
        origin += originDiff;

        frame.renderOffset.x = origin.x - frame.size.x/2;
        frame.renderOffset.y = origin.y - frame.size.y/2;

        frames.push_back(frame);
    }

    frames.push_back(last);
    return frames;
}
