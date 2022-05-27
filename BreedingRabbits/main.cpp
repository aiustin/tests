#include <iostream>

#include "src/rabbit.h"
#include "src/breedingalgoritms.h"
#include "src/test.h"

int main()
{
    Test::testSuperBreeding(5, 29);
    Test::testSuperBreeding(29, 7);

    for(auto i = 1; i < 100; ++i) {
        auto s = std::rand() % 100000;
        auto d = std::rand() % 1000;

        Test::testInBreedingDistance(d, s);
    }
    return 0;
}
