#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "hittable.h"
#include "materials/material.h"

class Plane : public Hittable
{
private:
    static glm::vec3 directionNormals[6];
    static const char *availableNormals[6];

public:
    glm::vec3 position;
    glm::vec3 normal;
    std::shared_ptr<Material> mat;
    int normalKind;

    // Plane(const std::string &name);
    Plane(const std::string &name, glm::vec3 position, glm::vec3 normal, std::shared_ptr<Material> mat);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputox) const override;
    virtual bool renderUI() override;

    // static std::shared_ptr<Hittable> CreatePlane(const std::string &name);
};
