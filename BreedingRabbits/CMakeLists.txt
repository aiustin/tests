cmake_minimum_required(VERSION 3.5)

project(breedingrabbits LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES    src/animal.cpp
                    src/rabbit.cpp
                    src/test.cpp
                    )

set(HEADER_FILES    src/animal.h
                    src/rabbit.h
                    src/animaltraits.h
                    src/breedingalgoritms.h
                    src/point.h
                    src/test.h
                    )

add_executable(simulations-plus main.cpp ${SOURCE_FILES} ${HEADER_FILES})

