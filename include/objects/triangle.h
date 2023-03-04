#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "hittable.h"
#include "materials/material.h"

class Triangle : public Hittable
{
public:
    glm::vec3 v0, v1, v2;
    std::shared_ptr<Material> mat;

    Triangle(const std::string &name, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, std::shared_ptr<Material> mat);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputox) const override;
    virtual bool renderUI() override;

    // static std::shared_ptr<Hittable> CreateTriangle(const std::string &name);
};