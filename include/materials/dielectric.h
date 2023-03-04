#pragma once
#include "materials/material.h"

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

