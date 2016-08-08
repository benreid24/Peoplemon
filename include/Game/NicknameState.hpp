#ifndef NICKNAMESTATE_HPP
#define NICKNAMESTATE_HPP

#include "Gamestate.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Menu/ScreenKeyboard.hpp"
#include "Menu/MenuImage.hpp"

/**
 * This Gamestate is the menu to allow the player to name their peoplemon. It can be invoked from anywhere
 *
 * \ingroup Game
 */
class NicknameState : public Gamestate
{
	PeoplemonRef* peoplemon;

	MenuImage background, pplPic;
	ScreenKeyboard keyboard;

	sf::Vector2f oldHudPos;
	int oldWrapWidth;
	bool wasAS;

	/**
	 * Runs the state
	 */
	bool execute();

public:
	/**
	 * Primary constructor for the state. Initializes internal references and the screen keyboard
	 *
	 * \param g A pointer to the main Game object
	 * \param ppl A pointer to the Peoplemon to be named
	 * \param n A pointer to the next Gamestate to execute
	 */
	NicknameState(Game* g, PeoplemonRef* ppl, Gamestate* n = nullptr);

	/**
	 * Cleans up resources and resets the HUD
	 */
	~NicknameState();
};

#endif // NICKNAMESTATE_HPP
