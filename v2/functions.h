#ifndef functions
#define functions

#include <iostream>
#include <random>

int generateRandomNumber(int min, int max) {
    std::random_device rd;  
    std::mt19937 gen(rd()); // mt engine, seed is rd
    std::uniform_int_distribution<> distr(min, max);
return distr(gen);
}

#endif