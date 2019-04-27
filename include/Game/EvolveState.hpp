#ifndef EVOLVESTATE_HPP
#define EVOLVESTATE_HPP

#include "Gamestate.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Resources/ResourcePool.hpp"
#include "Menu/MenuText.hpp"
#include "Media/Animation.hpp"

/**
 * This gamestate creates the visual effects of evolution and allows the player to cancel or allow
 *
 * \ingroup Game
 */
class EvolveState : public Gamestate
{
    PeoplemonRef* ppl;
    int nextPeoplemonId;
    bool evolved;

    TextureReference bgndTxtr, oldTxtr, newTxtr;
    sf::Sprite bgnd, newPpl, oldPpl;
    MenuText evolveText;
    Animation sparkles;

    /**
     * Runs the state
     */
    bool execute();

public:
    /**
     * Constructs the gamestate from the given data
     *
     * \param g A pointer to the main Game object
     * \param p A pointer to the peoplemon that is evolving
     * \param nextId The id of what to Evolve into. -1 to take from Peoplemon DB or use special logic
     */
    EvolveState(Game* g, PeoplemonRef* p, int nextId = -1);

    /**
     * Returns whether or not the player allowed the evolution to occur
     */
    bool evolutionSuccessful();
};

#endif // EVOLVESTATE_HPP
