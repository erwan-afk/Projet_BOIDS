
#include <iostream>
#include <cstdlib> // Pour rand() et srand()
#include <ctime>   // Pour time()

#include <random> 



double rand01()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
}




template<typename T>
T uniforme(T min, T max) {

    int random_num = rand01();

    T scaled_num = static_cast<T>(random_num) / RAND_MAX;

    T result = min + scaled_num * (max - min);

    return result;
}




