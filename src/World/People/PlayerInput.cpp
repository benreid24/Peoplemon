#include "World/People/PlayerInput.hpp"
#include "SFML.hpp"
#include "Properties.hpp"
using namespace sf;

PlayerInput::PlayerInput()
{
	type = None;
	Joystick::update();
	for (int i = 0; i<Joystick::Count; ++i)
	{
		if (Joystick::isConnected(i))
		{
			if (Joystick::getButtonCount(i)==10)
			{
				js = i;
				if (Joystick::hasAxis(js,Joystick::Z))
					type = Xbox;
				else
					type = NES;
			}
		}
	}
}

void PlayerInput::update()
{
	js = -1;
	type = None;
	for (int i = 0; i<Joystick::Count; ++i)
	{
		if (Joystick::isConnected(i))
		{
			if (Joystick::getButtonCount(i)==10)
			{
				js = i;
				if (Joystick::hasAxis(js,Joystick::Z))
					type = Xbox;
				else
					type = NES;
			}
		}
	}
}

bool PlayerInput::isInputActive(Input i)
{
	update();

	switch (i)
	{
	case Up:
		return Keyboard::isKeyPressed(Properties::upKey) || ((type!=None)?(Joystick::getAxisPosition(js,Joystick::Y)<-60):(false));
	case Right:
		return Keyboard::isKeyPressed(Properties::rightKey) || ((type!=None)?(Joystick::getAxisPosition(js,Joystick::X)>60):(false));
	case Left:
		return Keyboard::isKeyPressed(Properties::leftKey) || ((type!=None)?(Joystick::getAxisPosition(js,Joystick::X)<-60):(false));
	case Down:
		return Keyboard::isKeyPressed(Properties::downKey) || ((type!=None)?(Joystick::getAxisPosition(js,Joystick::Y)>60):(false));
	case Interact:
		return Keyboard::isKeyPressed(Properties::interactKey) || ((type==Xbox)?(Joystick::isButtonPressed(js,0)):(false)) || ((type==NES)?(Joystick::isButtonPressed(js,1)):(false));
	case Run:
		return Keyboard::isKeyPressed(Properties::runKey) || ((type==Xbox)?(Joystick::isButtonPressed(js,1)):(false)) || ((type==NES)?(Joystick::isButtonPressed(js,0)):(false));
	case Pause:
		return Keyboard::isKeyPressed(Properties::pauseKey) || ((type==Xbox)?(Joystick::isButtonPressed(js,7)):(false)) || ((type==NES)?(Joystick::isButtonPressed(js,9)):(false));
	default:
		return false;
	}
}
