#pragma once

#include <vector>

#include "glm/glm.hpp"

class Camera
{
private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 inverseProjection;
    glm::mat4 inverseView;

    float verticalFOV, nearClip, farClip;
    glm::vec3 position;
    glm::vec3 forwardDirection;

    // Cached ray directions
    std::vector<glm::vec3> rayDirections;

    glm::vec2 lastMousePosition;

    uint32_t viewportWidth, viewportHeight;

public:
    Camera(float verticalFOV, float nearClip, float farClip);

    bool onUpdate(float ts);
    void onResize(int width, int height);

    const glm::mat4 &getProjection() const;
    const glm::mat4 &getInverseProjection() const;
    const glm::mat4 &getView() const;
    const glm::mat4 &getInverseView() const;

    const glm::vec3 &getPosition() const;
    const glm::vec3 &getDirection() const;
    float getRotationSpeed();

    const std::vector<glm::vec3> &getRayDirections() const;

private:
    void recalculateProjection();
    void recalculateView();
    void recalculateRayDirections();
};