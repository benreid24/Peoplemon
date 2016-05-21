#include "World/Weather.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
using namespace std;
using namespace sf;

void BaseWeatherType::update()
{
    //do nothing
}

bool BaseWeatherType::isFinished()
{
    return true;
}

void BaseWeatherType::stop()
{
    //do nothing
}

void BaseWeatherType::draw(RenderWindow* w)
{
    //do nothing
}

int BaseWeatherType::getLightChange()
{
    return 0;
}

Thunder::Thunder() : flash(Vector2f(800,600))
{
    goTime = 0;
    lastTime = 0;
    flash.setFillColor(Color::Transparent);
}

void Thunder::update(Game* g)
{
    if (goTime!=0)
    {
        int t = gameClock.getTimeStamp()-goTime;
        const int a = 70, c = 400, d = 750;
        int alpha = 0;

        if (t>=d)
            goTime = 0;
        else
        {
            if (t<175)
                alpha = -100*t*(t-2*2)/(a*a);
            else
                alpha = 230*(t-d)*(t-2*c+d)/(-c*c+2*c*d-d*d);
        }
        flash.setFillColor(Color(255,255,255,alpha));
    }
    else if (Random(10000,25000)<gameClock.getTimeStamp()-lastTime)
    {
        g->soundEngine.playSound("thunder.wav");
        flash.setFillColor(Color(255,255,255,128));
        goTime = gameClock.getTimeStamp();
        lastTime = gameClock.getTimeStamp();
    }
}

void Thunder::draw(RenderWindow* window)
{
    window->draw(flash);
}

RainWeather::RainWeather(Game* g, bool isHard, bool thunder)
{
    isStopping = isDone = false;
    _g = g;
    lastTime = 0;
    creationTimer = gameClock.getTimeStamp();
    canThunder = thunder;
    airDrop = imagePool.loadResource(Properties::MiscImagePath+"rainDrop.png");
    splashDrop[0] = imagePool.loadResource(Properties::MiscImagePath+"rainSplash1.png");
    splashDrop[1] = imagePool.loadResource(Properties::MiscImagePath+"rainSplash2.png");
    airSpr.setTexture(*airDrop);
    splashSpr[0].setTexture(*splashDrop[0]);
    splashSpr[1].setTexture(*splashDrop[1]);
    airSpr.setOrigin(Vector2f(airDrop->getSize().x/2,airDrop->getSize().y));
    splashSpr[0].setOrigin(Vector2f(splashDrop[0]->getSize().x/2,splashDrop[0]->getSize().y));
    splashSpr[1].setOrigin(Vector2f(splashDrop[1]->getSize().x/2,splashDrop[1]->getSize().y));

    if (isHard)
    {
        sound = g->soundEngine.playSound("hardRain.wav",-1);
        airSpr.setRotation(-45);
        splashSpr[0].setRotation(-45);
        rainVel = Vector3f(0.030303,0.030303,0.757575);
        maxDrops = 1200;
        lChng = 35;
    }
    else
    {
        sound = g->soundEngine.playSound("lightRain.wav",-1);
        airSpr.setRotation(-15);
        splashSpr[1].setRotation(-15);
        splashSpr[0].setRotation(-15);
        rainVel = Vector3f(0,0,0.515151);
        maxDrops = 750;
        lChng = 22;
    }
    createPerSec = 0.303;
    rainDrops.reserve(maxDrops+10);
}

RainWeather::~RainWeather()
{
    _g->soundEngine.stopSound(sound);
}

void RainWeather::stop()
{
    isStopping = true;
    _g->soundEngine.fadeOut(sound);
}

bool RainWeather::isFinished()
{
    return isDone;
}

int RainWeather::getLightChange()
{
    return lChng;
}

