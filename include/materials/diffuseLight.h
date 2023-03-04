#pragma once
#include "materials/material.h"

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