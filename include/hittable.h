#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "ray.h"
#include "boundingBox.h"
#include "material.h"

class Hittable
{
public:
    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const = 0;
    virtual bool boundingBox(AABB &outputox) const = 0;
};

class Sphere : public Hittable
{
public:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> mat;

    Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> mat);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputox) const override;
};

class Plane : public Hittable
{
public:
    glm::vec3 position;
    glm::vec3 normal;
    std::shared_ptr<Material> mat;

    Plane(glm::vec3 position, glm::vec3 normal, std::shared_ptr<Material> mat);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputox) const override;
};

class Triangle : public Hittable
{
public:
    glm::vec3 v0, v1, v2;
    std::shared_ptr<Material> mat;

    Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, std::shared_ptr<Material> mat);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputox) const override;
};

class Scene : public Hittable
{
public:
    Scene();
    Scene(std::shared_ptr<Hittable> object);

    void clear();
    void add(std::shared_ptr<Hittable> object);

    const std::vector<std::shared_ptr<Hittable>> &getObjects() const;

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputox) const override;

    std::vector<std::shared_ptr<Hittable>> objects;
};