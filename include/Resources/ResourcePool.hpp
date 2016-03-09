#ifndef RESOURCEPOOL_HPP
#define RESOURCEPOOL_HPP

#include "SFML.hpp"
#include "Resources/ResourceTypes.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <memory>

/**
 * \defgroup Resources
 * \brief All classes related to resource management are in this module
 */

class AnimationSource;

/**
 * This function provides a generic interface for the ResourceManager to load resources. It must be specialized for any resource types whose constructor doesn't take a single string argument
 *
 * \param file The path to the resource to load
 *
 * \ingroup Resources
 */
template<typename T>
T* loadResourceFromUri(std::string file)
{
    return new T(file);
}

template<>
sf::Texture* loadResourceFromUri(std::string file);
template<>
sf::SoundBuffer* loadResourceFromUri(std::string file);
template<>
AnimationSource* loadResourceFromUri(std::string file);

/**
 * This class manages all resources and handles the deallocation of unused memory
 *
 * \ingroup Resources
 */
template<typename T>
class ResourcePool
{
    std::map<std::string,std::shared_ptr<T> > resources;

    sf::Thread runner;
    sf::Mutex lock;
    bool running;

    /**
     * This runs on a separate thread and clears resources every 5 minutes
     */
    void updater()
    {
        while (running)
        {
            for (int i = 0; i<300 && running; ++i)
                sf::sleep(sf::milliseconds(1000));
            if (running)
			{
				lock.lock();
				beg:
				for (auto i = resources.begin(); i!=resources.end(); ++i)
				{
					if (i->second.unique())
					{
						resources.erase(i);
						goto beg;
					}
				}
				lock.unlock();
			}
        }
    }

public:
    /**
     * Initializes the internal memory and starts the cleanup thread
     */
    ResourcePool() : runner(&ResourcePool<T>::updater,this)
    {
       running = true;
       runner.launch();
    }

    /**
     * Terminates the cleanup thread
     */
    ~ResourcePool()
    {
        running = false;
        runner.wait();
        std::cout << "Resource manager thread terminated\n";
    }

    /**
     * Loads the resource at the given URI
     *
     * \param uri The path to the resource to load
     * \return A pointer to a ResourceBase object containing the loaded resource
     */
    std::shared_ptr<T> loadResource(std::string uri)
    {
        lock.lock();
        auto i = resources.find(uri);
        if (i!=resources.end())
        {
            lock.unlock();
            return i->second;
        }
        std::shared_ptr<T> temp(loadResourceFromUri<T>(uri));
        resources[uri] = temp;
        lock.unlock();
        return temp;
    }

    /**
     * Frees all resources, regardless of what may still be using them
     */
    void clearAll()
    {
        lock.lock();
        resources.clear();
        lock.unlock();
    }
};

#endif // RESOURCEPOOL_HPP
