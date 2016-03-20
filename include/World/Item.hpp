#ifndef ITEM_HPP
#define ITEM_HPP

#include "Object.hpp"
#include "Resources/ResourcePool.hpp"

extern class ResourcePool<sf::Texture> imagePool;
std::string intToString(int i);

/**
 * This class represents items in the over world as objects that can be rendered and interacted with
 *
 * \ingroup Entities
 */
class Item : public Object
{
    int itemId, mapId;
    TextureReference ball;

public:
    /**
     * Initializes the item with the given data
     *
     * \param w A pointer to the World object
     * \param iId The id of the item itself
     * \param mId The map unique id of the item
     * \param pos The position of the item in world pixels
     */
    Item(World* w, int iId, int mId, sf::Vector2f pos)
    {
        itemId = iId;
        mapId = mId;
        position = pos+sf::Vector2f(32,32);
        ball = imagePool.loadResource("Resources/Media/Images/World/Misc/item.png");
        if (iId<500)
        {
            w->setSpaceOccupied(sf::Vector2i(position.x/32,position.y/32),true);
            sprite.setTexture(*ball);
        }
        else
			iId -= 500;
    }

    /**
     * Does nothing
     */
    void update(Game* game){}

    /**
     * Adds the item to the player's inventory and remove from the map
     *
     * \param game A pointer to the main Game object
     */
    void interact(Game* game)
    {
    	if (itemId<500)
			game->world.setSpaceOccupied(sf::Vector2i(position.x/32,position.y/32),false);
        game->player.giveItem(itemId);
        game->world.setItemPickedUp(mapId);
        game->world.removeObject(this);
        game->hud.displayMessage("You found a "+game->itemList[itemId].name+"!","foundItem.wav");
    }

    /**
     * Renders the item as a pokeball to the given window
     *
     * \param window The window to render to
     * \param camPos The position of the camera
     */
    void draw(sf::RenderWindow* window, sf::Vector2f camPos)
    {
        sprite.setPosition(position-camPos-sf::Vector2f(32,32));
        window->draw(sprite);
    }

    /**
     * Returns the string identifier of the item
     */
	std::string getIdentifier()
	{
		return "Item("+intToString(itemId)+")";
	}
};

#endif // ITEM_HPP
