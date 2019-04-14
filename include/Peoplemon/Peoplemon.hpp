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
		BoardGameMaster = 1,
		Chillaxed = 2,
        Classy = 3,
        Goon = 4,
        QuickDraw = 5,
        AlwaysFriendly = 6,
        ImpulseBuy = 8,
        RunAway = 9,
        MrExtra = 10,
        BeefedUp = 11,
        Reserved = 12,
        DukeOfJokes = 13,
        Engaging = 14,
        SnackShare = 15,
        DerpDerp = 16,
        Klutz = 17,
        GenderBend = 18,
        Garbage = 19,
        BikeMechanic = 20,
        SidetrackTeach = 21,
        NoJokeTeach = 22,
        FieryTeach = 23,
        ExperiencedTeach = 24,
        NewTeach = 25,
        DozeOff = 26,
        DouseFlames = 27,
        Flirty = 28,
        UndyingFaith = 29,
        TooCool = 30,
        FakeStudy = 31,
        Alcoholic = 32,
        TotalBro = 33,
        TotalMom = 34,
        CantSwim = 35,
        AllNighter = 36,
        AilmentSaturated = 37,
        Adament = 38,
        AbsolutePitch = 39,
        GamemakerVirus = 40,
        Snapshot = 41,
        GetBaked = 42
	}specialAbilityId;

    /**
     * Returns the STAB multiplier given the peoplemon and move type
     *
     * \param atk The attacer's type
     * \param mv The move type
     * \return The STAB multiplier
     */
    static double getSTAB(Type atk, Type mv);

    /**
     * Returns the effectiveness multiplier based on move type and peoplemon type (getting hit)
     *
     * \param mv The move type
     * \param def The defender's type
     * \return The multiplier based on the types
     */
	static double getEffectivenessMultiplier(Type mv, Type def);

    /**
     * Returns the multiplier for the given stat stage
     *
     * \param s The stage of the stat
     * \param isCrit Whether or not this is for the critical stat
     * \return The stat multiplier
     */
    static double getStatMultiplier(int s, bool isCrit);

    const static std::pair<std::string,std::string> abilityTexts[43]; //name and description of special abilities
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
    int turnsUntilWake; //Rest

    /**
     * Default constructor. Simply zeros everything
     */
    PeoplemonRef();

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
     * Loads the peoplemon from an independent peoplemon file
     *
     * \param g A pointer to the main Game object
     * \param file The path of the file to load from
     */
    void load(Game* g, std::string file);

    /**
     * Recalculates the stats of the peoplemon based on its IV's and EV's
     */
    void recalcStats(Game* g, bool resetAbility = false);

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

    /**
     * Tells whether or not this Peoplemon has at least one ailment
     *
     * \return True if there is at least on ailment, false otherwise
     */
	bool hasAtLeastOneAilment();

	/**
	 * Returns the bonus multiplier for the given peopleball on this Peoplemon
	 *
	 * \param g A pointer to the main Game object
	 * \param b The id of the ball being used
	 * \param t The number of turns elapsed
	 * \param l The level of the opponent Peoplemon
	 */
	double getBallBonus(Game* g, int b, int t, int l);

    /**
     * Tells whether or not the peoplemon knows the given move
     *
     * \param m The id of the move to check for
     * \return Whether or not the peoplemon knows the move
     */
	bool knowsMove(int m);

	/**
	 * Teaches the peoplemon the given move. If an index is specified the move will overwrite that spot, otherwise the move is placed, if possible, into an empty spot
	 *
	 * \param g A pointer to the main Game object
	 * \param m The id of the move to learn
	 * \param i The index to place the move. Leave empty to put in empty slot
	 * \return Whether or not the move was taught
	 */
	bool teachMove(Game* g, int m, int i = -1);
};

//current formula for XP to next level: level^3 - (level-1)^3
//current forumla for XP award: isTrainer?(1.5:1) * fainted.xpYield * fainted.level * notEvolved?(1.2:1) / 7

/**
 * This structure is for storing data for wild peoplemon and for creating a PeoplemonRef when wilds are encountered
 *
 * \ingroup Peoplemon
 */
struct WildPeoplemon //http://www.gamefaqs.com/boards/696959-pokemon-x/68539129
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
