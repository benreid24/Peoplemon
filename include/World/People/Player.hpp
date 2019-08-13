#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Util/File.hpp"
#include "PlayerInput.hpp"

/**
 * This class stores all player related data as well as controls the player in the over world, handling UI
 *
 * \ingroup Entities
 * \ingroup Game
 */
class Player : public Character
{
    Game* game;
    bool isBoy, isRunning;
    Animation running[4];

    std::vector<int> items;
    std::vector<PeoplemonRef> curPeoplemon;
    std::vector<StoredPeoplemon> storedPeoplemon;
    int money;

public:
    /**
     * Initializes all the player data to the default values
     *
     * \param g A pointer to the main Game object
     */
    Player(Game* g);

    /**
     * I'm not sure why I made this
     */
    ~Player();

    /**
     * Initializes all the internal data appropriately based on the parameters given
     *
     * \param isBoy Whether or not the player is a boy
     * \param name The name of the player
     */
    void newGame(bool isBoy, std::string name);

    /**
     * Saves all the player data to the given file
     *
     * \param saveFile The file to save the data to
     */
    void save(File* saveFile);

    /**
     * Loads the player data from the given file
     *
     * \param loadFile The file to load the data from
     */
    void load(File* loadFile);

    /**
     * Gathers player input and updates the player, altering the game state as necessary
     *
     * \param game A pointer to the main Game object
     */
    void update(Game* game);

    /**
     * Gets the list of entities in front of the player and calls their interact methods
     *
     * \param game A pointer to the main Game object
     */
    void interact(Game* game);

    /**
     * Renders the player to the given window
     *
     * \param window The window to render to
     * \param camPos The position of the camera to offset the world position by
     */
    void draw(sf::RenderWindow* window, sf::Vector2f camPos);


    /**
     * Returns whether or not the player "is occupied", meaning whether or not they can talk or battle
     */
    bool isOccupied(); //determined by lock, used by trainers to determine when to engage

    /**
     * Returns the gender of the player as a string
     */
    std::string getGender();

    /**
     * Returns the quantity of items the player has with the given id
     *
     * \param i The id to check for
     * \return The amount of items with the given id that the player owns
     */
    int hasItem(int i);

    /**
     * Removes the given quantity of items with the given id
     *
     * \param i The id of the item to remove
     * \param q The amount of items to take
     */
    void takeItem(int i, int q = 1);

    /**
     * Gives the player one item with the given id
     *
     * \param i The id of the item to give to the player
     */
    void giveItem(int i);

    /**
     * Alters the players money by the specified amount
     *
     * \param m The amount to change their money by, use negative numbers to remove money
     * \return Whether or not the money was able to be altered
     */
    bool alterMoney(int m);

    /**
     * Returns the amount of money the player has
     */
    int getMoney();

    /**
     * Returns a pointer to the vector containing the player's items
     */
    std::vector<int>* getItems();

    /**
     * Returns a pointer to the vector containing the players current party peoplemon
     */
    std::vector<PeoplemonRef>* getCurrentPeoplemon();

    /**
     * Returns a vector to the player's peoplemon in the storage box
     */
    std::vector<StoredPeoplemon>* getStoredPeoplemon();

    /**
     * Adds a Peoplemon to the storage system
     *
     * \param ppl The Peoplemon to add
     */
    void addStoredPeoplemon(PeoplemonRef ppl, bool registerCaught);

    /**
     * Convenience function to add a Peoplemon to the party or PC if no room
     */
    void givePeoplemon(PeoplemonRef ppl, bool resetHealth = true, bool firstTime = true);

    /**
     * Returns true if the player has the requested Peoplemon
     *
     * \param id The id of the Peoplemon to check for
     * \param minLevel Optional minimum level the Peoplemon must be at
     * \return True if the player has the requested Peoplemon
     */
    bool hasPeoplemon(int id, int minLevel = -1);

    /**
     * Similar to hasPeoplemon, but removes the Peoplemon from the party and returns it
     * The returned Peoplemon will have an id of -1 if the player didn't have it
     */
    PeoplemonRef takePeoplemon(int id, int minLevel = -1);

    /**
     * Returns whether or not the player can fly yet
     */
    bool canFly();

    /**
     * Returns whether or not the player has a peoplemon that will Impulse Buy this time
     */
	bool makeImpulseBuy();

	/**
	 * If any Peoplemon in the players party are Questionable this function will give them items when the player moves. Called by update
	 */
	void pickupRandom();

    /**
     * Fully heals all of the peoplemon in the players party
     */
    void whiteout();

    /**
     * Changes the walking/running anims. Optionally persists across game saves
     */
    void changeAnims(std::string walkAnim, std::string runAnim, bool persist);

    /**
     * Checks for persistent animations and changes to them if they exist. Call after loading game
     */
    void loadPersistentAnims();

    /**
     * Returns a string to identify the object for debug purposes
     */
    std::string getIdentifier();

    /**
     * Stops all movement animations. This is to get rid of a weird artifact when maps are loaded
     */
	void forceStop();

	/**
	 * Returns the position the player was last at. Use for lastMap functionality
	 */
	sf::Vector2f getLastPosition();
};

#endif // PLAYER_HPP
