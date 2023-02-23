#include <iostream>

#include "imgui.h"
#include "hittable.h"

inline void HitPayload::setFaceNormal(const Ray &ray, const glm::vec3 &outwardNormal)
{
    frontFace = glm::dot(ray.direction, outwardNormal) < 0;
    worldNormal = frontFace ? outwardNormal : -1.0f * outwardNormal;
}

Sphere::Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> mat)
    : center(center), radius(radius), mat(mat)
{
}

bool Sphere::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    glm::vec3 origin = ray.origin - center;
    float a = glm::dot(ray.direction, ray.direction);
    float half_b = glm::dot(origin, ray.direction);
    float c = glm::dot(origin, origin) - radius * radius;

    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0f)
        return false;

    float sqrtDiscriminant = glm::sqrt(discriminant);
    float t = (-half_b - sqrtDiscriminant) / a;
    if (t < tMin || tMax < t)
    {
        t = (-half_b + sqrtDiscriminant) / a;
        if (t < tMin || tMax < t)
            return false;
    }

    payload.hitDistance = t;
    payload.worldPosition = ray.origin + t * ray.direction;
    glm::vec3 normal = (payload.worldPosition - center) / radius;
    payload.setFaceNormal(ray, normal);
    payload.mat = mat;

    return true;
}

Plane::Plane(glm::vec3 position, glm::vec3 normal, std::shared_ptr<Material> mat)
    : position(position), normal(-normal), mat(mat)
{
}

bool Plane::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    float denominator = glm::dot(normal, ray.direction);
    if (glm::abs(denominator) > 1e-6)
    {
        glm::vec3 p = position - ray.origin;
        float t = glm::dot(p, normal) / denominator;

        if (t < tMin || tMax < t)
            return false;

        payload.worldPosition = ray.origin + t * ray.direction;
        payload.hitDistance = t;
        payload.setFaceNormal(ray, normal);
        payload.mat = mat;
        return t >= 0;
    }
    return false;
}

Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, std::shared_ptr<Material> mat)
    : v0(v0), v1(v1), v2(v2), mat(mat)
{
}
#define MT
#ifndef MT
bool Triangle::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    glm::vec3 v0v1 = v1 - v0;
    glm::vec3 v0v2 = v2 - v0;
    glm::vec3 normal = glm::cross(v0v1, v0v2);
    float area = normal.length() / 2.0f;

    float p = glm::dot(normal, ray.direction);
    if (glm::abs(p) < 1e-6)
        return false;

    float d = -glm::dot(normal, v0);
    float t = -(glm::dot(normal, ray.origin) + d);

    // if (t < 0)
    //     return false;

    if (t < tMin || tMax < t)
        return false;

    payload.worldPosition = ray.origin + t * ray.direction;
    payload.hitDistance = t;
    payload.mat = mat;
    payload.setFaceNormal(ray, normal);

    glm::vec3 edge0 = v1 - v0;
    glm::vec3 vp0 = payload.worldPosition - v0;
    glm::vec3 C1 = glm::cross(edge0, vp0);
    if (glm::dot(normal, C1) < 0)
        return false;

    glm::vec3 edge1 = v2 - v1;
    glm::vec3 vp1 = payload.worldPosition - v1;
    glm::vec3 C2 = glm::cross(edge1, vp1);
    if (glm::dot(normal, C2) < 0)
        return false;

    glm::vec3 edge2 = v0 - v2;
    glm::vec3 vp2 = payload.worldPosition - v2;
    glm::vec3 C3 = glm::cross(edge2, vp2);
    if (glm::dot(normal, C3) < 0)
        return false;

    return true;
}
#else
bool Triangle::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    glm::vec3 v0v1 = v1 - v0;
    glm::vec3 v0v2 = v2 - v0;
    glm::vec3 normal = glm::normalize(glm::cross(v0v1, v0v2));
    glm::vec3 p = glm::cross(ray.direction, v0v2);
    float det = glm::dot(v0v1, p);

    if (glm::abs(det) < 1e-6)
        return false;

    float invDet = 1.0f / det;

    glm::vec3 tvec = ray.origin - v0;
    float u = glm::dot(tvec, p) * invDet;
    if (u < 0 || u > 1)
        return false;

    glm::vec3 qvec = glm::cross(tvec, v0v1);
    float v = glm::dot(ray.direction, qvec) * invDet;
    if (v < 0 || u + v > 1)
        return false;

    float t = glm::dot(v0v2, qvec) * invDet;

    if (t < tMin || tMax < t)
        return false;

    payload.worldPosition = ray.origin + t * ray.direction;
    payload.hitDistance = t;
    payload.mat = mat;
    payload.setFaceNormal(ray, normal);

    return true;
}
#endif

Scene::Scene()
{
}

Scene::Scene(std::shared_ptr<Hittable> object)
{
    add(object);
}

void Scene::clear()
{
    objects.clear();
}

void Scene::add(std::shared_ptr<Hittable> object)
{
    objects.push_back(object);
}

bool Scene::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    HitPayload tempPayload;
    bool hitAnything = false;

    auto closestSoFar = tMax;

    for (const auto &object : objects)
    {
        if (object->hit(ray, tMin, closestSoFar, tempPayload))
        {
            hitAnything = true;
            closestSoFar = tempPayload.hitDistance;
            payload = tempPayload;
        }
    }

    return hitAnything;
}
