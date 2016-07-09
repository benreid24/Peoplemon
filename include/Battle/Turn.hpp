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
        Move = 0,
        Item = 1,
        Switch = 2,
        Run = 3,
        Void = 4
    }type;

    int id; //of the move or item, or the old index of the peoplemon being switched out

	/**
	 * Default constructor. Makes the type Void
	 */
    Turn() { type = Void; }
};

#endif // TURN_HPP
