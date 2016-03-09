#ifndef BATTLEBOX_HPP
#define BATTLEBOX_HPP

#include "SFML.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Menu/Menu.hpp"

/**
 * This is a helper class for BattleState. It handles updating and rendering the status boxes for each peoplemon
 * in a battle
 *
 * \ingroup Menu
 * \ingroup Battle
 */
class BattleBox
{
    bool isPlayer;
    sf::Vector2f position;
    sf::RectangleShape hpBar,xpBar;
    int hpTargetW, xpTargetW; //for slow updating
    MenuText name, level, curHp;
    TextureReference boxTxtr;
    sf::Sprite box;

public:
    /**
     * Creates and empty battle box
     */
    BattleBox();

    /**
     * Calling this function will cause the box to be positioned and displayed for the player peoplemon
     */
    void setIsPlayer();

    /**
     * Updates the visual elements with the data from the given peoplemon
     *
     * \param ppl The peoplemon whose data should fill the box
     * \param grad Whether or not the HP bar should gradually change or just jump to its new point
     */
    void update(PeoplemonRef ppl, bool grad = true);

    /**
     * Updates the HP bar and XP bar to be closer to where they're supposed to be, if not already there
     */
    void update();

    /**
     * Tells whether or not the HP bar is where it is supposed to be
     *
     * \return Whether or not the HP bar is filled to the proper level
     */
    bool barGood();

    /**
     * Renders the box to the given window
     *
     * \param window The window to render to
     */
    void draw(sf::RenderWindow* window);
};

#endif // BATTLEBOX_HPP
