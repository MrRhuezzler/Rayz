#pragma once

#include "hittable.h"
#include "scene.h"

class BVHNode : public Hittable
{
public:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB box;

    BVHNode();
    BVHNode(const Scene &scene);
    BVHNode(const std::vector<std::shared_ptr<Hittable>> &objects, int start, int end);

    inline bool boxCompare(const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b, int axis);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputBox) const override;
};