#pragma once

#include <memory>

#include "glm/glm.hpp"
#include "ray.h"

#include "texture.h"

class Material;

struct HitPayload
{
    glm::vec3 worldPosition;
    glm::vec3 worldNormal;
    std::shared_ptr<Material> mat;
    float hitDistance;
    float u, v;
    bool frontFace;

    inline void setFaceNormal(const Ray &r, const glm::vec3 &outward_normal);
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

class Lambertian : public Material
{
public:
    Lambertian(const glm::vec3 &albedo);
    Lambertian(const std::shared_ptr<Texture> &texture);
    virtual bool scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const override;
    virtual bool renderUI() override;

public:
    std::shared_ptr<Texture> texture;
};

class Metal : public Material
{
public:
    Metal(const std::shared_ptr<Texture> &texture, float fuzz);
    Metal(const glm::vec3 &albedo, float fuzz);
    virtual bool scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const override;
    virtual bool renderUI() override;

public:
    std::shared_ptr<Texture> texture;
    float fuzz;
};

class Dieletric : public Material
{
public:
    Dieletric(const glm::vec3 &albedo, float index_of_refraction);
    Dieletric(const std::shared_ptr<Texture> &texture, float index_of_refraction);
    virtual bool scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const override;
    virtual bool renderUI() override;

private:
    static glm::vec3 refract(const glm::vec3 &uv, const glm::vec3 &n, float etai_over_etat);
    static float reflectance(float cosine, float ref_idx);

public:
    std::shared_ptr<Texture> texture;
    float ir;
};

class DiffuseLight : public Material
{
public:
    std::shared_ptr<Texture> texture;

    DiffuseLight(glm::vec3 albedo);
    DiffuseLight(const std::shared_ptr<Texture> &texture);

    virtual bool scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const override;
    virtual glm::vec3 emitted(const Ray &ray, const HitPayload &payload, double u, double v, const glm::vec3 &p) const override;
    virtual bool renderUI() override;
};