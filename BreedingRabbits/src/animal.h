#ifndef ANIMAL_H
#define ANIMAL_H

#include "point.h"

class Animal
{
public:
    using PositionType = Point<int, 2>;
public:
    Animal(const PositionType& p = PositionType()) : pos_{p} {}

    void moveTo(PositionType newPos) { pos_ = newPos; }
    const PositionType& pos() const { return pos_; };
private:
    PositionType pos_;
};

/*
 * Re count_in_breeding_distance
 * If size of the data is really big and speed is a concern that one possible optimisation is to create a custom struct
 * where the point is the main data so the data can fit better into a cache line.
 * struct PointRabbit {
 * int x;
 * int y;
 * Rabbit r; // or depending on the size of the Rabbit -> std::unique_ptr<Rabbit>, but then there are memory allocations involved
 *           // (which may not matter if Animal/Rabbit is a really polymorphic hierarchy but as there were no real requirements
 *           // for the polymophic behaviour I though I'd keep things simple and fast)
 *           // It all depends on the requirements & context
 *
 * }
 *
 */


#endif // ANIMAL_H
