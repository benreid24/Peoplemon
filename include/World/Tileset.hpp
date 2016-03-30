#ifndef TILESET_HPP
#define TILESET_HPP

#include "SFML.hpp"
#include "Resources/ResourcePool.hpp"
#include <vector>

/**
 * This class loads tilesets from the file system and maps each resource to its id for the maps
 *
 * \ingroup World
 */
class Tileset
{
	std::string lastFile;
    std::vector<TextureReference> textures;
    std::vector<AnimationReference> animations;

public:
    /**
     * Creates and empty tileset
     */
    Tileset();

    /**
     * Loads the tileset from the given file
     *
     * \param file The file to load from
     */
    Tileset(std::string file);

    /**
     * Loads the tileset from the given file
     *
     * \param file The file to load from
     */
    void load(std::string file);

    /**
     * Returns a reference to the texture of the tile with the given id
     *
     * \param index The id of the tile to get
     * \return A reference to the requested tile, if it exists
     */
    TextureReference getTile(int index);

    /**
     * Returns a reference to the animation with the given id
     *
     * \param index The id of the animation to get
     * \return A reference to the requested animation, if it exists
     */
    AnimationReference getAnim(int index);

    /**
     * Returns the number or animations in the tileset
     */
	int numAnims();
};

#endif // TILESET_HPP
