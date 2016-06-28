#ifndef HUD_HPP
#define HUD_HPP

#include "SFML.hpp"
#include "Menu.hpp"
#include "Media/Animation.hpp"

class Game;

/**
 * This class provides a universal interface for rendering objects "on top" of the screen. This
 * allows multiple locations and threads to easily have access to the screen
 *
 * \ingroup Menu
 */
class HUD
{
    Game* game;
    //flags
    bool displayingMessage;
    bool gettingChoice;
    bool alwaysShow;

    //displaying message/getting choice
    std::string message, messageShown; //used for ghost writer
    int timeTillDone, ghostTimer, minDelay;
    MenuImage textBox;
    MenuText messageText;
    ChoiceBox choiceBox;
    int wrapW;
    sf::Vector2f position;

    //animations
    std::vector<std::pair<std::pair<int,int>,Animation> > playingAnims;

    /**
     * Wraps the given string to a certain width to fit in the dialogue box
     *
     * \param str The string to wrap
     * \return The wrapped string
     */
    std::string wordWrap(std::string str);

public:
    /**
     * Initializes the HUD with a pointer to the main game object
     *
     * \param g A pointer to the main game object
     */
    HUD(Game* g);

    /**
     * This controls whether or not the dialogue box disappears when it is done displaying
     *
     * \param as Whether or not the dialogue box should hide when finished. True to always show
     */
    void setAlwaysShow(bool as);

    /**
     * Updates the on screen dialogue box and any other elements that need updating
     */
    void update();

    /**
     * Renders the HUD to the given screen
     *
     * \param window The window to render the HUD to
     */
    void draw(sf::RenderWindow* window);

    /**
     * Displays a message in the dialogue box and optionally plays the given sound
     *
     * \param message The message to display
     * \param sound The optional audio file to play
     * \param block Whether or not the HUD should block the calling thread
     */
    void displayMessage(std::string message, std::string sound = "", bool block = false); //used by scripts and talking

    /**
     * Tells whether or not the last message displayed has been "finished", meaning that the player viewed the entire message
     *
     * \return Whether or not the player finished viewing the last message passed
     */
    bool messageFinished();

    /**
     * Prompts the user for a choice. Can block while waiting for input or run concurrently
     *
     * \param message The message to display in the dialogue box
     * \param opts The options to prompt the player with
     * \param block Whether or not to block the calling thread until the user makes a choice
     * \return Either the chosen choice, if blocking, or empty if run concurrently
     */
    std::string getChoice(std::string message, std::vector<std::string> opts, bool block = true);

    /**
     * Returns the last chosen choice of the player. Use this method when getting player input non-concurrently
     *
     * \return The last choice chosen by the player, or empty if none
     */
    std::string getChoice(); //use this for ConversationState because that thread can't be blocked in the above function like scripts can

	/**
	 * Moves the text box to a different spot. Leave blank to reset. Same goes for the wrap width
	 *
	 * \param pos The new position of the text box
	 * \param wrapWidth The width to wrap the text at
	 */
	void rePosition(sf::Vector2f pos = sf::Vector2f(200,500), int wrapWidth = 390);

	/**
	 * Returns the current position of the HUD
	 */
	sf::Vector2f getPosition();

	/**
	 * Returns the current wrap width of the HUD
	 */
	int getWrapWidth();

    /**
     * Plays the given animation at the given spot on the screen for the given number of playbacks
     *
     * \param file The path to the animation to play
     * \param x The x coordinate on the screen to render at
     * \param y The y coordinate on the screen to render at
     * \param loops The amount of times to play the animation
     */
    void playAnimation(std::string file, int x, int y, int loops); //adds to vector of animations that are updated/drawn on top of everything
};

#endif // HUD_HPP
