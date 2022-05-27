#ifndef RABBIT_H
#define RABBIT_H

#include "animal.h"
#include "animaltraits.h"

class Rabbit : public Animal
{
public:
    Rabbit(const PositionType& p = PositionType()) : Animal(p) {}
};

template<>
struct is_super_breeding<Rabbit> {
    static const bool value = true;
};


#endif // RABBIT_H
