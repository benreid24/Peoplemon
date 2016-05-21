#ifndef PEOPLEMONANIMATIONS_HPP
#define PEOPLEMONANIMATIONS_HPP

#include "MoveAnimations.hpp"
#include "Peoplemon/Peoplemon.hpp"

class Game;

/**
 * Structure for easily storing and loading all the animations associated with a peoplemon
 *
 * \ingroup Battle
 * \ingroup Media
 */
struct PeoplemonAnimations
{
    AnimationReference cIn, sO, fnt, st;
    MoveAnimations moves[4];
    Animation comeIn, sendOut, faint, still;

    /**
     * Loads all the proper animations given the data passed
     *
     * \param g Pointer to the main game object
     * \param ppl The user's current peoplemon
     * \param op The opponents peoplemon
     * \param isPlayer Whether or not the user is the player
     */
    void load(Game* g, PeoplemonRef ppl, PeoplemonRef op, bool isPlayer);
};

#endif // PEOPLEMONANIMATIONS_HPP
