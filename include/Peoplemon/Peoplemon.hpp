#ifndef PEOPLEMON_HPP
#define PEOPLEMON_HPP

#include <vector>
#include <string>
#include "SFML.hpp"
#include "Stats.hpp"
#include "Move.hpp"

class File;
class Game;

/**
 * \defgroup Peoplemon
 * \brief All classes related to the peoplemon themselves are in this module
 */

/**
 * Structure for storing data related to different types of peoplemon. Instances will exist in a global lookup table
 *
 * \ingroup Peoplemon
 */
struct Peoplemon  //there will exist one global vector of these
{
    int id;
    int numSeen, numCaught;
    std::string name, description;
    Type type;
    Stats baseStats;
    std::vector<int> validMoves;
    std::vector<std::pair<int,int> > learnSet; //(level,moveId)
    int evolveLevel, evolveNewId;
    Stats evAward;
    int xpYield, xpGroup;

    /**
     * Defines all of the different ailment types
     */
    enum Ailment
    {
        None = 0,
        Confused = 1,
        Annoyed = 2,
        Frustrated = 3,
        Stolen = 4,
        Sticky = 5,
        Sleep = 6,
        Frozen = 7,
        Trapped = 8,
        Distracted = 9,
        Guarded = 10
    };

    /**
     * Defines all of the special abilities that a Peoplemon can have
     */
	enum SpecialAbility
	{
		NoAbility = 0,
		Engage = 1,
		Lax = 2,
        LateRiser = 3,
        Observant = 4,
        Opinionated = 5,
        Forgiving = 6,
        Sassy = 7,
        Edumucator = 8,
        RunAway = 9,
        Questionable = 10,
        Extroverted = 11,
        Introverted = 12,
        DontPoke = 13,
        Bud = 14,
        Share = 15,
        Negotiator = 16,
        EasyGoing = 17
	}specialAbilityId;

    /**
     * Returns the STAB multiplier given the attacking and defending types
     *
     * \param atk The attacer's type
     * \param def The defender's type
     * \param mv The move type
     * \return The STAB multiplier
     */
    static double getDamageMultiplier(Type atk, Type def, Type mv);

    /**
     * Returns the multiplier for the given stat stage
     *
     * \param s The stage of the stat
     * \param isCrit Whether or not this is for the critical stat
     * \return The stat multiplier
     */
    static double getStatMultiplier(int s, bool isCrit);

    const static std::pair<std::string,std::string> abilityTexts[18]; //name and description of special abilities
    static double typeMultipliers[8][19];
};

/**
 * This structure is for storing actual peoplemon owned by the player and trainers
 *
 * \ingroup Peoplemon
 */
struct PeoplemonRef
{
    std::string name;
    int id; //to access most stuff
    Stats stats, ivs, evs, stages; //stats is recalculated whenever ivs or evs change
    int level, curXp, nextLvlXp;
    int holdItem;
    MoveRef moves[4]; //0 means no move
    int curHp;
    Peoplemon::Ailment curAils[4]; //first is the active ailment, each subsequent one is guaranteed to be passive
    int turnsWithAil, turnsConfused; //for tracking
    Peoplemon::SpecialAbility curAbility;

    /**
     * Saves the peoplemon's data to the given file
     *
     * \param file The file to save to
     */
    void save(File* file);

    /**
     * Loads the peoplmon from the given opened file
     *
     * \param g A pointer to the main Game object
     * \param file The file to load from
     */
    void load(Game* g, File* file);

    /**
     * Loads the peoplemon from an independant peoplemon file
     *
     * \param g A pointer to the main Game object
     * \param file The path of the file to load from
     */
    void load(Game* g, std::string file);

    /**
     * Recalculates the stats of the peoplemon based on its IV's and EV's
     */
    void recalcStats(Game* g);

    /**
     * Awards the given EVs to the peoplemon while enforcing the rules for adding
     *
     * \param g A pointer to the main Game object
     * \param evAward The EVs to award
     */
    void awardEVs(Game* g, Stats evAward);

    /**
     * Adds the passive ailment passed to this Peoplemon
     *
     * \param a The ailment to add
     */
    void addPassiveAilment(Peoplemon::Ailment a);

    /**
     * Removes the given passive ailment from this Peoplemon
     *
     * \param ail The ailment to remove
     */
    void removePassiveAilment(Peoplemon::Ailment ail);

    /**
     * Returns whether or not this Peoplemon has the given ailment
     *
     * \param ail The ailment to check for
     * \return Whether or not the Peoplemon has the given ailment
     */
    bool hasAilment(Peoplemon::Ailment ail);
};

//current formula for XP to next level: level^3 - (level-1)^3
//current forumla for XP award: isTrainer?(1.5:1) * fainted.xpYield * fainted.level * notEvolved?(1.2:1) / 7

/**
 * This structure is for storing data for wild peoplemon and for creating a PeoplemonRef when wilds are encountered
 *
 * \ingroup Peoplemon
 */
struct WildPeoplemon //TODO - http://www.gamefaqs.com/boards/696959-pokemon-x/68539129
{
    int id;
    int minL,maxL;
    int frequency; //any arbituary value, relative to other wild peoplemon's frequencies
    std::vector<std::pair<std::string,int> > overrides;

    /**
     * Loads the data from the given file
     *
     * \param file The file to load from
     */
    void load(std::string file);

    /**
     * Generates a PeoplemonRef using internal data
     *
     * \param g A pointer to the main Game object
     * \return A PeoplemonRef representing a wild peoplemon
     */
    PeoplemonRef getPeoplemon(Game* g);
};

/**
 * This structure is for representing a peoplemon stored in the storage system
 *
 * \ingroup Peoplemon
 */
struct StoredPeoplemon
{
    int boxId;
    sf::Vector2i position;
    PeoplemonRef data;

    /**
     * Initializes the structure with the given data
     *
     * \param b The number of the box that it should be in
     * \param pos The visual position of the peoplemon in its box
     * \param ppl The peoplemon to be stored
     */
    StoredPeoplemon(int b, sf::Vector2i pos, PeoplemonRef ppl);
};

#endif // PEOPLEMON_HPP
