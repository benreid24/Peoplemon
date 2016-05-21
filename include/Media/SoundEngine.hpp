#ifndef SOUNDENGINE_HPP
#define SOUNDENGINE_HPP

#include "SFML.hpp"
#include "Resources/ResourcePool.hpp"
#include <map>
#include <memory>

typedef int AudioReference;
class Game;

/**
 * \defgroup Media
 * \brief All classes related to media loading and usage are in this module
 */

/**
 * Structure for storing data associated with a playing audio sample
 *
 * \ingroup Media
 */
struct Audio
{
    SoundReference ref;
    sf::Sound sound;
    int curPlays, maxPlays;
};

/**
 * This class is responsible for managing all of the playing audio samples, looping them and stopping them
 * when they have finished the desired amount of playbacks
 *
 * \ingroup Media
 */
class SoundEngine
{
	Game* game;
    std::map<AudioReference,std::shared_ptr<Audio> > sounds;
    std::vector<std::pair<AudioReference,std::shared_ptr<Audio> > > fadeOuts;
    AudioReference lastAssigned;

    sf::Thread updater;
    sf::Mutex lock;
    bool running;

    /**
     * This function is run on a separate thread and manages updating all of the playing audio
     */
    void update();

public:
    /**
     * Initializes the internal state and starts the updater thread
     *
     * \param g A pointer to the main Game object
     */
    SoundEngine(Game* g);

    /**
     * Stops all audio and terminates the updater thread
     */
    ~SoundEngine();

    /**
     * Plays an audio sample however many times are specified
     *
     * \param file The path to the audio file to play. Will be prefixed by Properties::AudioPath
     * \param loops The number of times to play the audio. Passing -1 makes it loop forever
     * \return An AudioReference that can be used to manipulate the sound
     */
    AudioReference playSound(std::string file, int loops = 1);

    /**
     * Stops playing the given audio sample
     *
     * \param ref The AudioReference referring to the desired sample to stop
     */
    void stopSound(AudioReference ref);

    /**
     * Fades out, then stops the passed audio sample
     *
     * \param ref The AudioReference referring to the desired sample to fade and stop
     */
    void fadeOut(AudioReference ref);

    /**
     * Stops playing all sounds
     */
    void stopAll();
};

#endif // SOUNDENGINE_HPP
