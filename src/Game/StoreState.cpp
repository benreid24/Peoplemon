#include "Game/StoreState.hpp"
#include "Game/Game.hpp"
#include "Menu/Menu.hpp"
#include "Globals.hpp"
#include "Properties.hpp"
using namespace std;
using namespace sf;

StoreState::StoreState(Game* g, std::string prompt, std::string error, vector<pair<int, int> > i) : Gamestate(g)
{
    items = i;
    say = prompt;
    hobo = error;
    vector<int>* temp = game->player.getItems();
    for (unsigned int i = 0; i<temp->size(); ++i)
    {
        int t = temp->at(i);
        for (unsigned int j = 0; j<playerItems.size(); ++j)
        {
            if (playerItems[j].first==t)
            {
                playerItems[j].second++;
                goto found;
            }
            playerItems.push_back(make_pair(t,1));
            found:;
        }
    }
    if (game->player.pricesLowered())
	{
		for (unsigned int i = 0; i<items.size(); ++i)
		{
			items[i].second = double(items[i].second)*0.9;
		}
	}
    desc.setProps(Color::Black,22);
    desc.setPosition(Vector2f(30,410));
    title.setProps(Color::Black, 26);
    title.setPosition(Vector2f(490,70));
    money.setProps(Color::Black,22);
    money.setPosition(Vector2f(605,17));
}

