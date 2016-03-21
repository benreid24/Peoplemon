#include "Game/PeoplemonInfoState.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

PeoplemonInfoState::PeoplemonInfoState(Game* g, PeoplemonRef ppl) : Gamestate(g,NULL)
{
    doingMoves = false;
    curMove = 0;
    background.setImage("pplInfoBgnd.png");
    pageChoice.addChoice("Basic Info");
    pageChoice.addChoice("Moves");
    pageChoice.addChoice("Back");
    pageChoice.setTextProps(Color::White,22);
    pageChoice.setBackgroundColor(Color(130,130,130));
    pageChoice.setBorder(Color(245,245,245),3);
    pageChoice.setPosition(Vector2f(700,510));

    basicsBgnd.setImage("pplBasicsBgnd.png");
    pplPic.setImage(Properties::PeoplemonImagePath+intToString(ppl.id)+".png",false);
    name.setText(g->peoplemonList[ppl.id].name);
    level.setText(intToString(ppl.level));
    id.setText(intToString(ppl.id));
    type.setText("Type: "+g->typeList[g->peoplemonList[ppl.id].type]);
    hpStat.setText("HP: "+intToString(ppl.curHp)+"/"+intToString(ppl.stats.hp));
    atkStat.setText("Atk: "+intToString(ppl.stats.atk));
    defStat.setText("Def: "+intToString(ppl.stats.def));
    spAtkStat.setText("SpAtk: "+intToString(ppl.stats.spAtk));
    spDefStat.setText("SpDef: "+intToString(ppl.stats.spDef));
    spdStat.setText("Spd: "+intToString(ppl.stats.spd));
    ability.setText("Ability: "+Peoplemon::abilityTexts[g->peoplemonList[ppl.id].specialAbilityId].first+"\n"+wordWrap(Peoplemon::abilityTexts[g->peoplemonList[ppl.id].specialAbilityId].second,400));
    item.setText(g->itemList[ppl.holdItem].name);
    curXp.setText(intToString(ppl.curXp));
    reqXp.setText(intToString(ppl.nextLvlXp));
    status.setText(g->ailmentList[ppl.curAils[0]]);

    basicsBgnd.setPosition(Vector2f(345,75));
    pplPic.setPosition(Vector2f(25,160));
    name.setPosition(Vector2f(32,75));
    level.setPosition(Vector2f(256,113));
    id.setPosition(Vector2f(60,113));
    type.setPosition(Vector2f(385,152));
    hpStat.setPosition(Vector2f(400,212));
    atkStat.setPosition(Vector2f(400,265));
    defStat.setPosition(Vector2f(400,318));
    spAtkStat.setPosition(Vector2f(580,270));
    spDefStat.setPosition(Vector2f(580,320));
    spdStat.setPosition(Vector2f(580,370));
    ability.setPosition(Vector2f(375,423));
    item.setPosition(Vector2f(75,465));
    curXp.setPosition(Vector2f(102,486));
    reqXp.setPosition(Vector2f(98,515));
    status.setPosition(Vector2f(163,568));

    name.setProps(Color::White,30);
    level.setProps(Color::White,24);
    id.setProps(Color::White,22);
    type.setProps(Color::White,22);
    hpStat.setProps(Color::White,22);
    atkStat.setProps(Color::White,22);
    defStat.setProps(Color::White,22);
    spAtkStat.setProps(Color::White,22);
    spDefStat.setProps(Color::White,22);
    spdStat.setProps(Color::White,22);
    ability.setProps(Color::White,21);
    item.setProps(Color::White,22);
    curXp.setProps(Color::White,20);
    reqXp.setProps(Color::White,20);
    status.setProps(Color::White,22);

    basicPage.add(&background);
    basicPage.add(&basicsBgnd);
    basicPage.add(&pplPic);
    basicPage.add(&level);
    basicPage.add(&id);
    basicPage.add(&type);
    basicPage.add(&hpStat);
    basicPage.add(&atkStat);
    basicPage.add(&defStat);
    basicPage.add(&spAtkStat);
    basicPage.add(&spDefStat);
    basicPage.add(&spdStat);
    basicPage.add(&ability);
    basicPage.add(&item);
    basicPage.add(&curXp);
    basicPage.add(&reqXp);
    basicPage.add(&status);
    basicPage.add(&pageChoice);
    basicPage.add(&name);

    moveSel.setImage("pplMoveSel.png");
    moveUnsel.setImage("pplMoveUnsel.png");
    moveBgnd.setImage("pplMoveDesc.png");
    moveAtk.setProps(Color::White,24);
    moveAcc.setProps(Color::White,24);
    moveType.setProps(Color::White,24);
    moveDesc.setProps(Color::White, 22);

    moveBgnd.setPosition(Vector2f(0,141));
    moveAtk.setPosition(Vector2f(210,215));
    moveAcc.setPosition(Vector2f(210,270));
    moveType.setPosition(Vector2f(187,325));
    moveDesc.setPosition(Vector2f(42,390));

    int j = 0;
    for (int i = 0; i<4; ++i)
    {
        if (ppl.moves[i].id!=0)
        {
            MoveText temp;
            temp.id = ppl.moves[i].id;
            temp.name.setText(g->moveList[ppl.moves[i].id].name);
            temp.pp.setText("PP: "+intToString(ppl.moves[i].curPp)+"/"+intToString(g->moveList[ppl.moves[i].id].pp));
            temp.name.setPosition(Vector2f(390,105+j*113));
            temp.pp.setPosition(Vector2f(619,145+j*113));
            temp.name.setProps(Color(30,30,30),44);
            temp.pp.setProps(Color(20,20,20),26);
            moves.push_back(temp);
            ++j;
        }
    }
    updateMoveStuff();

    movePage.add(&background);
    movePage.add(&moveBgnd);
    movePage.add(&moveAcc);
    movePage.add(&moveAtk);
    movePage.add(&moveType);
    movePage.add(&moveDesc);
    movePage.add(&name);
    movePage.add(&id);
    movePage.add(&level);
}

