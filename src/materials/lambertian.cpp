#include "glm/gtc/random.hpp"
#include "materials/lambertian.h"

Lambertian::Lambertian(const glm::vec3 &albedo)
    : texture(std::make_shared<SolidColor>(albedo))
{
}

Lambertian::Lambertian(const std::shared_ptr<Texture> &texture)
    : texture(texture)
{
}

bool Lambertian::scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const
{
    glm::vec3 scatterDirection = payload.worldNormal + glm::normalize(glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f)));
    scattered.origin = payload.worldPosition;
    scattered.direction = scatterDirection;
    attenuation = texture->value(payload.u, payload.v, payload.worldPosition);
    return true;
}

bool Lambertian::renderUI()
{
    bool moved = false;

    if (texture->renderUI())
        moved = true;

    return moved;
}
