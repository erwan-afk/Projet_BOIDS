#pragma once
#include <math.h>
#include <p6/p6.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "glm/fwd.hpp"

// ----- Base -----
static auto& generator()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    return gen;
}
float alea(float min, float max)
{
    if (min > max)
        throw std::invalid_argument{"min must be smaller than max"};

    auto distribution = std::uniform_real_distribution<float>{min, max};
    return distribution(generator());
}

int randomBinary()
{
    return alea(0, 2);
}

float random1()
{
    return alea(0.f, 1.f);
}

float random2()
{
    return alea(-1.f, 1.f);
}

float random3()
{
    return alea(-0.4f, 0.4f);
}

float factoriel(float x)
{
    float res = 1.;
    for (unsigned int i = 1; i <= x; ++i)
    {
        res *= i;
    }
    return res;
}

// ----- other -----
template<typename T>
T displayVec(std::vector<T> vec)
{
    for (T num : vec)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

// ----- functions -----

template<typename T>
T uniforme(const std::vector<T>& vector)
{
    if (vector.empty())
    {
        throw std::invalid_argument("uniforme(vector) : Empty vector");
    }
    return vector[alea(0, vector.size())];
}

//--

template<typename T>
std::vector<T> permutation(std::vector<T> vector)
{
    if (vector.empty())
    {
        throw std::invalid_argument("permutation(vector) : Empty vector");
    }
    const int        size = vector.size();
    std::vector<T>   result(size);
    std::vector<int> remain(size);

    for (unsigned int i = 0; i < size; i++)
    {
        remain[i] = i;
    }

    for (unsigned int i = 0; i < size; i++)
    {
        int index = uniforme(remain);
        result[i] = vector[index];

        auto it = std::find(remain.begin(), remain.end(), index);
        if (it != remain.end())
        {
            remain.erase(it);
        }
    }
    return result;
}

//--

float exponentielle(float lambda)
{
    float x = -log(1 - random1()) / lambda;
    return 2.0f * (x / (1 + x)) - 1.0f;
}
float exponentielle2(float lambda)
{
    float x = -log(1 - random1()) / lambda;
    return x / (1 + x);
}

//--

float gamma(float x)
{
    return factoriel(x - 1);
}

float loi_beta(float alpha, float beta)
{
    float x, y;
    do
    {
        float u = random1();
        float v = random1();

        x = std::pow(u, 1.0f / alpha);
        y = std::pow(v, 1.0f / beta);
    } while (x + y > 1.0f);

    float result = (2.0f * x / (x + y)) - 1.0f;
    return result;
}
//--

float loi_normale(float esperance, float ecart_type)
{
    float res;
    do
    {
        double x = sqrt(-2 * log(random1())) * cos(2 * p6::PI * random1());
        res      = esperance + ecart_type * x;
    } while (res < -1.0f || res > 1.0f);
    return res;
}

//--

bool bernoulli(double p)
{
    if (random1() < p)
    {
        return true;
    }
    else
    {
        return false;
    }
}

float loi_laplace(float mu, float b)
{
    float x   = random1() - 0.5;
    float res = mu - b * std::copysign(1.0f, x) * std::log(1.0f - 2.0f * std::fabs(x));
    //[-1, 1];
    res = res / 10.0f;
    if (res <= 1.0f && res >= -1.0f)
    {
        return res;
    }
}

float loi_geometric(float p)
{
    float res = std::log(1.0 - random1()) / std::log(1.0 - p);
    res       = res / 10.0f;
    if (res <= 1.0f && res >= -1.0f)
    {
        return 0.4f * res + 0.9f; // pour [0.5, 1.3]
    }
}

float loi_depareto(float alpha, float a, float x0)
{
    float u   = random1();
    float res = a * (1.0f - u) / pow(u, 1.0f / alpha) + x0;
    return 2.0f * (res / 10.0f - 0.5f);
}

// ---- MARKOV -----

// state 0 = eau sombre
// state 1 = eau claire
// state 2 = eau avec pollution

glm::mat3 matriceTransition{
    {3 / 7, 3 / 7, 1 / 7},
    {3 / 10, 4 / 10, 2 / 10},
    {1 / 5, 1 / 5, 3 / 5}

};

void markov_setp2(int& current_state, glm::vec3 m)
{
    float a = random1();
    if (a < m[0])
    {
        current_state = 0;
    }
    else if (a < m[0] + m[1])
    {
        current_state = 1;
    }
    else if (a < m[0] + m[1] + m[2])
    {
        current_state = 2;
    }
    else
    {
        current_state = 3;
    }
}

void markov_setp1(int current_state)
{
    switch (current_state)
    {
    case 0:
        markov_setp2(current_state, matriceTransition[0]);
        break;
    case 1:
        markov_setp2(current_state, matriceTransition[1]);
        break;
    case 2:
        markov_setp2(current_state, matriceTransition[2]);
        break;
    }
}

/*

int main()
{
    // --- Random ---
    // for(unsigned int i = 0; i<100; i++){
    //    std::cout << random1() <<  " - " << randomBinary() << std::endl;
    // }

    // --- Factoriel ---
    // for(unsigned int i = 0; i<100; i++){
    //    std::cout << factoriel(5) << std::endl;
    // }

    // --- Uniforme ---
    // std::vector<int> original = {0,1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // for(unsigned int i = 0; i<10; i++){
    //    std::cout << uniforme(original) << std::endl;
    // }

    //______

    // --- Permutation ---
    // std::vector<int> original = {0,1, 2, 3, 4, 5, 6, 7, 8, 9 };
    // displayVec(original);

    // for(unsigned int i = 0; i<100; i++){
    //     displayVec(permutation(original));
    // }

    // --- Exponentielle ---
    // continue (in out)
    // float lambda = 1.0f; //modifier la courbe exp
    // int nb_try = 100;
    // float esperance = 0.;

    // for(unsigned int i = 0; i<nb_try; i++){
    //     float res = exponentielle(lambda);
    //     std::cout << res << std::endl;
    //     esperance+= res;
    // }
    // esperance/=nb_try;
    // std::cout << "Esperance : " << esperance << std::endl;

    // -- Bêta --
    // continue in, alpha beta continue, continue out
    // float alpha = 5.f;
    // float beta = 1.f;
    // float esperance = 0.;
    // int nb_try = 100;

    // for(unsigned int i = 0; i< nb_try; i++){
    //     float res =  loi_beta(alpha, beta);
    //     std::cout << res << std::endl;
    //     esperance+= res;
    // }

    // esperance/=nb_try;
    // std::cout << "Esperance : " << esperance << std::endl;

    // // -- Loi normal - méthode box-Muller --
    // centrée ->(en 0), réduite->(hauteur de la cloche)
    // continue in out
    // float esperance  = 3;
    // float ecart_type = 2;
    // int nb_try = 100;
    // for(unsigned int i = 0; i< nb_try; i++){
    //     std::cout << loi_normale(esperance, ecart_type) << std::endl;
    // }

    // --Bernoulli--
    // continue in -> bool out
    // int nb_try = 100;
    // double p = 0.8f;        //(win)
    //  for(unsigned int i = 0; i< nb_try; i++){
    //     std::cout << bernoulli(p) << std::endl;
    // }

    return 0;
}
*/