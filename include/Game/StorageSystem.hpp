#ifndef STORAGESYSTEM_HPP
#define STORAGESYSTEM_HPP

#include "Gamestate.hpp"
#include "Resources/ResourcePool.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Menu/Menu.hpp"
#include <map>

/**
 * This game state opens the storage system and allows the player to manage their peoplemon
 *
 * \ingroup Game
 */
class StorageSystem : public Gamestate
{
    TextureReference cursorTxtr, backgroundTxtr, rArrowTxtr, lArrowTxtr, boxTitleTxtr[14], boxTxtr[14];
    std::map<int,TextureReference> peoplemonTxtrs; //id is index

    sf::Sprite cursor, background, rArrow, lArrow, boxTitle[14], box[14];
    std::map<int,sf::Sprite> icons; //id is index

    std::vector<StoredPeoplemon>* peoplemon;
    sf::Vector2i cursorPos;

    MenuText name, level, item;
    MenuImage cancelBut;

    /**
     * Returns whether or not the given space is taken
     *
     * \param b The id of the box to check
     * \param x The x coordinate to check
     * \param y The y coordinate to check
     * \return Whether or not the given space is open
     */
    bool spaceFree(int b, int x, int y);

    /**
     * Returns the peoplemon at the given space, if any
     *
     * \param b The box to check
     * \param x The x coordinate
     * \param y The y coordinate
     * \return The peoplemon at the given space, or a peoplemon with id = 0 if there is none
     */
    PeoplemonRef getPeoplemon(int b, int x, int y);

    /**
     * Returns the index of the peoplemon at the given space, if any
     *
     * \param b The box to check
     * \param x The x coordinate
     * \param y The y coordinate
     * \return The index of the peoplemon at the given space, or -1 if there is none
     */
    int getIndex(int b, int x, int y);

    /**
     * Updates the peoplemon info on the left side of the screen
     *
     * \param i The index of the peoplemon currently selected
     */
    void updateMenu(int i);

    /**
     * Runs the state
     */
    bool execute();

public:
    /**
     * Initializes the internal state data
     *
     * \param g A pointer to the main Game object
     * \param ppl A pointer to the vector containing the player's stored peoplemon
     */
    StorageSystem(Game* g, std::vector<StoredPeoplemon>* ppl);
};

#endif // STORAGESYSTEM_HPP
