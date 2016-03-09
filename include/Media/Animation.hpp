#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "SFML.hpp"
#include <string>
#include <memory>

typedef std::shared_ptr<sf::Texture> TextureReference;

/**
 * Data structure to store information about individual frames in an animation
 *
 * \ingroup Media
 */
struct AnimationFrame
{
    sf::Vector2f sourcePos, size, renderOffset;
    int length, rotation, alpha;
    double scaleX,scaleY;
};

/**
 * This class handles the loading and storage of animation data. This enables the flyweight pattern
 * to be utilized when used in conjunction with the Animation class
 *
 * \ingroup Media
 */
class AnimationSource
{
    TextureReference sheet;
    std::vector<std::vector<AnimationFrame> > frames;
    bool loop;
    std::vector<sf::Sprite> sprites;

public:
    /**
     * Creates an empty animation
     */
    AnimationSource();

    /**
     * Loads an animation from the given file
     *
     * \param file The full path of the file to load
     */
    AnimationSource(std::string file);

    /**
     * Frees loaded resources
     */
    ~AnimationSource();

    /**
     * Loads an animation from the given file
     *
     * \param file The full path of the file to load
     */
    void load(std::string file);

    /**
     * Tells whether or not the animation automatically repeats when it finishes playing
     *
     * \return True if the animation replays by itself
     */
    bool isLooping();

    /**
     * Returns a Sprite object that is clipped, oriented and positioned where it needs to be according to the
     * animation data
     *
     * \param i The index of the frame to return
     * \param pos The desired on screen position of the animation
     * \return A vector of Sprite objects that are ready to be rendered
     */
    std::vector<sf::Sprite>& getFrame(int i, sf::Vector2f pos);

    /**
     * Given the current frame and elapsed time, combined with internal animation data, returns the new frame
     *
     * \param cFrm The current frame index
     * \param lTime The time elapsed since the last update
     * \return The index of the new animation frame that should be rendered
     */
    int incFrame(int cFrm, int lTime);

    /**
     * Tells the total number of frames in the loaded animation
     *
     * \return The total number of frames in the animation
     */
    int numFrames();
};

typedef std::shared_ptr<AnimationSource> AnimationReference;

class Animation
{
    AnimationReference animSrc;
    sf::Vector2f position;
    int curFrm, lastFrmChangeTime;
    bool playing;

public:
    /**
     * Creates an empty animation
     */
    Animation();

    /**
     * Sets the source data to the given AnimationSource
     *
     * \param src The AnimationSource to use
     */
    Animation(AnimationReference src);

    /**
     * Frees allotted resources
     */
    ~Animation();

    /**
     * Sets the source of the animation
     *
     * \param src The AnimationSource to use
     */
    void setSource(AnimationReference src);

    /**
     * Updates the animation. Sets the proper frame based on elapsed time since the last call to update
     */
    void update();

    /**
     * Sets the current frame to the given frame and resets the internal timer
     *
     * \param frm The index of the frame to make current
     */
    void setFrame(int frm);

    /**
     * Tells whether or not the animation has finished playing
     *
     * This function returns true always if the animation is set to loop. Otherwise, it returns true only
     * if play has been called and the animation finished displaying all of its frames
     *
     * \return Whether or not the animation finished
     */
    bool finished();

    /**
     * Tells whether or not the animation is set to loop
     *
     * \return Whether or not the animation is set to repeat automatically when finished
     */
    bool isLooping();

    /**
     * Starts playing the animation. This sets the current frame to 0 and resets the internal timer
     */
    void play();

    /**
     * Sets the desired position of the animation on screen. Individual frames are offset from this position
     *
     * \param pos The desired on screen position
     */
    void setPosition(sf::Vector2f pos);

    /**
     * Renders the animation to the given window
     *
     * \param window A pointer to the window to render to
     */
    void draw(sf::RenderWindow* window);
};

#endif // ANIMATION_HPP
