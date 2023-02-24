#include <algorithm>
#include <iostream>
#include "glm/gtc/random.hpp"
#include "bvhNode.h"

BVHNode::BVHNode()
{
}

BVHNode::BVHNode(const Scene &scene)
    : BVHNode(scene.getObjects(), 0, scene.objects.size())
{
}

BVHNode::BVHNode(const std::vector<std::shared_ptr<Hittable>> &objects, int start, int end)
{
    auto objs = objects;
    int axis = glm::linearRand<int>(0, 2);

    int objectSpan = end - start;
    if (objectSpan == 1)
    {
        left = right = objs[start];
    }
    else if (objectSpan == 2)
    {
        if (boxCompare(objs[start], objs[start + 1], axis))
        {
            left = objs[start];
            right = objs[start + 1];
        }
        else
        {
            left = objs[start + 1];
            right = objs[start];
        }
    }
    else
    {
        std::sort(objs.begin() + start, objs.begin() + end, [this, axis](const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b)
                  { return boxCompare(a, b, axis); });

        auto mid = start + objectSpan / 2;
        left = std::make_shared<BVHNode>(objs, start, mid);
        right = std::make_shared<BVHNode>(objs, mid, end);
    }

    AABB boxLeft, boxRight;

    if (!left->boundingBox(boxLeft) || !right->boundingBox(boxRight))
    {
        std::cout << "A) No bounding box" << std::endl;
    }

    box = AABB::surroundingBox(boxLeft, boxRight);
}

inline bool BVHNode::boxCompare(const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b, int axis)
{
    AABB boxA;
    AABB boxB;

    if (!a->boundingBox(boxA) || !b->boundingBox(boxB))
    {
        std::cout << "B) No bounding box" << std::endl;
    }
    return boxA.getMin()[axis] < boxB.getMin()[axis];
}

bool BVHNode::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    if (!box.hit(ray, tMin, tMax))
        return false;

    bool hitLeft = left->hit(ray, tMin, tMax, payload);
    bool hitRight = left->hit(ray, tMin, tMax, payload);

    return hitLeft || hitRight;
}

bool BVHNode::boundingBox(AABB &outputBox) const
{
    outputBox = box;
    return true;
}
