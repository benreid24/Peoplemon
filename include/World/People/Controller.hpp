#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

class Character;
class Game;

/**
 * Interface class for over world AI's that control Character movement
 *
 * \ingroup Entities
 */
class Controller
{
protected:
    Character* owner;

public:
    /**
     * Initializes the Controller with a pointer to its owning Character
     *
     * \param o The Character to control
     */
    Controller(Character* o) { owner = o; }

    /**
     * For polymorphism to be happy
     */
    virtual ~Controller(){};

    /**
     * Does one turn of input on the Character
     *
     * \param g A pointer to the main Game object
     */
    virtual void act(Game* g)=0;
};

#endif // CONTROLLER_HPP
