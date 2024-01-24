#pragma once

#include <cmath>
#include <iostream>

class Environnement {
private:
    double sizeX;
    double sizeY;

public:
    // Constructeur
    Environnement(double initSizeX, double initSizeY);

    void draw(Environnement shape);

    // Destructeur
    ~Environnement();
};
