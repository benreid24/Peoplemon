#ifndef PEOPLEMONINFOSTATE_HPP
#define PEOPLEMONINFOSTATE_HPP

#include "Gamestate.hpp"
#include "Menu/Menu.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include <vector>

/**
 * Gamestate for displaying information on a given peoplemon
 *
 * \ingroup Game
 */
class PeoplemonInfoState : public Gamestate
{

    MenuImage background;

    MenuImage basicsBgnd, pplPic;
    MenuText name, id, level, type, hpStat, atkStat, defStat, spAtkStat, spDefStat, spdStat, ability, item, curXp, reqXp, status;
    Menu basicPage;

    /**
     * Struct to compactly pair data for moves
     */
    struct MoveText
    {
        int id; //move id
        MenuText name, pp;
    };
    MenuImage moveSel, moveUnsel;
    std::vector<MoveText> moves;
    MenuImage moveBgnd;
    MenuText moveAtk, moveAcc, moveType, moveDesc;
    Menu movePage;

    ChoiceBox pageChoice;
    bool doingMoves;
    int curMove;

    /**
     * Runs the state
     */
    bool execute();

    /**
     * Updates the move UI elements based on the current selection
     */
    void updateMoveStuff();

    /**
     * Wraps the given string to a certain size
     *
     * \param str The string to wrap
     * \param w The width to wrap to in pixels
     * \return The wrapped string
     */
    std::string wordWrap(std::string str, int w);

public:
    /**
     * Constructs the state from the given data
     *
     * \param g A pointer to the main Game object
     * \param n The next state to run
     */
    PeoplemonInfoState(Game* g, PeoplemonRef ppl);
};


#endif // PEOPLEMONINFOSTATE_HPP
