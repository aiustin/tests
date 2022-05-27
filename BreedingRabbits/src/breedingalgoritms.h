#ifndef BREEDINGALGORITMS_H
#define BREEDINGALGORITMS_H

#include "animaltraits.h"

#include "point.h"

#include <cassert>
#include <cstdint>
#include <limits>
#include <vector>
#include <algorithm>

namespace breeding_alg
{

    struct SuperBreedingCache
    {
        std::vector<double> a = { 1. };
        // uint-to-double conversions are very expensive (>4-16 clock cycles without AVX512)
        // so, avoid them if possible and keep everything as double. This would need to be balanced
        // against double multiplications vs uint multiplications. Also the limits are different.
        std::vector<double> x_pow = { 1. };
        uint64_t n = 0;
        uint64_t N = 0;
    };

    /*
     *  no_of - "super breeding" implementation
     *
     *  Note: This is not thread-safe. To make it thread safe we can just pass the cache as a param instead of static var
     */
    template<typename T>
    uint64_t no_of_super_breeding(int64_t x, int64_t n)
    {
        assert(x >= 0 && n >= 0);
        if (x <= 0 || n < 0) {
            return 0;
        }

        static SuperBreedingCache cache;
        assert(cache.a.size() == cache.x_pow.size());

        // Full Cache Hit
        if (n == cache.n && cache.x_pow.size() > 1 && cache.x_pow[1] == x) {
            return cache.N;
        }

        // Initialize missing ratios
        cache.a.reserve(n + 1);
        for(auto i = cache.a.size(); i <= n; ++i) {
            cache.a.push_back(1 + 1./cache.a[i-1]);
        }

        // Initialize x_pow
        // only grow, to reduce reallocations - for cases like x==1 and n very large this could pottentially grow way too much
        // to the point we may not have enough memory.
        if (n + 1 > cache.x_pow.size()) {
            cache.x_pow.resize(n + 1);
        }
        // Reuse if possible
        const auto start = cache.x_pow.size() > 1 && cache.x_pow[1] == x ? cache.n + 1 : 1;
        for(auto i = start; i <= n; ++i) {
            assert(std::numeric_limits<double>::max() / x >= cache.x_pow[i-1]);
            cache.x_pow[i] = cache.x_pow[i-1] * x;
        }

        cache.n = n;

        double N = 0;
        for(auto i = 0; i <= n; ++i) {
            assert(std::numeric_limits<double>::max() / cache.a[i] >= cache.x_pow[i]);
            assert(std::numeric_limits<double>::max() - cache.a[i] * cache.x_pow[i] >= N);
            N += cache.a[i] * cache.x_pow[i];
            assert(N <= std::numeric_limits<uint64_t>::max());
        }
        cache.N = static_cast<uint64_t>(N);

        return cache.N;
    }


    /*
     * no_of
     *
     * @params:
     *  x - starting population
     *  n - no of days
     * @returns: no of T after n days starting with a population of size x
     */
    template<typename T>
    uint64_t no_of(int64_t x, int64_t n)
    {
        if constexpr(is_super_breeding<T>::value)
            return no_of_super_breeding<T>(x, n);
        else
            static_assert (is_super_breeding<T>::value, "Only super breeding implementation supported for now.");
        return 0;
    }

    /*
     * square_distance
     *
     * @params:
     *  a0, a1 - animals
     *
     * @returns: the square of the distance
     */
    template<typename T>
    int animals_square_distance(const T& a0, const T& a1)
    {
        return square_distance(a0.pos(), a1.pos());
    }


    /*
     * count_in_breeding_distance
     *
     * @params:
     *  animal - reference animal
     *  animals - list of animals
     *  threshold - breeding distance
     *
     * @returns: count of T in breeding distance of animal
     */
    template<typename T, typename TS>
    int count_in_breeding_distance(const T& animal, const TS& animals, const typename T::PositionType::Type& threshold)
    {
        const auto squareThreshold = threshold * threshold;
        return std::count_if(animals.begin(), animals.end(), [&animal, &squareThreshold](const T& a) {
            return animals_square_distance(animal, a) <= squareThreshold;
        } );
    }


    /*
     * count_in_breeding_distance_optimised
     *
     *  // Not implemented - @see Animal class comments
     *
     * @returns: count of T in breeding distance of animal
     */
    template<typename T, typename TS>
    int count_in_breeding_distance_optimised(const T& p0, const TS& ps, const typename T::PositionType::Type& threshold)
    {
        const auto squareThreshold = threshold * threshold;
        return std::count_if(ps.begin(), ps.end(), [&p0, &squareThreshold](const T& p) {
            return square_distance(p0, p) <= squareThreshold;
        } );
    }
}

#endif // BREEDINGALGORITMS_H
