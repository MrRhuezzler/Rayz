#include <iostream>

#include "glm/gtc/constants.hpp"

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
    // glm::vec3 normal = glm::normalize(payload.worldPosition - center);
    payload.setFaceNormal(ray, normal);
    payload.mat = mat;

    payload.u = glm::atan(normal.x, normal.z) / (2.0f * glm::pi<float>()) + 0.5f;
    payload.v = normal.y * 0.5 + 0.5;

    // payload.u = (atan2(normal.x, -normal.z) / glm::pi<float>() + 1.0f) / 2.0f;
    // payload.v = asin(normal.y) / glm::pi<float>() + .5;

    return true;
}

bool Sphere::boundingBox(AABB &outputBox) const
{
    outputBox = AABB(center - glm::vec3(radius), center + glm::vec3(radius));
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

        glm::vec3 a = glm::cross(normal, glm::vec3(1, 0, 0));
        glm::vec3 b = glm::cross(normal, glm::vec3(0, 1, 0));
        glm::vec3 max_ab = glm::dot(a, a) < glm::dot(b, b) ? b : a;
        glm::vec3 c = glm::cross(normal, glm::vec3(0, 0, 1));

        glm::vec3 uVec = glm::normalize(glm::dot(max_ab, max_ab) < glm::dot(c, c) ? c : max_ab);
        glm::vec3 vVec = glm::cross(normal, uVec);

        payload.worldPosition = ray.origin + t * ray.direction;
        payload.hitDistance = t;
        payload.setFaceNormal(ray, normal);
        payload.mat = mat;
        payload.u = glm::dot(payload.worldPosition - position, uVec);
        payload.v = glm::dot(payload.worldPosition - position, vVec);
        return t >= 0;
    }
    return false;
}

bool Plane::boundingBox(AABB &outputBox) const
{
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
    payload.u = u;
    payload.v = v;

    return true;
}
#endif

bool Triangle::boundingBox(AABB &outputBox) const
{
    outputBox = AABB(glm::min(v0, glm::min(v1, v2)), glm::max(v0, glm::max(v1, v2)));
    return true;
}

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

const std::vector<std::shared_ptr<Hittable>> &Scene::getObjects() const
{
    return objects;
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

bool Scene::boundingBox(AABB &outputBox) const
{
    if (objects.empty())
        return false;

    AABB temp;
    bool firstBox = true;

    for (const auto &object : objects)
    {
        if (!object->boundingBox(temp))
            return false;
        outputBox = firstBox ? temp : AABB::surroundingBox(outputBox, temp);
        firstBox = false;
    }

    return true;
}