void RainWeather::update()
{
    if (isDone)
        return;

	double dt = gameClock.getTimeStamp()-lastTime;
	Vector3f dif(rainVel.x*dt,rainVel.y*dt,rainVel.z*dt);
	lastTime = gameClock.getTimeStamp();

    for (unsigned int i = 0; i<rainDrops.size(); ++i)
    {
        if (rainDrops[i].z!=0)
        {
            rainDrops[i] += dif;
            if (rainDrops[i].z<=0)
                rainDrops[i].z = 0;
        }
        if (rainDrops[i].z==0)
        {
            if (rainDrops[i].time==0)
                rainDrops[i].time = gameClock.getTimeStamp();
            else if (gameClock.getTimeStamp()-rainDrops[i].time>=80 && !isStopping)
            {
                int xMin = _g->world.getCamera().x-300;
                int yMin = _g->world.getCamera().y-300;
                rainDrops[i] = Vector4D(Random(xMin, xMin+1400),Random(yMin,yMin+1200),Random(50,90),0);
            }
            else if (isStopping)
            {
                rainDrops.erase(rainDrops.begin()+i);
                i--;
            }
        }
    }

	if (gameClock.getTimeStamp()-creationTimer>100)
    {
    	int created = 0;
		int makeNow = double(gameClock.getTimeStamp()-creationTimer)*createPerSec;
		creationTimer = gameClock.getTimeStamp();
		while (signed(rainDrops.size())<maxDrops && created<makeNow)
		{
			int xMin = _g->world.getCamera().x-300;
			int yMin = _g->world.getCamera().y-300;
			rainDrops.push_back(Vector4D(Random(xMin, xMin+1400),Random(yMin,yMin+1200),Random(50,90),0));
			created++;
		}
    }

    if (canThunder)
        thunder.update(_g);

    if (isStopping)
    {
        maxDrops -= 10;
        if (rainDrops.size()==0)
            isDone = true;
    }
}

void RainWeather::draw(RenderWindow* window)
{
    for (unsigned int i = 0; i<rainDrops.size(); ++i)
    {
        if (rainDrops[i].z>0)
        {
            airSpr.setPosition(rainDrops[i].toScreen(_g->world.getCamera()));
            window->draw(airSpr);
        }
        else
        {
            int x = (gameClock.getTimeStamp()-rainDrops[i].time)/60;
            if (x>1)
                x = 1;
            splashSpr[x].setPosition(rainDrops[i].toScreen(_g->world.getCamera()));
            window->draw(splashSpr[x]);
        }
    }
    if (canThunder)
        thunder.draw(window);
}

SnowWeather::SnowWeather(Game* g, bool h, bool t)
{
    isStopping = isDone = false;
    lastTime = 0;
    creationTimer = gameClock.getTimeStamp();
    _g = g;
    flake = imagePool.loadResource(Properties::MiscImagePath+"snowFlake.png");
    spr.setTexture(*flake);
    canThunder = t;
    createPerSec = 0.364;

    if (h)
    {
        fallSpeed = 1.212121;
        maxFlakes = 10000;
        lChng = 30;
        lifeTime = 2000;
    }
    else
    {
        fallSpeed = 0.121212;
        maxFlakes = 6000;
        lChng = 15;
        lifeTime = 4000;
    }
    flakes.reserve(maxFlakes+10);
}

SnowWeather::~SnowWeather()
{
    //dtor
}

bool SnowWeather::isFinished()
{
    return isDone;
}

void SnowWeather::stop()
{
    isStopping = true;
}

int SnowWeather::getLightChange()
{
    return lChng;
}

void SnowWeather::update()
{
    if (isDone)
        return;

	double dif = double(gameClock.getTimeStamp()-lastTime)*fallSpeed;
	lastTime = gameClock.getTimeStamp();

    for (unsigned int i = 0; i<flakes.size(); ++i)
    {
        if (flakes[i].z!=0)
        {
            flakes[i].z -= dif;
            if (flakes[i].z<=0)
            {
                flakes[i].z = 0;
                flakes[i].time = gameClock.getTimeStamp();
            }
            else
            {
                if (fallSpeed<1) //light
                {
                    flakes[i].x += cos(flakes[i].z*3.1415926/60)*6;
                    flakes[i].y += 2;
                }
                else //hard
                {
                    flakes[i].x += dif/2 + cos(flakes[i].z*3.1415926/60)*12;
                    flakes[i].y += dif/4;
                }
            }
        }
        else
        {
            if (gameClock.getTimeStamp()-flakes[i].time>lifeTime)
            {
            	if (!isStopping)
                {
                	int xMin = _g->world.getCamera().x-300;
					int yMin = _g->world.getCamera().y-300;
					flakes[i] = Vector4D(Random(xMin, xMin+1400),Random(yMin,yMin+1200),Random(50,90),0);
                }
                else if (isStopping)
				{
					flakes.erase(flakes.begin()+i);
					i--;
				}
            }
        }
    }

	if (gameClock.getTimeStamp()-creationTimer>100)
    {
    	int createNow = double(gameClock.getTimeStamp()-creationTimer)*createPerSec;
    	int created = 0;
    	creationTimer = gameClock.getTimeStamp();
		while (signed(flakes.size())<maxFlakes && created<createNow)
		{
			int xMin = _g->world.getCamera().x-300;
			int yMin = _g->world.getCamera().y-300;
			flakes.push_back(Vector4D(Random(xMin, xMin+1400),Random(yMin,yMin+1200),Random(50,90),0));
			created++;
		}
    }

    if (canThunder)
        thunder.update(_g);
    if (isStopping)
    {
        maxFlakes -= 80;
        if (flakes.size()==0)
            isDone = true;
    }
}

