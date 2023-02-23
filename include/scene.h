#pragma once

#include <vector>
#include "glm/glm.hpp"

struct Material
{
    glm::vec3 albedo;
    float roughness = 1.0f;
    float metallic = 0.0f;
};

struct Sphere
{
    glm::vec3 position;
    float radius;

    Material mat;
};

struct Scene
{
    std::vector<Sphere> spheres;
};