bool StoreState::execute()
{
    const int MainPrompt = 0, Buying = 1, Selling = 2;
    int state = MainPrompt;

    MenuImage background("store.png");

    ChoiceBox mainChoice;
    mainChoice.addChoice("Buy");
    mainChoice.addChoice("Sell");
    mainChoice.addChoice("Exit");
    mainChoice.setBackgroundColor(Color::White);
    mainChoice.setBorder(Color::Black,4);
    mainChoice.setTextProps(Color::Black,24);
    mainChoice.setPosition(Vector2f(35,250));

    ScrollChoiceBox buyItems;
    for (unsigned int i = 0; i<items.size(); ++i)
    {
        buyItems.addChoice(createItem(game->itemList[items[i].first].name, items[i].second));
    }
    buyItems.addChoice("Back");
    buyItems.setBackgroundColor(Color::Transparent);
    buyItems.setTextProps(Color::Black,22);
    buyItems.setPosition(Vector2f(385,150));
    buyItems.setVisibleChoices(10); //change this

    ScrollChoiceBox sellItems;
    refreshSell:
        sellItems.clear();
    for (unsigned int i = 0; i<playerItems.size(); ++i)
    {
        sellItems.addChoice(createItem(game->itemList[playerItems[i].first].name+" x"+intToString(playerItems[i].second),game->itemList[playerItems[i].first].price));
    }
    sellItems.addChoice("Back");
    sellItems.setBackgroundColor(Color::Transparent);
    sellItems.setTextProps(Color::Black,22);
    sellItems.setPosition(Vector2f(385,150));
    sellItems.setVisibleChoices(10);

    sleep(milliseconds(225));
    while (!finishFrame())
    {
        money.setText("Money: "+intToString(game->player.getMoney()));

        game->mainWindow.clear();
        background.draw(&game->mainWindow);
        mainChoice.draw(&game->mainWindow);
        desc.draw(&game->mainWindow);
        title.draw(&game->mainWindow);
        money.draw(&game->mainWindow);

        switch (state)
        {
        case MainPrompt:
            mainChoice.update();
            desc.setText(wordWrap(say));
            title.setText("Items For Sale");

            if (mainChoice.getChoice()=="Buy")
            {
                sleep(milliseconds(225));
                mainChoice.reset();
                state = Buying;
            }
            if (mainChoice.getChoice()=="Sell")
            {
                sleep(milliseconds(225));
                mainChoice.reset();
                state = Selling;
            }
            if (mainChoice.getChoice()=="Exit" || user.isInputActive(PlayerInput::Run))
                return false;

            buyItems.draw(&game->mainWindow);

            break;
        case Buying:
            buyItems.update();
            title.setText("Items For Sale");
            if (buyItems.getCurrentChoice()!="Back")
                desc.setText(wordWrap(game->itemList[items[getItem(&items,buyItems.getCurrentChoice())].first].description));
            else
                desc.setText("Stop shopping");

            buyItems.draw(&game->mainWindow);

            if (buyItems.getChoice().size()>0 && buyItems.getChoice()!="Back")
            {
                pair<int,int> item = items[getItem(&items,buyItems.getChoice())];
                sleep(milliseconds(225));
                buyItems.reset();
                if (game->player.getMoney()<item.second)
                {
                    desc.setText(wordWrap(hobo));
                    game->mainWindow.clear();
                    background.draw(&game->mainWindow);
                    mainChoice.draw(&game->mainWindow);
                    buyItems.draw(&game->mainWindow);
                    desc.draw(&game->mainWindow);
                    title.draw(&game->mainWindow);
                    money.draw(&game->mainWindow);
                    game->mainWindow.display();
                    sleep(milliseconds(1300));
                }
                else
                {
                    desc.setText(wordWrap("How many would you like to buy?"));
                    ScrollChoiceBox amnt;
                    amnt.addChoice("Back");
                    for (int i = 1; i<=((item.second!=0)?(game->player.getMoney()/item.second):(99)); ++i)
                    {
                        amnt.addChoice(intToString(i));
                    }
                    amnt.setTextProps(Color::Black,22);
                    amnt.setPosition(Vector2f(135,250));
                    amnt.setVisibleChoices(1);
                    amnt.setBackgroundColor(Color::White);
                    amnt.setBorder(Color::Black,3);

                    while (amnt.getChoice().size()==0 && !user.isInputActive(PlayerInput::Run))
                    {
                        amnt.update();
                        if (amnt.getCurrentChoice()!="Back")
                            money.setText("Money: "+intToString(game->player.getMoney()-item.second*stringToInt(amnt.getCurrentChoice())));
                        else
                            money.setText("Money: "+intToString(game->player.getMoney()));
                        if (finishFrame())
                            return true;

                        game->mainWindow.clear();
                        background.draw(&game->mainWindow);
                        mainChoice.draw(&game->mainWindow);
                        buyItems.draw(&game->mainWindow);
                        desc.draw(&game->mainWindow);
                        title.draw(&game->mainWindow);
                        money.draw(&game->mainWindow);
                        amnt.draw(&game->mainWindow);
                        game->mainWindow.display();

                        sleep(milliseconds(30));
                    }

                    if (amnt.getChoice()!="Back" && amnt.getChoice().size()>0)
                    {
                        int q = stringToInt(amnt.getChoice());
                        game->player.alterMoney(-q*item.second);
                        for (int i = 0; i<q; i++)
                            game->player.giveItem(item.first);
                        addItem(item.first,q);
                        goto refreshSell;
                    }
                    sleep(milliseconds(225));
                }
            }

            if (buyItems.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
            {
                sleep(milliseconds(225));
                buyItems.reset();
                state = MainPrompt;
            }

            break;
        case Selling:
            sellItems.update();
            title.setText("Items You Can Sell");
            if (sellItems.getCurrentChoice()!="Back")
                desc.setText(wordWrap(game->itemList[playerItems[getItem(&playerItems,sellItems.getCurrentChoice())].first].description));
            else
                desc.setText("Stop Selling");
            sellItems.draw(&game->mainWindow);

            if (sellItems.getChoice().size()>0 && sellItems.getChoice()!="Back")
            {
                int x = getItem(&playerItems,sellItems.getChoice());
                pair<int,int> item = playerItems[x];

                sleep(milliseconds(225));
                sellItems.reset();
                desc.setText(wordWrap("How many would you like to sell?"));
                ScrollChoiceBox amnt;
                amnt.addChoice("Back");
                for (int i = 1; i<=item.second; ++i)
                {
                    amnt.addChoice(intToString(i));
                }
                amnt.setTextProps(Color::Black,22);
                amnt.setPosition(Vector2f(135,250));
                amnt.setVisibleChoices(1);
                amnt.setBackgroundColor(Color::White);
                amnt.setBorder(Color::Black,3);

                while (amnt.getChoice().size()==0 && !user.isInputActive(PlayerInput::Run))
                {
                    amnt.update();
                    if (amnt.getCurrentChoice()!="Back")
                        money.setText("Money: "+intToString(game->player.getMoney()+game->itemList[item.first].price*stringToInt(amnt.getCurrentChoice())));
                    else
                        money.setText("Money: "+intToString(game->player.getMoney()));
                    if (finishFrame())
                        return true;

                    game->mainWindow.clear();
                    background.draw(&game->mainWindow);
                    mainChoice.draw(&game->mainWindow);
                    sellItems.draw(&game->mainWindow);
                    desc.draw(&game->mainWindow);
                    title.draw(&game->mainWindow);
                    money.draw(&game->mainWindow);
                    amnt.draw(&game->mainWindow);
                    game->mainWindow.display();

                    sleep(milliseconds(30));
                }

                if (amnt.getChoice()!="Back" && amnt.getChoice().size()>0)
                {
                    int q = stringToInt(amnt.getChoice());
                    game->player.alterMoney(q*game->itemList[item.first].price);
                    game->player.takeItem(item.first,q);
                    playerItems[x].second -= q;
                    if (playerItems[x].second==0)
                        playerItems.erase(playerItems.begin()+x);
                    goto refreshSell;
                }
                sleep(milliseconds(225));
            }

            if (sellItems.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
            {
                sleep(milliseconds(225));
                sellItems.reset();
                state = MainPrompt;
            }

            break;
        default:
            cout << "Oh shit!\n";
            return true;
        }
        game->mainWindow.display();
        sleep(milliseconds(30));
    }

    return true;
}

string StoreState::wordWrap(string str)
{
    Text temp;
    temp.setFont(Properties::PrimaryMenuFont);
    temp.setCharacterSize(22);
    temp.setString(str);
    for (unsigned int i = 0; i<str.size(); ++i)
    {
        if (temp.findCharacterPos(i).x>=260)
        {
            for (int j = i; j>=0; j--)
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

string StoreState::createItem(string name, int price)
{
    Text temp;
    string r = name+intToString(price);
    temp.setFont(Properties::PrimaryMenuFont);
    temp.setCharacterSize(22);
    temp.setString(r);

    while (temp.findCharacterPos(r.size()-1).x<345)
    {
        name += " ";
        r = name+intToString(price);
        temp.setString(r);
    }

    return r;
}

int StoreState::getItem(vector<pair<int, int> >* vec, string str)
{
    for (unsigned int i = 0; i<vec->size(); ++i)
    {
        if (str.find(game->itemList[vec->at(i).first].name)!=string::npos)
            return i;
    }
    return 10000000; //so it crashes >:)
}

void StoreState::addItem(int id, int q)
{
    for (unsigned int i = 0; i<playerItems.size(); ++i)
    {
        if (playerItems[i].first==id)
        {
            playerItems[i].second += q;
            return;
        }
    }
    playerItems.push_back(make_pair(id,q));
}
