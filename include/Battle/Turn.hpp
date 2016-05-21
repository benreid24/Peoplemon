#ifndef TURN_HPP
#define TURN_HPP

#include "Peoplemon/Peoplemon.hpp"

/**
 * Structure to represent the action taken by a Battler on their turn
 *
 * \ingroup Battle
 */
struct Turn
{
    /**
     * Defines the different types of actions
     */
    enum Type
    {
        Move,
        Item,
        Switch,
        Run
    }type;

    int id; //of the move or item, or the old index of the peoplemon being switched out
};

#endif // TURN_HPP