void SnowWeather::draw(RenderWindow* window)
{
    for (unsigned int i = 0; i<flakes.size(); ++i)
    {
        spr.setPosition(flakes[i].toScreen(_g->world.getCamera()));
        if (flakes[i].time!=0)
            spr.setColor(Color(255,255,255,255-255*(double(gameClock.getTimeStamp()-flakes[i].time)/double(lifeTime))));
        else
            spr.setColor(Color(255,255,255,255));
        window->draw(spr);
    }

    if (canThunder)
        thunder.draw(window);
}

SunnyWeather::SunnyWeather() : cover(Vector2f(800,600))
{
    cover.setFillColor(Color(255,255,60,45));
    t = 0;
    a = 0;
    isStopping = false;
    lastTime = 0;
}

SunnyWeather::~SunnyWeather()
{
    //dtor
}

void SunnyWeather::stop()
{
    isStopping = true;
    a = cover.getFillColor().a;
}

bool SunnyWeather::isFinished()
{
    return cover.getFillColor().a==0;
}

void SunnyWeather::update()
{
    if (!isStopping)
	{
		t += double(gameClock.getTimeStamp()-lastTime)*0.030303;
		cover.setFillColor(Color(255,255,60,45+cos(double(t)/45*3.1415926)*25));
	}
    else
	{
		a -= 0.0151515*double(gameClock.getTimeStamp()-lastTime);
		if (a<0)
			a = 0;
		cover.setFillColor(Color(255,255,60,a));
	}
	lastTime = gameClock.getTimeStamp();
}

void SunnyWeather::draw(RenderWindow* window)
{
    window->draw(cover);
}

int SunnyWeather::getLightChange()
{
    return -20;
}

FogWeather::FogWeather(Game* g, bool isThick)
{
	a = 0;
    _g = g;
    isStopping = false;
    lastTime = 0;
    spr.setColor(Color(255,255,255,0)); //gradually increase
    fogTxtr = imagePool.loadResource(Properties::MiscImagePath+"fog.png");
    spr.setTexture(*fogTxtr);
    spr.setOrigin(fogTxtr->getSize().x/2-4,fogTxtr->getSize().y/2-4);

    int baseX = g->world.getCamera().x-128;
    int baseY = g->world.getCamera().y-128;
    for (int x = 0; x<70; x++)
    {
        for (int y = 0; y<60; y++)
        {
            particles.push_back(Particle(baseX+x*26,baseY+y*26,Random(0,360),double(Random(-5,5))/33));
        }
    }

    if (isThick)
    {
        targetA = 110;
        lChng = 50;
    }
    else
    {
        targetA = 70;
        lChng = 20;
    }
}

FogWeather::~FogWeather()
{
    //dtor
}

void FogWeather::stop()
{
    isStopping = true;
}

bool FogWeather::isFinished()
{
    return spr.getColor().a==0;
}

int FogWeather::getLightChange()
{
    return lChng;
}

