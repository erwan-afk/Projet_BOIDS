#pragma once

#include <string>
#include <vector>
#include "ModelMesh.hpp"
#include "p6/p6.h"

class Hitbox {
private:
    glm::vec3 position;
    glm::vec3 scale;

public:
    Hitbox(const ModelMesh& mesh)
    {
    }

    glm::vec3 getPosition() const
    {
        return position;
    }

    glm::vec3 getScale() const
    {
        return scale;
    }

    ~Hitbox();
};