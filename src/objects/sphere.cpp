#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "objects/sphere.h"
#include "materials/lambertian.h"

Sphere::Sphere(const std::string &name)
    : Hittable(name), center(0.0f), radius(0.5f), mat(std::make_shared<Lambertian>(glm::vec3(1.0f, 0.0f, 0.0f)))
{
}

Sphere::Sphere(const std::string &name, glm::vec3 center, float radius, std::shared_ptr<Material> mat)
    : center(center), radius(radius), mat(mat), Hittable(name)
{
}

bool Sphere::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    glm::vec3 origin = ray.origin - center;
    float a = glm::dot(ray.direction, ray.direction);
    float half_b = glm::dot(origin, ray.direction);
    float c = glm::dot(origin, origin) - radius * radius;

    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0f)
        return false;

    float sqrtDiscriminant = glm::sqrt(discriminant);
    float t = (-half_b - sqrtDiscriminant) / a;
    if (t < tMin || tMax < t)
    {
        t = (-half_b + sqrtDiscriminant) / a;
        if (t < tMin || tMax < t)
            return false;
    }

    payload.hitDistance = t;
    payload.worldPosition = ray.origin + t * ray.direction;
    glm::vec3 normal = (payload.worldPosition - center) / radius;
    // glm::vec3 normal = glm::normalize(payload.worldPosition - center);
    payload.setFaceNormal(ray, normal);
    payload.mat = mat;

    payload.u = glm::atan(normal.x, normal.z) / (2.0f * glm::pi<float>()) + 0.5f;
    payload.v = normal.y * 0.5 + 0.5;

    // payload.u = (atan2(normal.x, -normal.z) / glm::pi<float>() + 1.0f) / 2.0f;
    // payload.v = asin(normal.y) / glm::pi<float>() + .5;

    return true;
}

bool Sphere::boundingBox(AABB &outputBox) const
{
    outputBox = AABB(center - glm::vec3(radius), center + glm::vec3(radius));
    return true;
}

bool Sphere::renderUI()
{
    bool moved = false;
    {
        ImGui::SeparatorText("Props");
        if (ImGui::DragFloat3("###", glm::value_ptr(center), 0.01f))
            moved = true;
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
        {
            ImGui::SetTooltip("Center");
        }
        if (ImGui::DragFloat("###", &radius, 0.01f))
            moved = true;
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
        {
            ImGui::SetTooltip("Radius");
        }
    }
    {
        ImGui::SeparatorText("Mat");
        if (mat->renderUI())
            moved = true;
    }
    return moved;
}

std::shared_ptr<Hittable> Sphere::CreateSphere(const std::string &name)
{
    return std::make_shared<Sphere>(name);
}
