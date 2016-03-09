#ifndef WEATHER_HPP
#define WEATHER_HPP

#include "SFML.hpp"
#include "Resources/ResourcePool.hpp"
#include "Media/SoundEngine.hpp"
#include <list>
#include <memory>

class Game;

/**
 * Specialized vector structure for storing and working with weather particles
 *
 * \ingroup Utilities
 */
struct Vector4D
{
    float x,y,z;
    int time;

    /**
     * Initializes the vector with the given parameters
     *
     * \param _x The x component
     * \param _y The y component
     * \param _z the z component
     * \param t The time component
     */
    Vector4D(float _x, float _y, float _z, int t)
    {
        x = _x;
        y = _y;
        z = _z;
        time = t;
    }

    /**
     * Adds the given 3d vector to this one by adding the x and y components by subtracting the z component
     *
     * \param v The 3d vector to add to this vector
     */
    void operator+= (const sf::Vector3f& v)
    {
        if (time==0)
        {
            x += v.x;
            y += v.y;
            z -= v.z;
        }
    }

    /**
     * Converts this vector to 2d screen space
     *
     * \param cam The position of the camera
     */
    sf::Vector2f toScreen(sf::Vector2f cam)
    {
        return sf::Vector2f(x-z/4-cam.x,y-z/2-cam.y);
    }
};

/**
 * Basic structure for storing information about rotating particles
 *
 * \ingroup Utilities
 */
struct Particle
{
    float x,y;
    float rotation, angularVel;

    /**
     * Initializes the Particle with the given parameters
     *
     * \param _x The x component
     * \param _y The y component
     * \param r The angular position
     * \param av The angular velocity
     */
    Particle(float _x, float _y, float r, float av)
    {
        x = _x;
        y = _y;
        rotation = r;
        angularVel = av;
    }
};

/**
 * Base class to define an interface for the Weather class to use that is the same for every type of weather
 *
 * \ingroup World
 */
class BaseWeatherType
{
protected:
	int lastTime;
public:
    /**
     * Here for polymorphism
     */
    virtual ~BaseWeatherType(){}

    /**
     * Stops the weather by starting whatever "fadeout" effect it has, if any
     */
    virtual void stop(); //slow transition to clear weather. possibly call this while also starting another type

    /**
     * Returns whether or not the weather has finished "fading out" after a call to stop
     */
    virtual bool isFinished();

    /**
     * Returns an integer modifier to be added to the ambient light level of the world
     */
    virtual int getLightChange();

    /**
     * Updates the weather and its effects
     */
    virtual void update();

    /**
     * Renders the weather to the given window
     *
     * \param window The window to render to
     */
    virtual void draw(sf::RenderWindow* window);
};

/**
 * This class is a helper class for other weather classes. It manages the flashes, sound and timing of thunder
 *
 * \ingroup World
 */
class Thunder
{
    sf::RectangleShape flash;
    int goTime; //time for first flash to go down, then time for second flash to start, then time for second flash to die. init based on alpha channel of flash
    int lastTime;

public:
    /**
     * Initializes the thunder
     */
    Thunder();

    /**
     * Updates the thunder
     *
     * \param g A pointer to the main Game object
     */
    void update(Game* g);

    /**
     * Renders the thunder to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);
};

/**
 * This class handles all intensities of rain weather, and optionally controls a Thunder object
 *
 * \ingroup World
 */
class RainWeather : public BaseWeatherType
{
    Game* _g;
    AudioReference sound;
    TextureReference airDrop, splashDrop[2];
    sf::Sprite airSpr, splashSpr[2];

    std::vector<Vector4D> rainDrops;
    sf::Vector3f rainVel;
    int maxDrops;

    Thunder thunder;
    bool canThunder;

    bool isStopping, isDone;
    int creationTimer;
    double createPerSec;
    int lChng;

public:
    /**
     * Initializes the rain storm
     *
     * \param g A pointer to the main Game object
     * \param isHard Whether or not the rain is intense
     * \param thunder Whether or not there should be thunder
     */
    RainWeather(Game* g, bool isHard, bool thunder);

    /**
     * Cleans up all resources and dries off the player
     */
    ~RainWeather();

    /**
     * Begins the process of getting rid of raindrops until there are no more
     */
    void stop();

    /**
     * Returns whether or not the rain has finished stopping
     */
    bool isFinished();

    /**
     * Returns a light change value correspondant to the intensity of the rain
     */
    int getLightChange();

    /**
     * Updates all of the rain drops and the thunder, if any
     */
    void update();

    /**
     * Renders the storm to the window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);
};

/**
 * This class handles all intensities of snow weather and also optionally manages a Thunder object
 *
 * \ingroup World
 */
class SnowWeather : public BaseWeatherType
{
    Game* _g;
    TextureReference flake;
    sf::Sprite spr; //vary transparency based on time

    std::vector<Vector4D> flakes; //modulate x/y as an alternating,repeating function of z?
    int maxFlakes;  //light snow will be repeating function, hard snow will be linear
    double fallSpeed;
    Thunder thunder;
    bool canThunder;