string PeoplemonInfoState::wordWrap(string str, int w)
{
    Text temp;
    temp.setFont(Properties::PrimaryMenuFont);
    temp.setCharacterSize(21);
    temp.setString(str);
    for (unsigned int i = 0; i<str.size(); ++i)
    {
        if (temp.findCharacterPos(i).x>=w)
        {
            for (unsigned int j = i; j>=0; j--)
            {
                if (str[j]==' ')
                {
                    str.insert(j,"\n");
                    temp.setString(str);
                    break;
                }
            }
        }
    }
    return str;
}

void PeoplemonInfoState::updateMoveStuff()
{
    moveAtk.setText(intToString(game->moveList[moves[curMove].id].dmg));
    moveAcc.setText(intToString(game->moveList[moves[curMove].id].acc));
    moveType.setText(game->typeList[game->moveList[moves[curMove].id].type]);
    moveDesc.setText(wordWrap(game->moveList[moves[curMove].id].description,270));
}

bool PeoplemonInfoState::execute()
{
    while (!finishFrame())
    {
        if (doingMoves)
        {
            if (user.isInputActive(PlayerInput::Up) && curMove>0)
            {
                curMove--;
                updateMoveStuff();
            }
            if (user.isInputActive(PlayerInput::Down) && unsigned(curMove)<moves.size()-1)
            {
                curMove++;
                updateMoveStuff();
            }
            if (user.isInputActive(PlayerInput::Interact) || user.isInputActive(PlayerInput::Run))
			{
				sleep(milliseconds(250));
				doingMoves = false;
			}
        }
        else
            pageChoice.update();

        if (pageChoice.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
            return false;
        else if (pageChoice.getChoice()=="Moves")
            doingMoves = true;
        pageChoice.reset();

        if (pageChoice.getCurrentChoice()=="Moves")
        {
            movePage.draw(&game->mainWindow);
            if (!doingMoves)
                pageChoice.draw(&game->mainWindow);
            for (unsigned int i = 0; i<moves.size(); ++i)
            {
                if (unsigned(curMove)==i)
                {
                    moveSel.setPosition(Vector2f(340,70+i*113));
                    moveSel.draw(&game->mainWindow);
                }
                else
                {
                    moveUnsel.setPosition(Vector2f(340,70+i*113));
                    moveUnsel.draw(&game->mainWindow);
                }
                moves[i].name.draw(&game->mainWindow);
                moves[i].pp.draw(&game->mainWindow);
            }
        }
        else
            basicPage.draw(&game->mainWindow);
        game->mainWindow.display();
        sleep(milliseconds(100));

    }

    return true;
}