void FogWeather::update()
{
	double dt = gameClock.getTimeStamp()-lastTime;
	double difY = dt*0.030303;
	double difX = difY*2;

    for (unsigned int i = 0; i<particles.size(); ++i)
    {
        particles[i].x += difX;
        particles[i].y += difY;
        particles[i].rotation += particles[i].angularVel*dt;

        if (particles[i].x>_g->world.getCamera().x+1680)
            particles[i] = Particle(_g->world.getCamera().x-96,particles[i].y,Random(0,360),double(Random(-5,5))/33);
        if (particles[i].y>_g->world.getCamera().y+1440)
            particles[i] = Particle(particles[i].x,_g->world.getCamera().y-96,Random(0,360),double(Random(-5,5))/33);
    }

    if (a<targetA && !isStopping)
	{
		a += dt*0.060606;
		spr.setColor(Color(255,255,255,a));
	}

    if (isStopping && a>0)
    {
        a -= dt*0.060606;
        if (a<0)
            a = 0;
        spr.setColor(Color(255,255,255,a));
    }
}

void FogWeather::draw(RenderWindow* window)
{
    for (unsigned int i = 0; i<particles.size(); ++i)
    {
        spr.setPosition(particles[i].x-_g->world.getCamera().x,particles[i].y-_g->world.getCamera().y);
        spr.setRotation(particles[i].rotation);
        window->draw(spr);
    }
}

SandstormWeather::SandstormWeather(Game* g) : cover(Vector2f(800,600))
{
    _g = g;
    isStopping = false;
    lastTime = 0;
    a = 0;
    lChng = 30;
    mainTxtr = imagePool.loadResource(Properties::MiscImagePath+"sandMain.png");
    swirlTxtr = imagePool.loadResource(Properties::MiscImagePath+"sandSwirl.png");
    mainSpr.setTexture(*mainTxtr);
    mainSpr.setOrigin(Vector2f(mainTxtr->getSize().x/2,mainTxtr->getSize().y/2));
    swirlSpr.setTexture(*swirlTxtr);
    cover.setFillColor(Color(230,230,90,0));
    mainSpr.setColor(Color(255,255,255,0));
    swirlSpr.setColor(Color(255,255,255,0));

    int minX = g->world.getCamera().x-100;
    int minY = g->world.getCamera().y-100;
    for (int x = 0; x<16; x++)
    {
        for (int y = 0; y<14; y++)
        {
            sand.push_back(Vector2f(minX+x*98,minY+y*55));
        }
    }
    for (int i = 0; i<40; ++i)
    {
        swirls.push_back(Vector3f(Random(minX+200,minX+1468),Random(minY+200,minY+900),Random(0,360)));
    }
}

SandstormWeather::~SandstormWeather()
{
    //
}

void SandstormWeather::stop()
{
    isStopping = true;
}

int SandstormWeather::getLightChange()
{
    return lChng;
}

bool SandstormWeather::isFinished()
{
    return mainSpr.getColor().a==0;
}

void SandstormWeather::update()
{
    int minX = _g->world.getCamera().x - 100;
    int minY = _g->world.getCamera().y - 100;
    int sX = _g->world.getCamera().x + 1468;
    int sY = _g->world.getCamera().y + 660;
    double dt = gameClock.getTimeStamp()-lastTime;
    double dx1 = dt*0.363636, dy1 = dt*0.090909;
    double dx2 = dt*0.545454, dy2 = dt*0.121212, dz = dt*0.454545;

    for (unsigned int i = 0; i<sand.size(); ++i)
    {
        sand[i].x -= dx1;
        sand[i].y -= dy1;
        if (sand[i].x<minX)
            sand[i].x = sX;
        if (sand[i].y<minY)
            sand[i].y = sY;
    }
    for (unsigned int i = 0; i<swirls.size(); ++i)
    {
        swirls[i].x -= dx2;
        swirls[i].y -= dy2;
        swirls[i].z -= dz;
        if (swirls[i].x<minX || swirls[i].y<minY)
            swirls[i] = Vector3f(sX,Random(minY+200,minY+900),Random(0,360));
    }

    if (isStopping)
    {
        a -= dt*0.151515;
        if (a<0)
            a = 0;
        mainSpr.setColor(Color(255,255,255,a));
        swirlSpr.setColor(Color(255,255,255,a));
        a -= 5.303;
        if (a<0)
            a = 0;
        cover.setFillColor(Color(230,230,90,a));
    }
    else if (mainSpr.getColor().a<255)
    {
        a += dt*0.151515;
        if (a>255)
            a = 255;
        mainSpr.setColor(Color(255,255,255,a));
        swirlSpr.setColor(Color(255,255,255,a));
        if (a>2.4242)
            a = 80;
        cover.setFillColor(Color(230,230,90,a));
    }
}

