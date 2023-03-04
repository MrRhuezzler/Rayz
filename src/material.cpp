#include "imgui.h"
#include "materials/material.h"

void HitPayload::setFaceNormal(const Ray &ray, const glm::vec3 &outwardNormal)
{
    frontFace = glm::dot(ray.direction, outwardNormal) < 0;
    worldNormal = frontFace ? outwardNormal : -1.0f * outwardNormal;
}
