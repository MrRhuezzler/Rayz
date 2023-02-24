#pragma once

#include "ray.h"

class AABB
{
public:
    glm::vec3 minimum, maximum;
    AABB();
    AABB(const glm::vec3 &a, const glm::vec3 &b);

    const glm::vec3 &getMin() const;
    const glm::vec3 &getMax() const;

    bool hit(const Ray &ray, float tMin, float tMax) const;

    static AABB surroundingBox(AABB box0, AABB box1);
};
