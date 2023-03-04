#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "objects/triangle.h"

Triangle::Triangle(const std::string &name, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, std::shared_ptr<Material> mat)
    : v0(v0), v1(v1), v2(v2), mat(mat), Hittable(name)
{
}
#define MT
#ifndef MT
bool Triangle::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    glm::vec3 v0v1 = v1 - v0;
    glm::vec3 v0v2 = v2 - v0;
    glm::vec3 normal = glm::cross(v0v1, v0v2);
    float area = normal.length() / 2.0f;

    float p = glm::dot(normal, ray.direction);
    if (glm::abs(p) < 1e-6)
        return false;

    float d = -glm::dot(normal, v0);
    float t = -(glm::dot(normal, ray.origin) + d);

    // if (t < 0)
    //     return false;

    if (t < tMin || tMax < t)
        return false;

    payload.worldPosition = ray.origin + t * ray.direction;
    payload.hitDistance = t;
    payload.mat = mat;
    payload.setFaceNormal(ray, normal);

    glm::vec3 edge0 = v1 - v0;
    glm::vec3 vp0 = payload.worldPosition - v0;
    glm::vec3 C1 = glm::cross(edge0, vp0);
    if (glm::dot(normal, C1) < 0)
        return false;

    glm::vec3 edge1 = v2 - v1;
    glm::vec3 vp1 = payload.worldPosition - v1;
    glm::vec3 C2 = glm::cross(edge1, vp1);
    if (glm::dot(normal, C2) < 0)
        return false;

    glm::vec3 edge2 = v0 - v2;
    glm::vec3 vp2 = payload.worldPosition - v2;
    glm::vec3 C3 = glm::cross(edge2, vp2);
    if (glm::dot(normal, C3) < 0)
        return false;

    return true;
}
#else
bool Triangle::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    glm::vec3 v0v1 = v1 - v0;
    glm::vec3 v0v2 = v2 - v0;
    glm::vec3 normal = glm::normalize(glm::cross(v0v1, v0v2));
    glm::vec3 p = glm::cross(ray.direction, v0v2);
    float det = glm::dot(v0v1, p);

    if (glm::abs(det) < 1e-6)
        return false;

    float invDet = 1.0f / det;

    glm::vec3 tvec = ray.origin - v0;
    float u = glm::dot(tvec, p) * invDet;
    if (u < 0 || u > 1)
        return false;

    glm::vec3 qvec = glm::cross(tvec, v0v1);
    float v = glm::dot(ray.direction, qvec) * invDet;
    if (v < 0 || u + v > 1)
        return false;

    float t = glm::dot(v0v2, qvec) * invDet;

    if (t < tMin || tMax < t)
        return false;

    payload.worldPosition = ray.origin + t * ray.direction;
    payload.hitDistance = t;
    payload.mat = mat;
    payload.setFaceNormal(ray, normal);
    payload.u = u;
    payload.v = v;

    return true;
}
#endif

bool Triangle::boundingBox(AABB &outputBox) const
{
    outputBox = AABB(glm::min(v0, glm::min(v1, v2)), glm::max(v0, glm::max(v1, v2)));
    return true;
}

bool Triangle::renderUI()
{
    bool moved = false;
    {
        ImGui::SeparatorText("Props");
        if (ImGui::DragFloat3("###", glm::value_ptr(v0), 0.001f))
            moved = true;
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
        {
            ImGui::SetTooltip("v0");
        }
        if (ImGui::DragFloat3("###", glm::value_ptr(v1), 0.001f))
            moved = true;
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
        {
            ImGui::SetTooltip("v1");
        }
        if (ImGui::DragFloat3("###", glm::value_ptr(v2), 0.001f))
            moved = true;
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
        {
            ImGui::SetTooltip("v2");
        }
    }

    {
        ImGui::SeparatorText("Mat");
        if (mat)
            if (mat->renderUI())
                moved = true;
    }

    return moved;
}

// std::shared_ptr<Hittable> Triangle::CreateTriangle(const std::string &name)
// {

// }
