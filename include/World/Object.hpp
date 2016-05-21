#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "SFML.hpp"

class Game;

/**
 * Base class for all objects that exist in the overworld
 *
 * \ingroup World
 */
class Object
{
protected:
    sf::Sprite sprite;
    sf::Vector2f position;

public:
    /**
     * Here for polymorphism
     */
    virtual ~Object(){};

    /**
     * Returns the position of the object in world pixels
     */
    sf::Vector2f getPosition()
    {
        return position;
    }

    /**
     * Renders the object to the screen. Needs to be overridden for more graphically complex objects
     *
     * \param window The window to render the object to
     * \param camPos The position of the camera
     */
    virtual void draw(sf::RenderWindow* window, sf::Vector2f camPos)
    {
        sprite.setPosition(position-camPos);
        window->draw(sprite);
    }

    /**
     * Does one turn of action on the object
     *
     * \param game A pointer to the main Game object
     */
    virtual void update(Game* game)=0;

    /**
     * Defines what the object should do when interacted with by the player
     *
     * \param game A pointer to the main Game object
     */
    virtual void interact(Game* game)=0; //what to do when player interacts with it

    /**
     * Returns a string to identify the object for debug purposes
     */
    virtual std::string getIdentifier()
    {
        return "Object Base!!";
    }
};

#endif // OBJECT_HPP
