#include "Game/DeleteMoveState.hpp"
#include "Game/Game.hpp"
#include "Properties.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

DeleteMoveState::DeleteMoveState(Game* g, PeoplemonRef* p, int m) : Gamestate(g,NULL)
{
    ppl = p;
    taught = false;
    for (int i = 0; i<4; ++i)
        moves[i] = p->moves[i].id;
    moves[4] = m;

    bgnd.setImage("delMoveWindow.png");
    power.setPosition(Vector2f(220,220));
    acc.setPosition(Vector2f(220,275));
    type.setPosition(Vector2f(200,330));
    desc.setPosition(Vector2f(50,405));
    power.setProps(Color::White,22);
    acc.setProps(Color::White,22);
    type.setProps(Color::White,22);
    desc.setProps(Color::White,20);

    for (int i = 0; i<5; ++i)
    {
        string nm = game->moveList[moves[i]].name;
        choices.addChoice(nm);
        revLookup[nm] = moves[i];
    }
    choices.setBorder(Color(30,30,30),4);
    choices.setBackgroundColor(Color(90,90,90));
    choices.setPosition(Vector2f(400,175));
    choices.setTextProps(Color::White,28);

    menu.add(&bgnd);
    menu.add(&desc);
    menu.add(&acc);
    menu.add(&type);
    menu.add(&power);
    menu.add(&choices);
}

string DeleteMoveState::wordWrap(string str)
{
    Text temp;
    temp.setFont(Properties::PrimaryMenuFont);
    temp.setCharacterSize(20);
    temp.setString(str);
    for (unsigned int i = 0; i<str.size(); ++i)
    {
        if (temp.findCharacterPos(i).x>=230)
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

bool DeleteMoveState::execute()
{
    while (!finishFrame())
    {
        choices.update();
        if (choices.getChoice().size()>0)
        {
            string message;
            int ch = revLookup[choices.getChoice()];
            if (ch==4)
                message = "Stop trying to learn "+choices.getChoice()+"?";
            else
                message = "Forget "+choices.getChoice()+" and learn "+game->moveList[moves[4]].name+"?";

            game->hud.getChoice(message,vector<string>({"Yes","No"}),false);

            while (game->hud.getChoice().size()==0)
            {
                if (finishFrame())
                    return true;

                game->hud.update();

                menu.draw(&game->mainWindow);
                game->hud.draw(&game->mainWindow);
                game->mainWindow.display();
                sleep(milliseconds(30));
            }

            if (game->hud.getChoice()=="Yes")
            {
                message = ppl->name+" stopped trying to learn "+game->moveList[moves[4]].name+"!";
                if (ch!=4)
                {
                	message = ppl->name+" forgot "+game->moveList[moves[ch]].name+" and learned "+game->moveList[moves[4]].name+"!";
                    ppl->moves[ch].id = moves[ch];
                    ppl->moves[ch].curPp = game->moveList[moves[ch]].pp;
                    taught = true;
                }

                game->hud.displayMessage(message);
                while (!game->hud.messageFinished())
                {
                    if (finishFrame())
                        return true;
                    game->hud.update();

                    menu.draw(&game->mainWindow);
                    game->hud.draw(&game->mainWindow);
                    sleep(milliseconds(30));
                }

                return false;
            }

            choices.reset(); //just continue with menu
            sleep(milliseconds(225));
        }

		int lMove = revLookup[choices.getCurrentChoice()];
		power.setText(intToString(game->moveList[lMove].dmg));
		acc.setText(intToString(game->moveList[lMove].acc));
		type.setText(game->typeList[game->moveList[lMove].type]);
		desc.setText(wordWrap(game->moveList[lMove].description));

        menu.draw(&game->mainWindow);
        game->mainWindow.display();
        sleep(milliseconds(30));
    }

    return true;
}

bool DeleteMoveState::moveLearned()
{
	return taught;
}
