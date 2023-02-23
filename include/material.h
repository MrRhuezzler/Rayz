#pragma once

#include <memory>

#include "glm/glm.hpp"
#include "ray.h"

class Material;

struct HitPayload
{
    glm::vec3 worldPosition;
    glm::vec3 worldNormal;
    std::shared_ptr<Material> mat;
    float hitDistance;
    bool frontFace;

    inline void setFaceNormal(const Ray &r, const glm::vec3 &outward_normal);
};

class Material
{
public:
    virtual bool scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const glm::vec3 &albedo);
    virtual bool scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const override;

public:
    glm::vec3 albedo;
};

class Metal : public Material
{
public:
    Metal(const glm::vec3 &albedo, float fuzz);
    virtual bool scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const override;

public:
    glm::vec3 albedo;
    float fuzz;
};

class Dieletric : public Material
{
public:
    Dieletric(double index_of_refraction);
    virtual bool scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    static glm::vec3 refract(const glm::vec3 &uv, const glm::vec3 &n, float etai_over_etat);
    static float reflectance(float cosine, float ref_idx);

public:
    double ir;
};