#ifndef PLAYERINPUT_HPP
#define PLAYERINPUT_HPP

class Game;

/**
 * Abstraction class to manage player inputs. Handles responses from both the keyboard and joysticks
 *
 * \ingroup Entities
 */
class PlayerInput
{
	Game* game;
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
	 * Set the internal reference to the main Game object. Call this function to ensure that the game only updates when in focus
	 *
	 * \param g A pointer to the main Game object
	 */
	void setGame(Game* g);

	/**
	 * Tells whether or not the given input is occurring
	 *
	 * \param i The Input to check
	 * \return True if the input is active, false otherwise
	 */
	bool isInputActive(Input i);
};

#endif // PLAYERINPUT_HPP
