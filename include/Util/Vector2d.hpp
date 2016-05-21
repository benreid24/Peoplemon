#ifndef _2DVECTOR_HPP
#define _2DVECTOR_HPP

#include <vector>

/**
 * \defgroup Utilities
 * \brief All classes that provide basic functionality are in this module
 */

/**
 * Utility class that wraps std::vector, making it indexable in two dimensions
 *
 * \ingroup Utilities
 */
template <typename T>
class Vector2D
{
    std::vector<T> data;
    int W, H;

    public:
    /**
     * Creates an empty vector
     */
    Vector2D()
    {
        //ctor
    }

    /**
     * Frees all used memory
     */
    ~Vector2D()
    {
        //dtor
    }

    /**
     * Returns a reference to the element at the given position
     *
     * \param x The x coordinate of the element to access
     * \param y The y coordinate of the element to access
     * \return A reference to the element at the given indices
     */
    T& operator() (int x, int y)
    {
        return data[y*W+x];
    }

    /**
     * Resizes the vector to the given size
     *
     * \param x The new width
     * \param y The new height
     */
    void setSize(int x, int y)
    {
        W = x;
        H = y;
        data.resize(W*H, T());
    }

    /**
     * Returns the width of the vector
     */
    int getWidth()
    {
        return W;
    }

    /**
     * Returns the height of the vector
     */
    int getHeight()
    {
        return H;
    }

    /**
     * Clears all memory used by the vector, erasing all elements
     */
    void clear()
    {
        data.clear();
    }
};

#endif
