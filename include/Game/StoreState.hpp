#ifndef STORESTATE_HPP
#define STORESTATE_HPP

#include "Gamestate.hpp"
#include "Menu/Menu.hpp"

/**
 * This Gamestate is the store that the player can shop in, buying and selling items
 *
 * \ingroup Game
 */
class StoreState : public Gamestate
{
    std::string say, hobo;
    std::vector<std::pair<int,int> > items; // <id,price>
    std::vector<std::pair<int,int> > playerItems; //id,quantity

    MenuText title, desc, money;

    /**
     * Wraps the given string to fit in the text box
     *
     * \param orig The string to wrap
     * \return The wrapped string
     */
    std::string wordWrap(std::string orig);

    /**
     * Creates a string to represent the given item in the choice box using its name and price
     *
     * \param name The name of the item
     * \param price The price of the item
     * \return A string containing the name and price of the item, with whitespace added for appearance
     */
    std::string createItem(std::string name, int price);

    /**
     * Returns the given item from the choice string
     *
     * \param vec The vector of items to search in
     * \param str The string visually representing the item
     * \return The index of the item in the items vector
     */
    int getItem(std::vector<std::pair<int,int> >* vec, std::string str);

    /**
     * Adds the item to the sellItems vector, or updates the quantity if it is already there
     *
     * \param id The id of the item to add
     * \param q The quantity to add
     */
    void addItem(int id, int q);

    /**
     * Runs the state
     */
    bool execute();

public:
    /**
     * Constructs the state using the given items
     *
     * \param g A pointer to the main Game object
     * \param prompt What the store should say when first entered
     * \param error What the store should say when the player doesn't have enough money
     * \param i A vector containing all of the items. The first element of each pair is the id, the second is the price
     */
    StoreState(Game* g, std::string prompt, std::string error, std::vector<std::pair<int,int> > i);
};

#endif // STORESTATE_HPP
