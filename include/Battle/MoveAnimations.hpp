#ifndef MOVEANIMATIONS_HPP
#define MOVEANIMATIONS_HPP

#include "Resources/ResourcePool.hpp"
#include "Media/Animation.hpp"
#include "Peoplemon/Move.hpp"

/**
 * This structure is to easily load and store all the animations associated with a given move
 *
 * \ingroup Battle
 * \ingroup Media
 */
struct MoveAnimations
{
    AnimationReference bgnd, fgnd, atk, def;
    Animation background, foreground, attacker, defender;

    /**
     * Loads the proper animations given the passed data
     *
     * \param pplmon The user's current peoplemon
     * \param op The name of the opponents peoplemon
     * \param m The move to load animations for
     * \param isPlayer True if the animations are being loaded for the players peoplemon
     */
    void load(std::string pplmon, std::string op, Move m, bool isPlayer);
};

#endif // MOVEANIMATIONS_HPP
