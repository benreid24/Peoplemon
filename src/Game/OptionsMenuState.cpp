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
    cout << "Options menu\n";
    //TODO - present list of things to change (controls, sound, theme?)
    //have little submenus right in here for each thing to change

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
            ChoiceBox opts;
            opts.setTextProps(Color::Black, 25);
            opts.setPosition(Vector2f(300,200));
            opts.addChoice("Movement");
            opts.addChoice("Interaction");
            opts.addChoice("Running");
            opts.addChoice("Pause");
            opts.addChoice("Back");

            while (true)
            {
                opts.update();

                if (opts.getChoice()=="Movement")
                {
                    sleep(milliseconds(200));
                    ChoiceBox inner;
                    inner.setTextProps(Color::Black, 22);
                    inner.setPosition(Vector2f(450,200));
                    inner.addChoice("WASD");
                    inner.addChoice("Arrow Keys (Default)");
                    inner.addChoice("Back");

                    while (true)
                    {
                        inner.update();

                        if (inner.getChoice()=="WASD")
                        {
                            Properties::upKey = Keyboard::W;
                            Properties::rightKey = Keyboard::D;
                            Properties::downKey = Keyboard::S;
                            Properties::leftKey = Keyboard::A;
                            break;
                        }
                        if (inner.getChoice()=="Arrow Keys (Default)")
                        {
                            Properties::upKey = Keyboard::Up;
                            Properties::rightKey = Keyboard::Right;
                            Properties::downKey = Keyboard::Down;
                            Properties::leftKey = Keyboard::Left;
                            break;
                        }
                        if (inner.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
                            break;

                        if (finishFrame())
                            return true;

                        menu.draw(&game->mainWindow);
                        inner.draw(&game->mainWindow);
                        opts.draw(&game->mainWindow);
                        game->mainWindow.display();
                        sleep(milliseconds(30));
                    }
                    sleep(milliseconds(200));
                    opts.reset();
                }
                if (opts.getChoice()=="Interaction")
                {
                    sleep(milliseconds(200));
                    ScrollChoiceBox inner;
                    inner.setTextProps(Color::Black, 22);
                    inner.setPosition(Vector2f(450,200));
                    inner.setVisibleChoices(6);
                    inner.addChoice("Back");
                    inner.addChoice("Z");
                    inner.addChoice("X");
                    inner.addChoice("C");
                    inner.addChoice("V (Default)");
                    inner.addChoice("B");
                    inner.addChoice("N");
                    inner.addChoice("M");
                    inner.addChoice("L");
                    inner.addChoice("K");
                    inner.addChoice("J");

                    while (true)
                    {
                        inner.update();

                        if (inner.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
                            break;
                        else if (inner.getChoice().size()>0)
                        {
                            setPlayerControl(&Properties::interactKey, inner.getChoice());
                            break;
                        }

                        if (finishFrame())
                            return true;

                        menu.draw(&game->mainWindow);
                        inner.draw(&game->mainWindow);
                        opts.draw(&game->mainWindow);
                        game->mainWindow.display();
                        sleep(milliseconds(30));
                    }
                    sleep(milliseconds(200));
                    opts.reset();
                }
                if (opts.getChoice()=="Running")
                {
                    sleep(milliseconds(200));
                    ChoiceBox inner;
                    inner.setTextProps(Color::Black, 22);
                    inner.setPosition(Vector2f(450,200));
                    inner.addChoice("Space (Default)");
                    inner.addChoice("Left Alt");
                    inner.addChoice("Right Alt");
                    inner.addChoice("Back");

                    while (true)
                    {
                        inner.update();

                        if (inner.getChoice()=="Space (Default)")
                        {
                            Properties::runKey = Keyboard::Space;
                            break;
                        }
                        if (inner.getChoice()=="Left Alt")
                        {
                            Properties::runKey = Keyboard::LAlt;
                            break;
                        }
                        if (inner.getChoice()=="Right Alt")
                        {
                            Properties::runKey = Keyboard::RAlt;
                            break;
                        }
                        if (inner.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
                            break;

                        if (finishFrame())
                            return true;

                        menu.draw(&game->mainWindow);
                        inner.draw(&game->mainWindow);
                        opts.draw(&game->mainWindow);
                        game->mainWindow.display();
                        sleep(milliseconds(30));
                    }
                    sleep(milliseconds(200));
                    opts.reset();
                }
                if (opts.getChoice()=="Pause")
                {
                    sleep(milliseconds(200));
                    ChoiceBox inner;
                    inner.setTextProps(Color::Black, 22);
                    inner.setPosition(Vector2f(450,200));
                    inner.addChoice("Enter (Default)");
                    inner.addChoice("Right Shift");
                    inner.addChoice("Left Shift");
                    inner.addChoice("Backspace");
                    inner.addChoice("Back");

                    while (true)
                    {
                        inner.update();

                        if (inner.getChoice()=="Enter (Default)")
                        {
                            Properties::pauseKey = Keyboard::Return;
                            break;
                        }
                        if (inner.getChoice()=="Right Shift")
                        {
                            Properties::pauseKey = Keyboard::RShift;
                            break;
                        }
                        if (inner.getChoice()=="Left Shift")
                        {
                            Properties::pauseKey = Keyboard::LShift;
                            break;
                        }
                        if (inner.getChoice()=="Backspace")
                        {
                            Properties::pauseKey = Keyboard::BackSpace;
                            break;
                        }
                        if (inner.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
                            break;

                        if (finishFrame())
                            return true;

                        menu.draw(&game->mainWindow);
                        inner.draw(&game->mainWindow);
                        opts.draw(&game->mainWindow);
                        game->mainWindow.display();
                        sleep(milliseconds(30));
                    }
                    sleep(milliseconds(200));
                    opts.reset();
                }
                if (opts.getChoice()=="Back" || user.isInputActive(PlayerInput::Run))
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
            game->music.update();
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
