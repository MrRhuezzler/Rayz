#include "materials/diffuseLight.h"

DiffuseLight::DiffuseLight(glm::vec3 albedo)
    : DiffuseLight(std::make_shared<SolidColor>(albedo))
{
}

DiffuseLight::DiffuseLight(const std::shared_ptr<Texture> &texture)
    : texture(texture)
{
}

bool DiffuseLight::scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const
{
    return false;
}

glm::vec3 DiffuseLight::emitted(const Ray &ray, const HitPayload &payload, double u, double v, const glm::vec3 &p) const
{
    if (payload.frontFace)
        return texture->value(u, v, p);
    else
        return glm::vec3(0.0f);
}

bool DiffuseLight::renderUI()
{
    bool moved = false;
    if (texture->renderUI())
        moved = true;
    return moved;
}
