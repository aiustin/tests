#include "test.h"


#include "rabbit.h"
#include "breedingalgoritms.h"

#include <algorithm>
#include <iostream>
#include <cmath>

namespace alg_naive {

uint64_t no_of_rabbits_super_breeding(int64_t x, int64_t n)
{
    if (n < 0 || x <=0 ) {
        return 0;
    }
    double fn_2 = 1.;
    double fn_1 = 1.;
    double N = 0.;
    double x_pow = 1.;
    for(auto i = 0; i <= n; ++i) {
        double a = fn_2 / fn_1;
        N += a * x_pow;
        //std::cout << " i= " << i << " a= " << a << " xpow= " << x_pow << " N= " << N << std::endl;
        x_pow *= x;
        fn_1 += fn_2;
        std::swap(fn_1, fn_2);
    }
    return static_cast<uint64_t>(N);
}

int no_of_rabbits_in_breeding_distance(const Rabbit& rabbit, const std::vector<Rabbit>& rabbits, int distance)
{
    int count = 0;
    const auto& p0 = rabbit.pos();

    for(const auto& r : rabbits) {
        const auto& p = r.pos();
        const auto square_dist = (p0.x[0] - p.x[0]) * (p0.x[0] - p.x[0]) + (p0.x[1] - p.x[1]) * (p0.x[1] - p.x[1]);
        const auto dist = sqrt( square_dist );

        if ( (square_dist <= distance * distance && dist > distance) ||
             (square_dist > distance * distance && dist <= distance)
             )
        {
            assert(false);
        }

        if ( dist <= distance) {
            ++count;
        }
    }
    return count;
}

}

namespace Test {
void testSuperBreeding(int xMax, int nMax)
{
    assert(breeding_alg::no_of<Rabbit>(11, 3) == alg_naive::no_of_rabbits_super_breeding(11, 3));
    assert(breeding_alg::no_of<Rabbit>(11, 14) == alg_naive::no_of_rabbits_super_breeding(11, 14));
    assert(breeding_alg::no_of<Rabbit>(11, 14) == alg_naive::no_of_rabbits_super_breeding(11, 14));
    assert(breeding_alg::no_of<Rabbit>(3, 6) == alg_naive::no_of_rabbits_super_breeding(3, 6));

    for(auto i = 0; i < 100; ++i) {
        auto x = std::rand() % xMax;
        auto n = std::rand() % nMax;

        auto N0 = breeding_alg::no_of<Rabbit>(x, n);
        auto N1 = alg_naive::no_of_rabbits_super_breeding(x, n);

        static const uint64_t ERROR = 1;
        if (N0 != N1 && ((N0 > N1 && N0 - N1 > ERROR) || (N0 < N1 && N1 - N0 > ERROR)))
        {
            std::cout << "x= " << x << " n= " << n << " N0= " << N0 << " N1= " << N1 << std::endl;
        }
        assert(N0 == N1 || (N0 > N1 && N0 - N1 <= ERROR) || (N0 < N1 && N1 - N0 <= ERROR));
    }
}

void testInBreedingDistance(int distance, int sampleSize)
{
    const int XMAX = 1000;
    const int YMAX = 1000;

    auto x = std::rand() % XMAX - XMAX / 2;
    auto y = std::rand() % YMAX - YMAX / 2;
    std::vector<Rabbit> rabbits;
    Rabbit rabbit{Animal::PositionType{x, y}};

    for(auto i = 0; i < sampleSize; ++i) {
        x = std::rand() % XMAX - XMAX / 2;
        y = std::rand() % YMAX - YMAX / 2;
        rabbits.push_back(Rabbit{Animal::PositionType{x, y}});

    }

    const auto c0 = breeding_alg::count_in_breeding_distance(rabbit, rabbits, distance);
    const auto c1 = alg_naive::no_of_rabbits_in_breeding_distance(rabbit, rabbits, distance);
    if (c0 != c1) {
        std::cout << c0 << " " << c1 << std::endl;
    }
    assert(c0 == c1);
}

}
