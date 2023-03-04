#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "textures/solidColor.h"

SolidColor::SolidColor(const glm::vec3 &color)
    : color(color)
{
}

glm::vec3 SolidColor::value(float u, float v, const glm::vec3 &p) const
{
    return color;
}

bool SolidColor::renderUI()
{
    bool moved = false;
    if (ImGui::ColorEdit3("###", glm::value_ptr(color)))
        moved = true;
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
    {
        ImGui::SetTooltip("Albedo");
    }
    return moved;
}
