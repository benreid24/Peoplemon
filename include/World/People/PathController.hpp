#ifndef PATHCONTROLLER_HPP
#define PATHCONTROLLER_HPP

#include "Controller.hpp"
#include "Util/File.hpp"

/**
 * Controller class to make a Character follow a path
 *
 * \ingroup Entities
 */
class PathController : public Controller
{
    std::vector<int> path;
    int curStep;
    bool runBackwards, movingForward; //whether to run beginning to end then back or just loop normally
    bool finishedOnce;

    /**
     * Returns the reversed direction of the given direction
     *
     * \param dir The direction to reverse
     */
    int reverseDirection(int dir);

public:
    /**
     * Loads the controller from the given save file
     *
     * \param c A pointer to the Characteer to be controlled
     * \param file The file to load the path from
     */
    PathController(Character* c, File* file);

    /**
     * Constructs the controller from the given path
     *
     * \param c The Character to be controlled
     * \param p The path to follow
     */
    PathController(Character* c, std::vector<int> p);

    /**
     * Performs the path behavior on the Character
     *
     * \param g A pointer to the main Game object
     */
    void act(Game* g);

    /**
     * Returns whether or not the controller has finished navigating the path
     */
    bool completed();
};

#endif // PATHCONTROLLER_HPP
