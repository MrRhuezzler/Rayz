#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "hittable.h"
#include "materials/material.h"

class Sphere : public Hittable
{
public:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> mat;

    Sphere(const std::string &name);
    Sphere(const std::string &name, glm::vec3 center, float radius, std::shared_ptr<Material> mat);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputox) const override;
    virtual bool renderUI() override;

    static std::shared_ptr<Hittable> CreateSphere(const std::string &name);
};
