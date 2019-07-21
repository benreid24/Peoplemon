#ifndef BAGITEMLIST_HPP
#define BAGITEMLIST_HPP

#include "Menu.hpp"
#include <vector>

class Game;

/**
 * This is a helper UI class to be used by BagState. It is for displaying items in the bag and
 * allowing the user to look through them
 *
 * \ingroup Menu
 */
class BagItemList
{
    std::vector<std::pair<int,std::string> > itemList; //id, name
    int curItem;

    MenuImage upArrow, downArrow, unSelBox, selBox;
    MenuText itemName;

public:
    /**
     * Initialize UI elements from the given data
     *
     * \param items A pointer to the vector storing the player's items
     * \param first The first id to include from the vector
     * \param last The last id to include. Included ids are in the range [first,last]
     * \param g A pointer to the main Game object
     */
    BagItemList(std::vector<int>* items, int first, int last, Game* g);

    /**
     * Updates the UI based on user input
     */
    void update();

    /**
     * Render the items to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);

    /**
     * Set the items contained in the menu
     *
     * \param items A pointer to the vector storing the player's items
     * \param first The first id to include from the vector
     * \param last The last id to include. Included ids are in the range [first,last]
     * \param g A pointer to the main Game object
     * \param firstExclude First item id to exclude
     * \param lastExclude Last item id to exclude. Excluded range is [firstExclude, lastExclude]
     */
    void setItems(std::vector<int>* items, int first, int last, Game* g, int firstExclude = 99999, int lastExclude = -1);

    /**
     * Returns the id of the currently selected item
     *
     * \return The id of the selected item
     */
    int getCurrentItem();
};

#endif // BAGITEMLIST_HPP
