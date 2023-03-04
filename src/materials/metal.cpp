#include "imgui.h"
#include "glm/gtc/random.hpp"
#include "materials/metal.h"

Metal::Metal(const glm::vec3 &albedo, float fuzz)
    : Metal(std::make_shared<SolidColor>(albedo), fuzz)
{
}

Metal::Metal(const std::shared_ptr<Texture> &texture, float fuzz)
    : texture(texture), fuzz(fuzz)
{
}

bool Metal::scatter(const Ray &ray, const HitPayload &payload, glm::vec3 &attenuation, Ray &scattered) const
{
    glm::vec3 scatteredDirection = glm::reflect(ray.direction, payload.worldNormal);
    scattered.origin = payload.worldPosition;
    scattered.direction = scatteredDirection + fuzz * glm::normalize(glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f)));
    attenuation = texture->value(payload.u, payload.v, payload.worldPosition);
    return dot(scatteredDirection, payload.worldNormal) > 0;
}

bool Metal::renderUI()
{
    bool moved = false;
    if (ImGui::DragFloat("###", &fuzz, 0.001f))
        moved = true;
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
    {
        ImGui::SetTooltip("Fuzz");
    }
    if (texture->renderUI())
        moved = true;
    return moved;
}
