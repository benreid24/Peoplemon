#ifndef MENUIMAGE_HPP
#define MENUIMAGE_HPP

#include "SFML.hpp"
#include "Resources/ResourcePool.hpp"
#include <string>

/**
 * Helper class to consolidate the code required to load/render images. Specifically for images that will
 * appear in menus
 *
 * \ingroup Menu
 * \ingroup Media
 */
class MenuImage
{
    TextureReference txtr;
    sf::Sprite spr;

public:
    /**
     * Creates an empty menu image object
     */
    MenuImage();

    /**
     * Creates a menu image from th given file and position
     *
     * \param file The path to the image to load
     * \param pos The position on the screen to put the image at when rendering
     */
    MenuImage(std::string file, sf::Vector2f pos = sf::Vector2f());

    /**
     * Loads an image from the given file
     *
     * \param file The path to the image to load
     * \param prefixPath Whether or not to prefix the filename with Properties::MenuImagePath
     * \param size Optionally scales the image to this size
     */
    void setImage(std::string file, bool prefixPath = true, sf::Vector2f size = sf::Vector2f());

    /**
     * Scales the image to the given size
     *
     * \param size The size to scale the image to
     */
    void setSize(sf::Vector2f size);

    /**
     * Sets the position to render the image at on the screen
     *
     * \param pos The position to render the image at
     */
    void setPosition(sf::Vector2f pos); //origin is upper left corner

    /**
     * Sets the origin of the image in local coordinates. The default is (0,0)
     *
     * \param orig The origin of the image
     */
    void setOrigin(sf::Vector2f orig);

    /**
     * Rotates the image
     *
     * \param dir The angle of rotation in degrees
     */
    void rotate(int dir); //origin is always center

    /**
     * Renders the image to the given window
     *
     * \param window The window to render the image in
     */
    void draw(sf::RenderWindow* window);
};

#endif // MENUIMAGE_HPP
