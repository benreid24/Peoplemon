#ifndef SCORE_HPP
#define SCORE_HPP

#include "ReactionPreference.hpp"

/**
 * Structure for storing the weights assigned to each scoring component
 *
 * \ingroup AI
 * \ingroup Battle
 */
struct Weights
{
    double level, cHealth, healthAdv, atkAdv, spAtkAdv, defAdv, spDefAdv, spdAdv, typeAdv, superMove, ailmentAdv;

    /**
     * Initializes all of the weights to 1
     */
    Weights()
    {
        level = 1;
        cHealth = 1;
        healthAdv = 1;
        atkAdv = 1;
        spAtkAdv = 1;
        defAdv = 1;
        spDefAdv = 1;
        spdAdv = 1;
        typeAdv = 1;
        superMove = 1;
        ailmentAdv = 1;
    }

    /**
     * Initializes all of the weights to the given parameters
     *
     * \param l The level weight
     * \param cH The current health weight
     * \param hA The relative health advantage weight
     * \param aA The attack advantage weight
     * \param saA The special attack advantage weight
     * \param dA The defensive advantage weight
     * \param sdA The special defense advantage weight
     * \param sA The speed advantage weight
     * \param tA The type advantage weight
     * \param mA The super effective move availability advantage weight
     * \param alA The ailment advantage weight
     */
    Weights(double l, double cH, double hA, double aA, double saA, double dA, double sdA, double sA, double tA, double mA, double alA)
    {
        level = l;
        cHealth = cH;
        healthAdv = hA;
        atkAdv = aA;
        spAtkAdv = saA;
        defAdv = dA;
        spDefAdv = sdA;
        spdAdv = sA;
        typeAdv = tA;
        superMove = mA;
        ailmentAdv = alA;
    }
};

/**
 * Structure for storing a score of a "position" in a battle
 *
 * \ingroup AI
 * \ingroup Battle
 */
struct Score
{
    double level, cHealth, healthAdv, atkAdv, spAtkAdv, defAdv, spDefAdv, spdAdv, typeAdv, superMove, ailmentAdv, total;

    /**
     * Restricts all of the score components to their respective ranges
     */
    void limit()
    {
        if (healthAdv<-1)
            healthAdv = -1;
        if (healthAdv>1)
            healthAdv = 1;
        if (level<-1)
            level = -1;
        if (level>1)
            level = 1;
        if (atkAdv>1)
            atkAdv = 1;
        if (spAtkAdv>1)
            spAtkAdv = 1;
        if (defAdv>1)
            defAdv = 1;
        if (spDefAdv>1)
            spDefAdv = 1;
        if (spdAdv<-1)
            spdAdv = -1;
        if (spdAdv>1)
            spdAdv = 1;
    }

    /**
     * Calculate the score total based on the components and given weights
     *
     * \param w The weights to use
     */
    void sum(Weights w)
    {
        total = level*w.level + cHealth*w.cHealth + healthAdv+w.healthAdv + atkAdv+w.atkAdv + spAtkAdv*w.spAtkAdv + defAdv*w.defAdv + spDefAdv*w.spDefAdv + spdAdv*w.spdAdv + superMove*w.superMove + typeAdv*w.typeAdv + ailmentAdv*w.ailmentAdv;
    }
};

/**
 * Structure for scoring a particular move used by one peoplemon on another
 *
 * \ingroup AI
 * \ingroup Battle
 */
struct MoveScore
{
    ReactionPreference::ReactionType type;
    double dmgScore;
    double effectScore; //these might have to be hard coded from a table of sorts
};

#endif // SCORE_HPP
