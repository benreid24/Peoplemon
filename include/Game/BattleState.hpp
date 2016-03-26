#ifndef BATTLESTATE_HPP
#define BATTLESTATE_HPP

#include "Gamestate.hpp"
#include "Menu/Menu.hpp"
#include "Battle/Battler.hpp"
#include "Battle/PeoplemonAnimations.hpp"
#include "Menu/BattleBox.hpp"

/**
 * Defines a Gamestate for when the player is in battle and implements the battles themselves
 *
 * \ingroup Game
 * \ingroup Battle
 */
class BattleState : public Gamestate
{
    Battler *player, *opponent;
    PeoplemonAnimations playerAnims, opponentAnims;
    std::string opponentName, loseLine, playlist;
    int prizeMoney;
    bool canRun, playerWinned;
    bool ensureClosed;
    bool lastMoveHit;

    Battler* order[2];
    Turn turns[2]; //this is so the player can have their input taken first so they're able to input while a network player is doing their turn
    PeoplemonAnimations* anims[2];
    std::vector<int> sentIn; //indexes of player peoplemon used

    TextureReference backgroundTxtr;
    sf::Sprite background;
    BattleBox playerBox, opBox;
    std::vector<Animation*> toDraw; //rendered and updated by each of the below functions as necessary

    /**
     * Runs the state
     */
    bool execute();

    /**
     * Informs the state if it should close the game
     */
    bool shouldClose();

    /**
     * Adds the given index to the list of peoplemon that should receive xp when an enemy peoplemon is defeated
     *
     * \param i The index of the peoplemon that was sent in
     */
    void sendIn(int i);

    /**
     * Returns a string introducing the players opponent. Changes based on the type of opponent
     *
     * \return A string to output
     */
    std::string getIntroLine();

    /**
     * Returns the string to print when the given battler switches peoplemon
     *
     * \param b The Battler who is switching
     * \param oldP The index of the battler's old peoplemon
     * \param A string to output
     */
    std::string getSwitchLine(Battler* b, int oldP);

    /**
     * Returns the name of the peoplemon of the given Battler
     *
     * \param b The Battler
     * \param i The index of the peoplemon whose name to get
     * \return The name of the given peoplemon
     */
    std::string getPeoplemonName(Battler* b, int i);

    /**
     * Returns a PeoplemonRef struct for the given Battler
     *
     * \param b The Battler whose peoplemon to get
     * \param i The index of the peoplemon to get
     * \return The PeoplemonRef struct of the requested peoplemon
     */
    PeoplemonRef getPeoplemon(Battler* b, int i);

    /**
     * Returns what to output when the given Battler uses the given item
     *
     * \param b The Battler
     * \param id The id of the used item
     * \return A string to output
     */
    std::string getItemLine(Battler* b, int id);

    /**
     * Returns what to output when the given Battler uses the given move
     *
     * \param b The Battler
     * \param id The id of the moved used
     * \return The string to output
     */
    std::string getMoveLine(Battler* b, int id);

    /**
     * Plays a transition screen from the over world view into the battle view
     */
    void transitionScreen();

    /**
     * Renders the "static" battle view, and continues to do so until both HP bars are where they're supposed to be
     */
    void renderStatic();

    /**
     * Displays a message in the textbox and doesn't return until it is fully vied by the player
     *
     * \param m The message to output
     */
    void displayMessage(std::string m);

    /**
     * Plays the introduction anims for the given Battler for when battle first starts
     *
     * \param b The Battler
     */
    void playIntroAnim(Battler* b);

    /**
     * Plays the appropriate switch animations for when the battler switches, and loads the animations for the new peoplemon
     *
     * \param b The Battler who is switching
     * \param o The opponent Battler
     * \param curPpl The index of the currently out peoplemon
     * \param newPpl The index of the peoplemon to switch to
     */
    void playSwitchAnim(Battler* b, Battler* o, int curPpl, int newPpl);

    /**
     * Plays and handles the timing for the animations for when the given battler uses the given move
     *
     * \param b The Battler
     * \param moveId The id of the move being used
     */
    void playAttackAnim(Battler* b, int moveId);

    /**
     * Renders all objects put in the render queue, which is used for calling functions to generically use the shortcut animation functions
     */
    void renderQueue();

    /**
     * Applies move damage and effects and generates output
     *
     * \param atk The attacking Battler
     * \param def The Battler being attacked
     * \param id The id of the move being used
     * \return A vector of string to be outputted
     */
    std::vector<std::string> applyMove(Battler* atk, Battler* def, int id); //deals damages, applies effects

    /**
     * Handles checking end of battle conditions, switching peoplemon, playing animations and doing XP stuff. Really magical
     *
     * \param i The index of the Battler who's turn it currently is
     * \param j The index of the other Battler
     * \return Whether or not the battle is over. Calling code should still check shouldClose
     */
    bool doFaint(int i, int j);

public:
    /**
     * Constructs the Battlestate from the given parameters
     *
     * \param g A pointer to the main Game object
     * \param op A pointer to the Battler opponent object
     * \param opName The name of the opponent
     * \param ll What the opponent will say in battle when they lose
     * \param pm The prize money awarded for victory
     * \param cr Whether or not the player may run
     * \param music The filename of the playlist to file
     * \param bgnd The filename of the image to make the background
     * \param n The next state to execute
     */
    BattleState(Game* g, Battler* op, std::string opName, std::string ll, int pm, bool cr, std::string music = "battle.plst", std::string bgnd = "battleBgnd.png", Gamestate* n = NULL); //creating method above this knows if it is network or AI

    /**
     * Destructor, does nothing
     */
    ~BattleState();

    /**
     * Returns the outcome of the battle
     *
     * \return True if the player won, false otherwise
     */
    bool playerWon();

    /**
     * Renders the base graphics of the battle screen. This function is used by the PlayerBattle object while it get user input
     */
    void renderBase();
};

#endif // BATTLESTATE_HPP
