#include <cstdlib>
#include "Simulation.hpp"
#include "p6/p6.h"
#include "vector"


int main()
{
    // Creat first boid
    // Boid firstBoid(0.0, 0.0, 2.0, 3.0);
    // double deltaTime = 0.001;

    Simulation Sim;
    Sim.Run();
    return 0;
}
