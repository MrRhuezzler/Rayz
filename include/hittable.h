#pragma once

#include <string>
#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "ray.h"
#include "boundingBox.h"
#include "material.h"

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

    Plane(const std::string &name);
    Plane(const std::string &name, glm::vec3 position, glm::vec3 normal, std::shared_ptr<Material> mat);

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputox) const override;
    virtual bool renderUI() override;

    // static std::shared_ptr<Hittable> CreatePlane(const std::string &name);
};

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

class Scene : public Hittable
{
private:
    // static std:
public:
    Scene(const std::string &name);
    Scene(const std::string &name, const std::shared_ptr<Hittable> &object);

    void clear();
    void add(const std::shared_ptr<Hittable> &object);

    const std::vector<std::shared_ptr<Hittable>> &getObjects() const;

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputox) const override;
    virtual bool renderUI() override;

    std::vector<std::shared_ptr<Hittable>> objects;
};

// class Scene
// {
// public:
//     std::shared_ptr<HittableList> hittableList;

//     Scene();

//     bool renderUI();
//     void add(const std::shared_ptr<Hittable> &object);
//     const std::vector<std::shared_ptr<Hittable>> &getObjects() const;
// };