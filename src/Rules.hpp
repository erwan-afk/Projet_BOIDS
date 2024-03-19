
#include <iostream>
#include <cstdlib> // Pour rand() et srand()
#include <ctime>   // Pour time()

#include <random> 



//base
static auto& generator()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    return gen;
}
float rand(float min, float max)
{
    if (min > max)
        throw std::invalid_argument{"min must be smaller than max"};

    auto distribution = std::uniform_real_distribution<float>{min, max};
    return distribution(generator());
}




//functions

template<typename T>
T uniforme(T min, T max) {
    return rand(min, max);
}





