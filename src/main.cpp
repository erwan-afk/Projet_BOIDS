#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "p6/p6.h"
#include "vector"
#include "Boid.hpp"

int main()
{
// Creat first boid
     Boid firstBoid(0.0, 0.0, 2.0, 3.0);
    

    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Projet_BOIDS"}};
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Clear the background with a fading effect
        ctx.use_stroke = false;
        ctx.fill       = {0.2f, 0.1f, 0.3f, 1.0f};
        ctx.rectangle(p6::Center{0.0, 0.0}, p6::Radii{0.3, 0.5}, p6::Rotation{0.0_turn});
        // Draw something
        ctx.fill = {1.f, 0.7f, 0.2f};
        //ctx.circle(p6::Center{ctx.mouse()}, p6::Radius{0.1f});
        ctx.circle(p6::Center{firstBoid.getPosX(), firstBoid.getPosY()}, p6::Radius{0.1f});
        //std::cout << firstBoid.getPosX() << std::endl;
        
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
