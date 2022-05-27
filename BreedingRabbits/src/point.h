#ifndef POINT_H
#define POINT_H

#include <array>

template<typename T, uint dim>
struct Point
{
    using Type = T;
    std::array<T, dim> x;
};


template<typename T, uint dim>
inline T square_distance(const Point<T, dim>& p0, const Point<T, dim>& p1)
{
    // If the compiler doesn't optimise this for dim == 2 we can write a custom impl without a loop
    T dist = 0;
    for(auto i = 0; i < dim; ++i) {
        dist += (p0.x[i] - p1.x[i]) * (p0.x[i] - p1.x[i]);
    }
    return dist;
}

#endif // POINT_H
