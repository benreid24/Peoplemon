#include "Resources/ResourcePool.hpp"
#include "Media/Animation.hpp"

template<>
sf::SoundBuffer* loadResourceFromUri(std::string file)
{
    sf::SoundBuffer* temp = new sf::SoundBuffer();
    temp->loadFromFile(file);
    return temp;
}

template<>
sf::Texture* loadResourceFromUri(std::string file)
{
    sf::Texture* temp = new sf::Texture();
    temp->loadFromFile(file);
    return temp;
}

template<>
AnimationSource* loadResourceFromUri(std::string file)
{
    return new AnimationSource(file);
}
