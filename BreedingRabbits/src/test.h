#ifndef TEST_H
#define TEST_H

#include <cstdint>
#include <vector>

class Rabbit;
namespace alg_naive {

extern uint64_t no_of_rabbits_super_breeding(int64_t x, int64_t n);
extern int no_of_rabbits_in_breeding_distance(const Rabbit& rabbit, const std::vector<Rabbit>& rabbits, int distance);

}


namespace Test {
    extern void testSuperBreeding(int xMax, int nMax);
    extern void testInBreedingDistance(int distance, int sampleSize);
}



#endif // TEST_H
