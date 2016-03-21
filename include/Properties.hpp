#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <string>
#include "SFML.hpp"

/**
 * This class publicly stores all of the static properties of the game, such as file paths and window resolutions
 *
 * \ingroup Global
 */
class Properties
{
public:
    const static int ScreenWidth = 800;
    const static int ScreenHeight = 600;
    const static int TilesWide = 25;
    const static int TilesTall = 19;

    const static std::string GameSavePath;
    const static std::string DataPath;
    const static std::string MapPath;
    const static std::string TilesetPath;
    const static std::string PeoplePath;
    const static std::string PeoplemonBasePath;
    const static std::string WildPeoplemonPath;
    const static std::string OwnedPeoplemonPath;
    const static std::string MapTilePath;
    const static std::string AnimationPath;
    const static std::string PeoplemonAnimationPath;
    const static std::string PeoplemonImagePath;
    const static std::string BattleImagePath;
    const static std::string MoveAnimationPath;
    const static std::string MiscAnimationPath;
    const static std::string MiscImagePath;
    const static std::string SpriteSheetPath;
    const static std::string ScriptPath;
    const static std::string FontPath;
    const static std::string PlaylistPath;
    const static std::string MusicPath;
    const static std::string AudioPath;
    const static std::string TrainerPath;
    const static std::string NpcPath;
    const static std::string ConversationPath;
    const static std::string MenuImagePath;
    const static std::string CreditsPath;

    static sf::Font PrimaryMenuFont;
    static sf::Font SecondaryMenuFont;
    static sf::Font ConversationFont;
    static sf::Font BattleFont;

    static sf::Keyboard::Key upKey;
    static sf::Keyboard::Key rightKey;
    static sf::Keyboard::Key downKey;
    static sf::Keyboard::Key leftKey;
    static sf::Keyboard::Key runKey;
    static sf::Keyboard::Key interactKey;
    static sf::Keyboard::Key pauseKey;
};

#endif // PROPERTIES_HPP
