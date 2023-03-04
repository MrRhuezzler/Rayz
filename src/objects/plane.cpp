#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "objects/plane.h"

// Plane::Plane(const std::string &name)
// {

// }

Plane::Plane(const std::string &name, glm::vec3 position, glm::vec3 normal, std::shared_ptr<Material> mat)
    : position(position), normal(-normal), mat(mat), Hittable(name)
{
    for (int i = 0; i < 6; i++)
    {
        float val = glm::dot(normal, directionNormals[i]);
        if (val > 0.0f)
        {
            normalKind = i;
        }
    }
}

bool Plane::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    float denominator = glm::dot(normal, ray.direction);
    if (glm::abs(denominator) > 1e-6)
    {
        glm::vec3 p = position - ray.origin;
        float t = glm::dot(p, normal) / denominator;

        if (t < tMin || tMax < t)
            return false;

        glm::vec3 a = glm::cross(normal, glm::vec3(1, 0, 0));
        glm::vec3 b = glm::cross(normal, glm::vec3(0, 1, 0));
        glm::vec3 max_ab = glm::dot(a, a) < glm::dot(b, b) ? b : a;
        glm::vec3 c = glm::cross(normal, glm::vec3(0, 0, 1));

        glm::vec3 uVec = glm::normalize(glm::dot(max_ab, max_ab) < glm::dot(c, c) ? c : max_ab);
        glm::vec3 vVec = glm::cross(normal, uVec);

        payload.worldPosition = ray.origin + t * ray.direction;
        payload.hitDistance = t;
        payload.setFaceNormal(ray, normal);
        payload.mat = mat;
        payload.u = glm::dot(payload.worldPosition - position, uVec);
        payload.v = glm::dot(payload.worldPosition - position, vVec);
        return t >= 0;
    }
    return false;
}

bool Plane::boundingBox(AABB &outputBox) const
{
    return false;
}

glm::vec3 Plane::directionNormals[6] = {
    glm::vec3(-1.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
};

const char *Plane::availableNormals[6] = {"LEFT", "RIGHT", "UP", "DOWN", "FRONT", "BACK"};

bool Plane::renderUI()
{
    bool moved = false;

    {
        ImGui::SeparatorText("Props");
        if (ImGui::DragFloat3("###", glm::value_ptr(position), 0.001f))
            moved = true;
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
        {
            ImGui::SetTooltip("Position");
        }

        float x = ImGui::GetContentRegionAvail().x;
        if (ImGui::Button("Normal", ImVec2(x, 0.0f)))
        {
            ImGui::OpenPopup("normal_change");
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
        {
            ImGui::SetTooltip("%s", availableNormals[normalKind]);
        }

        if (ImGui::BeginPopup("normal_change"))
        {
            ImGui::SeparatorText("Directions");
            for (int i = 0; i < IM_ARRAYSIZE(availableNormals); i++)
                if (ImGui::Selectable(availableNormals[i]))
                {
                    normal = directionNormals[i];
                    normalKind = i;
                    moved = true;
                }
            ImGui::EndPopup();
        }
    }

    {
        ImGui::SeparatorText("Mat");
        if (mat->renderUI())
            moved = true;
    }

    return moved;
}

// std::shared_ptr<Hittable> Plane::CreatePlane(const std::string &name)
// {

// }
