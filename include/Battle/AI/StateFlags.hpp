#ifndef STATEFLAGS_HPP
#define STATEFLAGS_HPP

/**
 * Structure for storing state flags for a given position. Abstraction of a Score
 *
 * \ingroup AI
 * \ingroup Battle
 */
struct StateFlags
{
    bool atkAdv, defAdv;
    bool atkDisAdv, defDisAdv;
    bool spdAdv;
    bool likelyToDie, canKillOpp;
    bool switchAdv;
};

#endif // STATEFLAGS_HPP
