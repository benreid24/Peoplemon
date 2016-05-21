#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Media/Animation.hpp"
#include "World/Object.hpp"
#include "Controller.hpp"
#include <queue>
#include <stack>

/**
 * \defgroup Entities
 * \brief All world entities are in this module
 */

/**
 * This is the base class for all "living" entities in the world. This means NPCs, trainers and the player
 *
 * \ingroup Entities
 */
class Character : public Object
{
protected:
    std::string name;
    bool isLocked;
    std::stack<bool> prevLockStates;
    int dir;
    sf::Vector2i mapPos, lastPos; //used for movement checking. use Object::position for interpolation. movement is limited by checking mapPos*32==position. change speed by changing interpolation speed
    Animation walking[4];
    Controller* behavior;
    bool isMoving;
    unsigned long long int lastTime;
    std::queue<int> queuedInput;

    /**
     * Loads the appropriate directional animations from the given path
     *
     * \param a Array of 4 Animation objects to load the animations into
     * \param file The folder containing the directional animations
     */
    void loadAnim(Animation* a, std::string file);

public:
    /**
     * Default constructor. Initializes internal state data
     */
    Character();

    /**
     * Here for polymorphism to be happy
     */
    virtual ~Character();

    /**
     * Sets the position and direction of the character and resets the animation states
     *
     * \param pos The position of the entity in pixels
     * \param d The direction to face the entity
     */
    void spawn(sf::Vector2f pos, int d);

    /**
     * Returns the name of the entity
     */
    std::string getName();

    /**
     * Returns the direction the entity is facing
     */
    int getDir();

    /**
     * Returns the position of the entity in tiles
     */
    sf::Vector2i getMapPos();

    /**
     * Performs update logic on the entity. This means either AI or player input
     *
     * \param game A pointer to the main Game object
     */
    virtual void update(Game* game);

    /**
     * This method is called by the Player object when the player interacts with this entity
     *
     * \param game A pointer to the main Game object
     */
    virtual void interact(Game* game)=0;

    /**
     * Renders the entity to the given window
     *
     * \param window The window to render the entity into
     * \param camPos The position of the camera in pixels
     */
    virtual void draw(sf::RenderWindow* window, sf::Vector2f camPos);

    /**
     * This function causes the Character to move on tile in the given direction, if movement is legal
     *
     * \param game A pointer to the main Game object
     * \param d The direction to move in
     * \param ignoreCols Whether or not to ignore collisions
     * \param playAnims Whether or not to play the animations
     * \param queueInput Whether or not the input should be queued
     * \return Whether or not the Character was able to move
     */
    bool move(Game* game, int d, bool ignoreCols = false, bool playAnims = true, bool queueInput = false);

    /**
     * Shifts the position of the character the given number of tiles, ignoring other entities and walls
     *
     * \param x The amount of tiles to shift in the x direction
     * \param y The amount of tiles to shift in the y direction
     */
    void shift(int x, int y);

    /**
     * Sets whether or not the Character is allowed to move
     *
     * \param l Whether or not the Character can move
     * \param r Whether or not the Character should store its previous lock state for later restoration. Used by pause functionality
     */
    void setLock(bool l, bool r = false);

    /**
     * Resets the lock state to what it was before a call to setLock with r set to true
     */
    void resetLock();

    /**
     * Sets the current position and last position of the character on the map to be open
     *
     * \param g A pointer to the main Game object
     */
	void resetCollisions(Game* g);

    /**
     * Stops all movement animations. This is to get rid of a weird artifact when maps are loaded
     */
	virtual void forceStop();

    /**
     * Returns a string to identify the object for debug purposes
     */
    virtual std::string getIdentifier();
};

#endif // CHARACTER_HPP
