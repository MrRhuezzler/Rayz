#pragma once
#include "materials/material.h"

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
