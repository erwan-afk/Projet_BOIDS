#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "vector"

#include "Simulation.hpp"

int main()
{
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
        ctx.fill       = {0.2f, 0.1f, 0.3f, 0.1f};
        ctx.rectangle(p6::FullScreen{});
        // Draw something
        ctx.fill = {1.f, 0.7f, 0.2f};
        ctx.circle(p6::Center{ctx.mouse()}, p6::Radius{0.1f});
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}

