#include "hittable.h"

inline void HitPayload::setFaceNormal(const Ray &ray, const glm::vec3 &outwardNormal)
{
    frontFace = glm::dot(ray.direction, outwardNormal) < 0;
    worldNormal = frontFace ? outwardNormal : -1.0f * outwardNormal;
}

Sphere::Sphere()
    : center(0.0f), radius(0.5f)
{
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
