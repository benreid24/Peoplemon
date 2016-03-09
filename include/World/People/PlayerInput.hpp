#ifndef PLAYERINPUT_HPP
#define PLAYERINPUT_HPP

/**
 * Abstraction class to manage player inputs. Handles responses from both the keyboard and joysticks
 *
 * \ingroup Entities
 */
class PlayerInput
{
	int js;
	enum Type
	{
		Xbox,
		NES,
        None
	}type;

	void update();

public:
	/**
	 * Initializes the joystick if one is connected
	 */
	PlayerInput();

	/**
	 * The possible inputs
	 */
	enum Input
	{
		Up,
		Right,
		Down,
		Left,
		Interact,
		Run,
		Pause
	};

	/**
	 * Tells whether or not the given input is occurring
	 *
	 * \param i The Input to check
	 * \return True if the input is active, false otherwise
	 */
	bool isInputActive(Input i);
};

#endif // PLAYERINPUT_HPP
