#pragma once
#include "materials/material.h"

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
