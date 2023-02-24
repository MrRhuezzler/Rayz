#include "boundingBox.h"

AABB::AABB()
{
}

AABB::AABB(const glm::vec3 &a, const glm::vec3 &b)
    : minimum(a), maximum(b)
{
}

const glm::vec3 &AABB::getMin() const
{
    return minimum;
}

const glm::vec3 &AABB::getMax() const
{
    return maximum;
}

bool AABB::hit(const Ray &ray, float tMin, float tMax) const
{
    for (int i = 0; i < 3; i++)
    {
        float t0 = glm::min<float>(((minimum[i] - ray.origin[i]) / ray.direction[i]), ((maximum[i] - ray.origin[i]) / ray.direction[i]));
        float t1 = glm::max<float>(((minimum[i] - ray.origin[i]) / ray.direction[i]), ((maximum[i] - ray.origin[i]) / ray.direction[i]));

        tMin = glm::max(t0, tMin);
        tMax = glm::min(t1, tMax);

        if (tMax <= tMin)
            return false;
    }

    return true;
}

AABB AABB::surroundingBox(AABB box0, AABB box1)
{
    glm::vec3 minimum(glm::min(box0.getMin()[0], box1.getMin()[0]),
                      glm::min(box0.getMin()[1], box1.getMin()[1]),
                      glm::min(box0.getMin()[2], box1.getMin()[2]));

    glm::vec3 maximum(glm::max(box0.getMax()[0], box1.getMax()[0]),
                      glm::max(box0.getMax()[1], box1.getMax()[1]),
                      glm::max(box0.getMax()[2], box1.getMax()[2]));

    return AABB(minimum, maximum);
}