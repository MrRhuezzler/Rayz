#pragma once

#include <memory>

#include "glm/glm.hpp"
#include "ray.h"

#include "textures.h"

class Material;

struct HitPayload
{
    glm::vec3 worldPosition;
    glm::vec3 worldNormal;
    std::shared_ptr<Material> mat;
    float hitDistance;
    float u, v;
    bool frontFace;

    void setFaceNormal(const Ray &ray, const glm::vec3 &outwardNormal);
};

class Material
{
public:
    virtual glm::vec3 emitted(const Ray &ray, const HitPayload &payload, double u, double v, const glm::vec3 &p) const
    {
        return glm::vec3(0, 0, 0);
    }

    virtual bool scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const = 0;
    virtual bool renderUI()
    {
        return false;
    };
};
