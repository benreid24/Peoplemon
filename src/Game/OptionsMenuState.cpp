#include "Game/OptionsMenuState.hpp"
#include "Globals.hpp"
#include "Game/Game.hpp"
#include "Menu/Menu.hpp"
#include "Properties.hpp"
#include "version.h"
using namespace std;
using namespace sf;

OptionsMenuState::OptionsMenuState(Game* g, Gamestate* n) : Gamestate(g,n)
{
    //ctor
}

bool OptionsMenuState::execute()
{
    Menu menu;
    MenuImage background("optionsBgnd.png");
    ChoiceBox optBox;

    optBox.addChoice("Controls");
    optBox.addChoice("Sound");
    optBox.addChoice("Display");
    optBox.addChoice("Back");
    optBox.setTextProps(Color::Black,30);
    optBox.setBackgroundColor(Color::Transparent);
    optBox.setPosition(Vector2f(150,200));

    menu.add(&background);
    menu.add(&optBox);
    sleep(milliseconds(200));

    while (!finishFrame())
    {
        optBox.update();

        if (optBox.getChoice()=="Controls")
        {
            sleep(milliseconds(200));

            MenuText upText, rightText, downText, leftText, interactText, runText, pauseText;
            upText.setProps(Color::Black,25);
            rightText.setProps(Color::Black,25);
            downText.setProps(Color::Black,25);
            leftText.setProps(Color::Black,25);
            interactText.setProps(Color::Black,25);
            runText.setProps(Color::Black,25);
            pauseText.setProps(Color::Black,25);
            upText.setPosition(Vector2f(440,200));
            rightText.setPosition(Vector2f(440,225));
            downText.setPosition(Vector2f(440,250));
            leftText.setPosition(Vector2f(440,275));
            interactText.setPosition(Vector2f(440,300));
            runText.setPosition(Vector2f(440,325));
            pauseText.setPosition(Vector2f(440,350));
            upText.setText("Move Up: "+keyToString(Properties::upKey));
            rightText.setText("Move Right: "+keyToString(Properties::rightKey));
            downText.setText("Move Down: "+keyToString(Properties::downKey));
            leftText.setText("Move Left: "+keyToString(Properties::leftKey));
            interactText.setText("Interact: "+keyToString(Properties::interactKey));
            runText.setText("Run/Back: "+keyToString(Properties::runKey));
            pauseText.setText("Pause: "+keyToString(Properties::pauseKey));

            MenuText prompt;
            prompt.setPosition(Vector2f(100,100));
            prompt.setProps(Color::Black,40);
            prompt.setText("Press the key you want to map to the control");

            ChoiceBox opts;
            opts.setTextProps(Color::Black, 25);
            opts.setPosition(Vector2f(300,200));
            opts.addChoice("Move Up");
            opts.addChoice("Move Right");
            opts.addChoice("Move Down");
            opts.addChoice("Move Left");
            opts.addChoice("Interact");
            opts.addChoice("Run/Back");
            opts.addChoice("Pause");
            opts.addChoice("Back");

            while (true)
            {
                opts.update();

                if (opts.getChoice().size()>0 && opts.getChoice()!="Back")
				{
					sleep(milliseconds(250));
					if (finishFrame())
						return true;
					prompt.draw(&game->mainWindow);
					game->mainWindow.display();
					Keyboard::Key key;

					while (true)
					{
						Event evt;

						while (game->mainWindow.pollEvent(evt))
						{
							if (evt.type==Event::Closed)
								return true;
							if (evt.type==Event::KeyPressed)
							{
								key = evt.key.code;
                                goto pressed;
							}
						}

						sleep(milliseconds(40));
					}
					pressed:

					if (opts.getChoice()=="Move Up")
					{
						Properties::upKey = key;
						upText.setText("Move Up: "+keyToString(key));
					}
					else if (opts.getChoice()=="Move Right")
					{
						Properties::rightKey = key;
						rightText.setText("Move Right: "+keyToString(key));
					}
					else if (opts.getChoice()=="Move Down")
					{
						Properties::downKey = key;
						downText.setText("Move Down: "+keyToString(key));
					}
					else if (opts.getChoice()=="Move Left")
					{
						Properties::leftKey = key;
						leftText.setText("Move Left: "+keyToString(key));
					}
					else if (opts.getChoice()=="Interact")
					{
						Properties::interactKey = key;
						interactText.setText("Interact: "+keyToString(key));
					}
					else if (opts.getChoice()=="Run/Back")
					{
						Properties::runKey = key;
						runText.setText("Run/Back: "+keyToString(key));
					}
					else if (opts.getChoice()=="Pause")
					{
						Properties::pauseKey = key;
						pauseText.setText("Pause: "+keyToString(key));
					}
					opts.reset();
					sleep(milliseconds(300));
				}
				else if (opts.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
					break;

                if (finishFrame())
                    return true;

                menu.draw(&game->mainWindow);
                opts.draw(&game->mainWindow);
                upText.draw(&game->mainWindow);
                rightText.draw(&game->mainWindow);
                downText.draw(&game->mainWindow);
                leftText.draw(&game->mainWindow);
                interactText.draw(&game->mainWindow);
                runText.draw(&game->mainWindow);
                pauseText.draw(&game->mainWindow);
                game->mainWindow.display();
                sleep(milliseconds(30));
            }
            sleep(milliseconds(200));
            optBox.reset();
        }
        if (optBox.getChoice()=="Sound")
        {
            sleep(milliseconds(200));
            ChoiceBox opts;
            opts.setTextProps(Color::Black,25);
            opts.setPosition(Vector2f(300,200));
            opts.addChoice("On");
            opts.addChoice("Off");

            while (true)
            {
                opts.update();

                if (opts.getChoice()=="On")
                {
                    game->data.gameMuted = false;
                    break;
                }
                if (opts.getChoice()=="Off")
                {
                    game->data.gameMuted = true;
                    break;
                }
                if (user.isInputActive(PlayerInput::Run))
					break;

                if (finishFrame())
                    return true;

                menu.draw(&game->mainWindow);
                opts.draw(&game->mainWindow);
                game->mainWindow.display();
                sleep(milliseconds(30));
            }
            sleep(milliseconds(200));
            optBox.reset();
        }
        if (optBox.getChoice()=="Display")
        {
            bool wasFull = game->data.fullscreen;
            ChoiceBox fBox;

            fBox.addChoice("Fullscreen");
            fBox.addChoice("Windowed");
            fBox.setTextProps(Color::Black,30);
            fBox.setPosition(Vector2f(260,300));
            sleep(milliseconds(200));

            while (true)
			{
				fBox.update();

				if (fBox.getChoice()=="Fullscreen")
				{
					game->data.fullscreen = true;
					break;
				}
				if (fBox.getChoice()=="Windowed")
				{
					game->data.fullscreen = false;
					break;
				}
				if (finishFrame())
					return true;

				if (user.isInputActive(PlayerInput::Run))
					goto noChangeScreen;

				menu.draw(&game->mainWindow);
				fBox.draw(&game->mainWindow);
				game->mainWindow.display();
				sleep(milliseconds(30));
			}
			if (wasFull!=game->data.fullscreen)
			{
				if (game->data.fullscreen)
				{
					game->mainWindow.close();
					game->mainWindow.create(VideoMode(Properties::ScreenWidth,Properties::ScreenHeight,32), "Peoplemon v"+string(AutoVersion::FULLVERSION_STRING), Style::Fullscreen);
					game->mainWindow.setMouseCursorVisible(false);
					game->mainWindow.setVerticalSyncEnabled(true);
				}
				else
				{
					game->mainWindow.close();
					game->mainWindow.create(VideoMode(Properties::ScreenWidth,Properties::ScreenHeight,32), "Peoplemon v"+string(AutoVersion::FULLVERSION_STRING), Style::Titlebar|Style::Close);
					game->mainWindow.setMouseCursorVisible(false);
					game->mainWindow.setVerticalSyncEnabled(true);
				}
			}
			noChangeScreen:
			sleep(milliseconds(200));
			optBox.reset();
        }
        if (optBox.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
            return false;


        menu.draw(&game->mainWindow);
        game->mainWindow.display();
        sleep(milliseconds(30));
    }

    return true;
}
