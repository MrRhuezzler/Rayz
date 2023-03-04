#include "imgui.h"
#include "glm/gtc/noise.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "textures/noise.h"
#include "textures/solidColor.h"

NoiseTexture::NoiseTexture(glm::vec3 color, float scale)
    : NoiseTexture(std::make_shared<SolidColor>(color), scale)
{
}

NoiseTexture::NoiseTexture(std::shared_ptr<Texture> texture, float scale)
    : texture(texture), scale(scale)
{
}

glm::vec3 NoiseTexture::value(float u, float v, const glm::vec3 &p) const
{
    return texture->value(u, v, p) * 0.5f * (1 + glm::sin(scale * p.z) + 10 * NoiseTexture::turbulence(scale * p));
}

bool NoiseTexture::renderUI()
{
    bool moved = false;
    if (ImGui::DragFloat("#", &scale, 0.01f))
        moved = true;
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
    {
        ImGui::SetTooltip("Scale");
    }

    if (texture->renderUI())
        moved = true;

    return moved;
}

float NoiseTexture::turbulence(const glm::vec3 &p, int depth)
{
    float accumulated = 0.0f;
    float weight = 1.0f;
    auto temp = p;

    for (int i = 0; i < depth; i++)
    {
        accumulated += weight * glm::perlin(p);
        weight *= 0.5f;
        temp *= 2;
    }

    return accumulated;
}