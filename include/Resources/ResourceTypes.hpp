#ifndef RESOURCETYPES_HPP
#define RESOURCETYPES_HPP

#include <memory>
#include "SFML.hpp"
#include "Media/Animation.hpp"
#include "Scripts/Script Interpreter.hpp"

typedef std::shared_ptr<sf::Texture> TextureReference;
typedef std::shared_ptr<sf::SoundBuffer> SoundReference;
typedef std::shared_ptr<AnimationSource> AnimationReference;
typedef std::shared_ptr<Script> ScriptReference;

#endif // RESOURCETYPES_HPP
