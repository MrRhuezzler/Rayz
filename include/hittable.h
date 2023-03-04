#pragma once

#include <string>
#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "ray.h"
#include "boundingBox.h"
#include "materials/material.h"

class Hittable
{
public:
    const std::string name;
    Hittable(const std::string &name);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const = 0;
    virtual bool boundingBox(AABB &outputox) const = 0;
    virtual bool renderUI()
    {
        return false;
    };
};
