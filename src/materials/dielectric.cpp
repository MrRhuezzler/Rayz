#include "imgui.h"
#include "glm/gtc/random.hpp"
#include "materials/dielectric.h"

Dieletric::Dieletric(const glm::vec3 &albedo, float index_of_refraction)
    : Dieletric(std::make_shared<SolidColor>(albedo), index_of_refraction)
{
}

Dieletric::Dieletric(const std::shared_ptr<Texture> &texture, float index_of_refraction)
    : texture(texture), ir(index_of_refraction)
{
}

bool Dieletric::scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const
{
    attenuation = texture->value(payload.u, payload.v, payload.worldPosition);
    float ratio = payload.frontFace ? (1.0 / ir) : ir;

    glm::vec3 normalized = glm::normalize(ray.direction);
    double cos = glm::min(glm::dot(-normalized, payload.worldNormal), 1.0f);
    double sin = sqrt(1.0 - cos * cos);

    bool cannotRefract = (ratio * sin) > 1.0;

    if (cannotRefract || (reflectance(cos, ratio) > glm::linearRand(0.0f, 1.0f)))
        scattered.direction = glm::reflect(normalized, payload.worldNormal);
    else
        scattered.direction = glm::refract(normalized, payload.worldNormal, ratio);

    scattered.origin = payload.worldPosition;
    return true;
}

bool Dieletric::renderUI()
{
    bool moved = false;
    if (ImGui::DragFloat("###", &ir, 0.001f))
        moved = true;
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
    {
        ImGui::SetTooltip("Index of Refraction");
    }
    if (texture->renderUI())
        moved = true;
    return moved;
}

glm::vec3 Dieletric::refract(const glm::vec3 &uv, const glm::vec3 &n, float etai_over_etat)
{
    float c = glm::min(glm::dot(-1.0f * uv, n), 1.0f);
    glm::vec3 perpendicular = etai_over_etat * (uv + c * n);
    glm::vec3 parallel = -1.0f * glm::sqrt(glm::abs(1.0f - glm::dot(perpendicular, perpendicular))) * n;
    return perpendicular + parallel;
}

float Dieletric::reflectance(float cosine, float ref_idx)
{
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * glm::pow((1 - cosine), 5.0f);
}
