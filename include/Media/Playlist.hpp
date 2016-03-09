#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include "SFML.hpp"
#include <string>
#include <vector>

class Game;

/**
 * This class can load playlist files and all appropriate music files, and then plays the music in random order
 *
 * \ingroup Media
 */
class Playlist
{
	Game* game;
    sf::Music audio;
    std::vector<std::string> songs; //TODO - possibly threadsafe this
    std::vector<int> order;
    int curSong;
    bool started, wasMutedLast;
    std::string curList, prevList;

public:
    /**
     * Creates an empty playlist
     *
     * \param g A pointer to the main Game object
     */
    Playlist(Game* g);

    /**
     * Frees allocated resources
     */
    ~Playlist();

    /**
     * Loads the playlist from the given file
     *
     * \param file The full path of the file to load from
     * \param savePrev Whether or not to store the currently playing list for later playing
     */
    void load(std::string file, bool savePrev = false);

    /**
     * Starts playing the playlist from the first song
     */
    void play();

    /**
     * Plays the previously playing list, or stops the music if none exists
     */
    void previous();

    /**
     * Stops the music and resets the internal state
     */
    void stop();

    /**
     * Checks if the current song has finished playing, and moves to the next song if it has
     */
    void update();
};

#endif // PLAYLIST_HPP
