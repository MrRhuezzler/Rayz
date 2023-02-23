#include "jug/input.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "camera.h"

using namespace Jug;

Camera::Camera(float verticalFOV, float nearClip, float farClip)
    : projection(1.0f), view(1.0f), inverseProjection(1.0f), inverseView(1.0f), verticalFOV(verticalFOV), nearClip(nearClip), farClip(farClip), position(0.0f, 0.0f, 6.0f), forwardDirection(0.0f, 0.0f, -1.0f), lastMousePosition(0.0f, 0.0f), viewportHeight(0), viewportWidth(0)
{
}

bool Camera::onUpdate(float ts)
{
    glm::vec2 mousePos = Input::getMousePosition();
    glm::vec2 delta = (mousePos - lastMousePosition) * 0.002f;
    lastMousePosition = mousePos;

    if (!Input::isMouseButtonDown(MouseButton::Right))
    {
        Input::setCursorMode(CursorMode::Normal);
        return false;
    }

    Input::setCursorMode(CursorMode::Locked);
    bool moved = false;

    constexpr glm::vec3 upDir(0.0f, 1.0f, 0.0f);
    glm::vec3 rightDir = glm::cross(forwardDirection, upDir);

    float speed = 5.0f;

    if (Input::isKeyDown(KeyCode::W))
    {
        position += forwardDirection * speed * ts;
        moved = true;
    }
    else if (Input::isKeyDown(KeyCode::S))
    {
        position -= forwardDirection * speed * ts;
        moved = true;
    }

    if (Input::isKeyDown(KeyCode::A))
    {
        position -= rightDir * speed * ts;
        moved = true;
    }
    else if (Input::isKeyDown(KeyCode::D))
    {
        position += rightDir * speed * ts;
        moved = true;
    }

    if (Input::isKeyDown(KeyCode::LeftControl))
    {
        position -= upDir * speed * ts;
        moved = true;
    }
    else if (Input::isKeyDown(KeyCode::Space))
    {
        position += upDir * speed * ts;
        moved = true;
    }

    // Rotation
    if (delta.x != 0.0f || delta.y != 0.0f)
    {
        float pitchDelta = delta.y * getRotationSpeed();
        float yawDelta = delta.x * getRotationSpeed();

        glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDir),
                                                glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
        forwardDirection = glm::rotate(q, forwardDirection);
        moved = true;
    }

    if (moved)
    {
        recalculateView();
        recalculateRayDirections();
    }

    return moved;
}

void Camera::onResize(int width, int height)
{

    if (width <= 0 || height <= 0 || (width == viewportWidth && height == viewportHeight))
        return;

    viewportWidth = width;
    viewportHeight = height;

    recalculateProjection();
    recalculateRayDirections();
}

const glm::mat4 &Camera::getProjection() const
{
    return projection;
}

const glm::mat4 &Camera::getInverseProjection() const
{
    return inverseProjection;
}

const glm::mat4 &Camera::getView() const
{
    return view;
}

const glm::mat4 &Camera::getInverseView() const
{
    return inverseView;
}

const glm::vec3 &Camera::getPosition() const
{
    return position;
}

const glm::vec3 &Camera::getDirection() const
{
    return forwardDirection;
}

const std::vector<glm::vec3> &Camera::getRayDirections() const
{
    return rayDirections;
}

float Camera::getRotationSpeed()
{
    return 0.3f;
}

void Camera::recalculateProjection()
{
    projection = glm::perspectiveFov(glm::radians(verticalFOV), (float)viewportWidth, (float)viewportHeight, nearClip, farClip);
    inverseProjection = glm::inverse(projection);
}

void Camera::recalculateView()
{
    view = glm::lookAt(position, position + forwardDirection, glm::vec3(0, 1, 0));
    inverseView = glm::inverse(view);
}

void Camera::recalculateRayDirections()
{
    rayDirections.resize(viewportWidth * viewportHeight);
    for (int y = 0; y < viewportHeight; y++)
    {
        for (int x = 0; x < viewportWidth; x++)
        {
            glm::vec2 coord = {(float)x / (float)viewportWidth, (float)y / (float)viewportHeight};
            coord = coord * 2.0f - 1.0f; // -1 -> 1

            glm::vec4 target = inverseProjection * glm::vec4(coord.x, coord.y, 1, 1);
            glm::vec3 rayDirection = glm::vec3(inverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
            rayDirections[x + y * viewportWidth] = rayDirection;
        }
    }
}
