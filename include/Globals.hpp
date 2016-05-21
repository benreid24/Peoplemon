#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <vector>
#include "SFML.hpp"
#include "Util/GameClock.hpp"
#include "Resources/ResourcePool.hpp"

/**
 * \defgroup Global
 *  \brief Global functions to be used from everywhere
 */

extern class GameClock gameClock;
extern class PlayerInput user;
extern class ResourcePool<sf::Texture> imagePool;
extern class ResourcePool<sf::SoundBuffer> audioPool;
extern class ResourcePool<AnimationSource> animPool;
extern class std::vector<std::pair<sf::Keyboard::Key,std::string> > keymap;

/**
 * Returns the string representation of a key
 *
 * \param key The key to get the representation of
 * \return A string representing the given key, or "Unknown" if the key isn't found
 */
std::string keyToString(sf::Keyboard::Key key);

/**
 * Returns a random number in the range [min,max]
 *
 * \ingroup Global
 *
 * \param min The minimum value that can be returned
 * \param max The maximum value that can be returned
 * \return A random number
 */
int Random(int min, int max);

/**
 * Returns all files in the given directory with the given extension
 *
 * \ingroup Global
 *
 * \param dir The directory to search
 * \param ext The extension to look for
 * \return A list of all files in the given directory with the given extension
 */
std::vector<std::string> getDirectory(std::string dir, std::string ext);

/**
 * Converts and integer to a string
 *
 * \ingroup Global
 *
 * \param i The number to convert
 * \return The string representation of the number
 */
std::string intToString(int i);

/**
 * Converts a string to a base 10 integer
 *
 * \ingroup Global
 *
 * \param s The string to convert
 * \return The numerical value of the string
 */
int stringToInt(std::string s);

#endif // GLOBALS_HPP
