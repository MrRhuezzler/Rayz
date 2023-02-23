#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "ray.h"
#include "material.h"

class Hittable
{
public:
    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const = 0;
};

class Sphere : public Hittable
{
public:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> mat;

    Sphere();
    Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> mat);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
};

class Scene : public Hittable
{
public:
    Scene();
    Scene(std::shared_ptr<Hittable> object);

    void clear();
    void add(std::shared_ptr<Hittable> object);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;

private:
    std::vector<std::shared_ptr<Hittable>> objects;
};