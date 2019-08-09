#ifndef PPSHOWER_HPP
#define PPSHOWER_HPP

#include "Menu/Menu.hpp"
#include "Peoplemon/Move.hpp"

class Game;

/**
 * Menu class to show move PP when the player is choosing a move
 *
 * \ingroup Menu
 * \ingroup Battle
 */
class PpShower
{
    Game* game;
    MoveRef* moves;

    MenuImage bgnd;
    MenuText name, type, pp;
    Menu menu;

    /**
     * Updates the gui elements with the new moves information
     */
    void updateText(const MoveRef& mv);

public:
    /**
     * Constructs the PpShower and populates with the first valid move
     */
    PpShower(Game* game, MoveRef* moves);

    /**
     * Finds the move with the given name and updates the gui for it
     */
    void updateText(const std::string& moveName);

    /**
     * Renders the move information to the screen
     */
    void draw(sf::RenderWindow* window);
};

#endif // PPSHOWER_HPP