void SandstormWeather::draw(RenderWindow* window)
{
    window->draw(cover);
    for (unsigned int i = 0; i<sand.size(); ++i)
    {
        mainSpr.setPosition(Vector2f(sand[i]-_g->world.getCamera()));
        mainSpr.setRotation(sin((sand[i].x+sand[i].y)/160*3.1415926)*30);
        window->draw(mainSpr);
    }
    for (unsigned int i = 0; i<swirls.size(); ++i)
    {
        swirlSpr.setPosition(Vector2f(swirls[i].x,swirls[i].y)-_g->world.getCamera());
        swirlSpr.setRotation(swirls[i].z);
        window->draw(swirlSpr);
    }
}

Weather::Weather(Game* g)
{
    game = g;
    weather = NULL;
    nextChange = 0;
    type = None;
    isStopping = false;
    curLight = 0;
    desiredLight = 0;
}

void Weather::init(Type tp)
{
    if (weather)
    {
    	if (tp!=None)
        {
        	weather.reset();
			weather = NULL;
        }
        else
			weather->stop();
    }

    type = tp;
    if (type==LightRain)
        weather.reset(new RainWeather(game,false,false));
    else if (type==LightRainThunder)
        weather.reset(new RainWeather(game,false,true));
    else if (type==HardRain)
        weather.reset(new RainWeather(game,true,false));
    else if (type==HardRainThunder)
        weather.reset(new RainWeather(game,true,true));
    else if (type==LightSnow)
        weather.reset(new SnowWeather(game,false,false));
    else if (type==LightSnowThunder)
        weather.reset(new SnowWeather(game,false,true));
    else if (type==HardSnow)
        weather.reset(new SnowWeather(game,true,false));
    else if (type==HardSnowThunder)
        weather.reset(new SnowWeather(game,true,true));
    else if (type==Sunny)
        weather.reset(new SunnyWeather());
    else if (type==ThinFog)
        weather.reset(new FogWeather(game,false));
    else if (type==ThickFog)
        weather.reset(new FogWeather(game,true));
    else if (type==SandStorm)
        weather.reset(new SandstormWeather(game));

	nextChange = gameClock.getTimeStamp()+Random(180000,420000);
}

void Weather::update()
{
    if (curLight<desiredLight)
        curLight += 1;
    if (curLight>desiredLight)
        curLight -= 1;

    desiredLight = 0;
    if (weather)
    {
        weather->update();
        desiredLight = weather->getLightChange();
        if (gameClock.getTimeStamp()>=nextChange && (type==WaterRandom || type==SnowRandom || type==DesertRandom || type==AllRandom) && !isStopping)
        {
            weather->stop();
            isStopping = true;
        }
        if (isStopping)
        {
            if (weather->isFinished())
            {
                isStopping = false;
                weather.reset();
                weather = NULL;
                nextChange = gameClock.getTimeStamp()+Random(180000,420000);
            }
        }
    }

    else if (gameClock.getTimeStamp()>=nextChange)
    {
        if (type==WaterRandom)
            createRain();
        else if (type==SnowRandom)
            createSnow();
        else if (type==AllRandom)
        {
            int w = Random(0,500);
            if (w<100)
                createRain();
            else if (w<200)
                createSnow();
            else if (w<300)
                weather.reset(new SunnyWeather());
            else if (w<400)
                weather.reset(new FogWeather(game,Random(0,1000)<500));
            else
                weather.reset(new SandstormWeather(game));
        }
        nextChange = gameClock.getTimeStamp()+Random(120000,600000);
    }
}

void Weather::createRain()
{
    bool isHard = Random(0,1000)<500;
    bool canThunder = Random(0,1000)<500;
    weather.reset(new RainWeather(game,isHard,canThunder));
}

void Weather::createSnow()
{
    bool isHard = Random(0,1000)<500;
    bool canThunder = Random(0,1000)<500;
    weather.reset(new SnowWeather(game,isHard,canThunder));
}

void Weather::draw(RenderWindow* window)
{
    if (weather)
        weather->draw(window);
}

int Weather::getLightModifier()
{
    return curLight;
}
