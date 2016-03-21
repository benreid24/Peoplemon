#include "Game/PeopledexState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
#include <iostream>
using namespace std;
using namespace sf;

PeopledexState::PeopledexState(Game* g) : Gamestate(g), peoplemonList(g)
{
    background.setImage("peopledexBgnd.png");
    pplSeen.setImage("pplDexSeen.png");
    pplOwned.setImage("pplDexOwned.png");
    pplSeen.setPosition(Vector2f(40,475));
    pplOwned.setPosition(Vector2f(40,540));
    pplSeenT.setPosition(Vector2f(125,490));
    pplOwnedT.setPosition(Vector2f(145,555));
    peoplemonPic.setPosition(Vector2f(45,80));
    peoplemonName.setPosition(Vector2f(45, 345));
    peoplemonDesc.setPosition(Vector2f(50, 375));
    peoplemonDesc.setProps(Color::Blue, 22);
    peoplemonName.setProps(Color::Red, 26);
    pplSeenT.setProps(Color::Red, 22);
    pplOwnedT.setProps(Color::Red, 22);
    update();
}

bool PeopledexState::execute()
{
    int cId = peoplemonList.getCurPeoplemon();
    while (!finishFrame())
    {
        peoplemonList.update();
        if (peoplemonList.getCurPeoplemon()!=cId)
        {
            cId = peoplemonList.getCurPeoplemon();
            update();
        }
        if (peoplemonList.isFinished() || user.isInputActive(PlayerInput::Run))
            return false;

        background.draw(&game->mainWindow);
        peoplemonList.draw(&game->mainWindow);
        pplSeen.draw(&game->mainWindow);
        pplOwned.draw(&game->mainWindow);
        if (peoplemonList.getCurPeoplemon()!=-1)
        {
            peoplemonPic.draw(&game->mainWindow);
            peoplemonName.draw(&game->mainWindow);
            peoplemonDesc.draw(&game->mainWindow);
            pplSeenT.draw(&game->mainWindow);
            pplOwnedT.draw(&game->mainWindow);
        }
        game->mainWindow.display();
        sleep(milliseconds(90));
    }
    return true;
}

string PeopledexState::wordWrap(string str)
{
    Text temp;
    temp.setFont(Properties::PrimaryMenuFont);
    temp.setCharacterSize(22);
    temp.setString(str);
    for (unsigned int i = 0; i<str.size(); ++i)
    {
        if (temp.findCharacterPos(i).x>=350)
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

void PeopledexState::update()
{
    if (peoplemonList.getCurPeoplemon()!=-1)
    {
        peoplemonName.setText(game->peoplemonList[peoplemonList.getCurPeoplemon()].name);
        peoplemonDesc.setText(wordWrap(game->peoplemonList[peoplemonList.getCurPeoplemon()].description));
        peoplemonPic.setImage(Properties::PeoplemonImagePath+intToString(peoplemonList.getCurPeoplemon())+".png", false);
        pplSeenT.setText(intToString(game->peoplemonList[peoplemonList.getCurPeoplemon()].numSeen));
        pplOwnedT.setText(intToString(game->peoplemonList[peoplemonList.getCurPeoplemon()].numCaught));
    }
}
