#ifndef CREDITSSTATE_HPP
#define CREDITSSTATE_HPP

#include "Gamestate.hpp"
#include "SFML.hpp"
#include "Util/File.hpp"
#include "Resources/ResourcePool.hpp"

/**
 * This class is to store and manipulate data related to "credit objects", meaning text or images in the credits
 *
 * \ingroup Game
 */
class CreditsObject
{
    bool isImage;
    TextureReference txtr;
    sf::Sprite spr;
    sf::Text text;
    int yPos;

public:
    /**
     * Loads the object from the given file
     *
     * \param file The file to load from
     * \param lastPos The y coordinate of the most recent object added
     */
    CreditsObject(File* file, int lastPos);

    /**
     * Renders the object to the given window and applies the provided offset
     *
     * \param window The window to render to
     * \param yOff The offset to apply before rendering
     */
    void draw(sf::RenderWindow* window, int yOff);

    /**
     * Returns the y coordinate of this object
     */
    int getPos();
};

class CreditsState : public Gamestate
{
    std::vector<CreditsObject> objects;
    int goToPos; //last y pos plus a full screen height

    /**
     * Runs the state
     */
    bool execute();

public:
    /**
     * Initializes the state and loads all of the credit objects from the credits file
     *
     * \param g A pointer to the main game object
     */
    CreditsState(Game* g);
};

#endif // CREDITSSTATE_HPP
