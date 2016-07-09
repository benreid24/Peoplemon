#ifndef REMOTEPLAYER_HPP
#define REMOTEPLAYER_HPP

#include "Packing.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include <vector>

/**
 * Container for storing a remote Player's information and Peoplemon
 *
 * \ingroup Entities
 * \ingroup Network
 */
struct RemotePlayer
{
	Packing::PlayerInfo info;
    std::vector<PeoplemonRef> peoplemon;

    /**
     * Heals all of the Peoplemon and resets their stages
     */
	void heal()
	{
		for (unsigned int i = 0; i<peoplemon.size(); ++i)
		{
			peoplemon[i].curHp = peoplemon[i].stats.hp;
            peoplemon[i].stages.zero();
			for (int j = 0; j<4; ++j)
				peoplemon[i].curAils[j] = Peoplemon::None;
		}
	}
};

#endif // REMOTEPLAYER_HPP
