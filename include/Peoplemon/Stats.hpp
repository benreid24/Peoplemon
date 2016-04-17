#ifndef STATS_HPP
#define STATS_HPP

#include <iostream>

/**
 * Strcuture for storing all of the stats associated with a peoplemon
 *
 * \ingroup Peoplemon
 */
struct Stats
{
    int hp, atk, def, spAtk, spDef, acc, evade, spd, crit;

    /**
     * Adds the passed Stats to the stats contained internally
     *
     * \param r The Stats to add to this instance
     */
    Stats operator+ (const Stats& r)
    {
        Stats t;
        t.hp = hp+r.hp;
        t.atk = atk+r.atk;
        t.def = r.def+def;
        t.spAtk = r.spAtk+spAtk;
        t.spDef = r.spDef+spDef;
        t.acc = acc+r.acc;
        t.evade = evade+r.evade;
        t.spd = spd+r.spd;
        t.crit = crit+r.crit;
        return t;
    }

    /**
     * Zeros all the stats. This is to be used by battles for storing stages
     */
    void zero()
    {
        hp = 0;
        atk = 0;
        def = 0;
        spAtk = 0;
        spDef = 0;
        acc = 0;
        evade = 0;
        spd = 0;
        crit = 0;
    }

    /**
     * Returns the sum of the 6 stats that are used for EV calculations
     */
    int sum()
    {
        return (hp+atk+def+spd+spAtk+spDef);
    }

    /**
     * Prints all stats for debug purposes
     */
	void print()
	{
		std::cout << "Hp: " << hp << std::endl;
		std::cout << "Atk: " << atk << std::endl;
		std::cout << "Def: " << def << std::endl;
		std::cout << "SpAtk: " << spAtk << std::endl;
		std::cout << "SpDef: " << spDef << std::endl;
		std::cout << "Spd: " << spd << std::endl;
		std::cout << "Evade: " << evade << std::endl;
		std::cout << "Crit: " << crit << std::endl;
	}

    /**
     * Caps the 6 EV stats to the passed value
     *
     * \param c The value to cap the EV stats at
     */
    void cap(int c)
    {
        if (hp>c)
            hp = c;
        if (atk>c)
            atk = c;
        if (def>c)
            def = c;
        if (spAtk>c)
            spAtk = c;
        if (spDef>c)
            spDef = c;
        if (spd>c)
            spd = c;
    }

    /**
     * Default constructor. Simply zeros all the stats
     */
    Stats()
    {
    	zero();
    }
};

#endif // STATS_HPP
