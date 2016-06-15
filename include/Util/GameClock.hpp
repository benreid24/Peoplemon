#ifndef GAMECLOCK_HPP
#define GAMECLOCK_HPP

#include "SFML.hpp"

/**
 * Structure for storing the game time in hour:minute form
 *
 * \ingroup Utilities
 */
struct ClockTime
{
    int hour, minute;

    /**
     * Initializes the time to noon (12:00)
     */
    ClockTime()
    {
        hour = 12;
        minute = 0;
    }

    /**
     * Initializes the time to the given hour and minute
     *
     * \param h The hour of the time [0,23]
     * \param m The minute of the time [0,59]
     */
    ClockTime(int h, int m)
    {
        hour = h;
        minute = m;
    }
};

/**
 * This class is responsible for initializing, storing and updating the game time, both as time elapsed since play start and clock time in the game
 *
 * \ingroup Utilities
 */
class GameClock
{
    sf::Clock timer;
    ClockTime start;
    int startTime; //for tracking elapsed time since game play started
    int pausedTime;
    bool paused;

public:
    /**
     * Starts the timer and initializes the elapsed time to 0
     */
    GameClock()
    {
        startTime = 0;
        pausedTime = 0;
        start = ClockTime(12,0);
        paused = false;
    }

    /**
     * Does nothing, wasn't invited to the party, just showed up
     */
    ~GameClock(){}

    /**
     * Returns the time elapsed since program start in milliseconds
     */
    long getTimeStamp()
    {
        return timer.getElapsedTime().asMilliseconds();
    }

    /**
     * Returns the time elapsed in microseconds
     */
	long getMicros()
	{
		return timer.getElapsedTime().asMicroseconds();
	}

    /**
     * Returns the simulated in game time
     */
    ClockTime getClockTime() //present scale: 3 seconds = 1 minute
    {
        ClockTime t;
        int s = (paused)?(pausedTime):(getTimeStamp()-startTime);

        t.hour = (start.hour+(s/3000+start.minute)/60)%24;
        t.minute = (start.minute+s/3000)%60;

        return t;
    }

    /**
     * Sets the internal simulated time of the game
     *
     * \param t The time to set the game clock to
     */
    void setClockTime(ClockTime t)
    {
        start = t;
        startTime = timer.getElapsedTime().asMilliseconds();
    }

    /**
     * Sets the time to noon and resets the timer so that the time is synced
     */
	void newGame()
	{
		setClockTime(ClockTime(12,0));
		startTime = getTimeStamp();
		paused = false;
	}

    /**
     * Stops recording simulation time passing. Pauses time in essence, however getTimeStamp will still function
     */
    void pause()
    {
        pausedTime = getTimeStamp();
        paused = true;
    }

    /**
     * Continues tracking in game time
     */
    void unPause()
    {
        startTime += getTimeStamp()-pausedTime;
        paused = false;
    }
};

#endif // GAMECLOCK_HPP