    bool isStopping, isDone;
    int lChng, lifeTime, creationTimer;
    double createPerSec;

public:
    /**
     * Initializes the storm
     *
     * \param g A pointer to the main Game object
     * \param isHard Whether or not the intensity of the snow should be hard
     * \param thunder Whether or not it should thunder
     */
    SnowWeather(Game* g, bool isHard, bool thunder);

    /**
     * Melts all the snow
     */
    ~SnowWeather();

    /**
     * Begins the process of slowly stopping the snow
     */
    void stop();

    /**
     * Returns whether or not the snow has finished stopping
     */
    bool isFinished();

    /**
     * Returns a change in the ambient lighting correspondnt to the intensity of the snow
     */
    int getLightChange();

    /**
     * Updates all of the snow flakes and thunder
     */
    void update();

    /**
     * Draws the storm to the given window
     *
     * \param window The window to render the snow to
     */
    void draw(sf::RenderWindow* window);
};

/**
 * This class makes fog!
 *
 * \ingroup World
 */
class FogWeather : public BaseWeatherType
{
    Game* _g;
    TextureReference fogTxtr;
    sf::Sprite spr;
    std::vector<Particle> particles;
    bool isStopping; //stop by making spr transparent
    int targetA, lChng;
    double a;

public:
    /**
     * It's like a fog machine!
     *
     * \param g A pointer to the main Game object
     * \param isThick Whether or not the fog is thick
     */
    FogWeather(Game* g, bool isThick);

    /**
     * Condensation!
     */
    ~FogWeather();

    /**
     * Stops the fog
     */
    void stop();

    /**
     * Returns whether or not the fog has finished going away
     */
    bool isFinished();

    /**
     * Returns an ambient light change correspondant to the thickness of the fog
     */
    int getLightChange();

    /**
     * Makes it look beautiful
     */
    void update();

    /**
     * Renders the fog to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);
};

/**
 * This makes a sand storm
 *
 * \ingroup World
 */
class SandstormWeather : public BaseWeatherType
{
    Game* _g;
    TextureReference mainTxtr, swirlTxtr;
    sf::Sprite mainSpr, swirlSpr;
    sf::RectangleShape cover;
    std::vector<sf::Vector2f> sand;
    std::vector<sf::Vector3f> swirls;
    bool isStopping;
    int lChng;
    double a;

public:
    /**
     * Creates the storm
     *
     * \param g A pointer to the main Game object
     */
    SandstormWeather(Game* g);

    /**
     * Saves the dustbowl people
     */
    ~SandstormWeather();

    /**
     * Stops the sandstorm
     */
    void stop();

    /**
     * Returns whether or not the sandstorm has finished stopping
     */
    bool isFinished();

    /**
     * Returns the ambient light change of the storm
     */
    int getLightChange();

    /**
     * Moves all the sand
     */
    void update();

    /**
     * Renders the storm to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);
};

/**
 * This class just makes it look really bright by pulsating a yellow filter
 *
 *\ingroup World
 */
class SunnyWeather : public BaseWeatherType
{
    sf::RectangleShape cover;
    double t,a;
    bool isStopping;

public:
    /**
     * Initializes the pulsar
     */
    SunnyWeather();

    /**
     * Deactivates the pulsar
     */
    ~SunnyWeather();

    /**
     * Returns the amount to change the ambient brightness by
     */
    int getLightChange();

    /**
     * Stops the sun! D:
     */
    void stop();

    /**
     * Reports on the status of the inevitable heat death of the universe
     */
    bool isFinished();

    /**
     * Pulses the pulsar
     */
    void update();

    /**
     * Renders the pulsar on the world of Peoplemon
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);
};

/**
 * This class is to be used directly by World and will update and manage the weather in the game
 *
 * \ingroup World
 */
class Weather
{
public:
    /**
     * Initializes the internal state
     *
     * \param g A pointer to the main Game object
     */
    Weather(Game* g);

    /**
     * Defines all of the various types of weather
     */
    enum Type
    {
        None = 0,
        AllRandom = 1,
        LightRain = 2,
        LightRainThunder = 3,
        HardRain = 4,
        HardRainThunder = 5,
        LightSnow = 6,
        LightSnowThunder = 7,
        HardSnow = 8,
        HardSnowThunder = 9,
        ThinFog = 10,
        ThickFog = 11,
        Sunny = 12,
        SandStorm = 13,
        WaterRandom = 14,
        SnowRandom = 15,
        DesertRandom = 16
    };

    /**
     * Sets the current weather to the given type
     *
     * \param type The type of weather to switch to
     */
    void init(Type type);

    /**
     * Updates the internal weather, if any, or chooses a new weather type if a random type was set and the timing is right
     */
    void update();

    /**
     * Renders the current weather to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);

    /**
     * Returns the current ambient light modifier of the weather
     */
    int getLightModifier();

private:
    Game* game;
    std::shared_ptr<BaseWeatherType> weather;
    Type type;
    int nextChange;
    int curLight, desiredLight;
    bool isStopping;

    /**
     * Creates a random type of rain by varying intensity and thunder
     */
    void createRain();

    /**
     * Creates a random type of snow by varying intensity and thunder
     */
    void createSnow();
};

#endif // WEATHER_HPP
