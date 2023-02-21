#include "renderer.h"

Renderer::Renderer()
{
}

void Renderer::onResize(uint32_t width, uint32_t height)
{
    if (finalImage)
    {
        if (finalImage->getWidth() == width && finalImage->getHeight() == height)
            return;

        finalImage->resize(width, height);
    }
    else
    {
        finalImage = std::make_shared<Image>(width, height);
    }

    delete imageData;
    imageData = new uint32_t[width * height];
}

void Renderer::render()
{
    for (uint32_t y = 0; y < finalImage->getHeight(); y++)
    {
        for (uint32_t x = 0; x < finalImage->getWidth(); x++)
        {
            // 0 -> 1
            glm::vec2 coord = {(float)x / (float)finalImage->getWidth(), (float)y / (float)finalImage->getHeight()};
            coord = coord * 2.0f - 1.0f;

            auto color = perPixel(coord);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

            imageData[x + y * finalImage->getWidth()] = convertToABGR(color);
        }
    }
    finalImage->setData(imageData);
}

std::shared_ptr<Image> Renderer::getFinalImage()
{
    return finalImage;
}

glm::vec4 Renderer::perPixel(glm::vec2 coord)
{
    glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
    glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
    float radius = 0.5f;
    // rayDirection = glm::normalize(rayDirection);

    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(rayOrigin, rayDirection);
    float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0f)
        return glm::vec4(0, 0, 0, 1);

    // float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
    float closetT = (-b - glm::sqrt(discriminant)) / (2.0f * a);

    // glm::vec3 h0 = rayOrigin + rayDirection * t0;
    glm::vec3 hitPoint = rayOrigin + rayDirection * closetT;
    glm::vec3 normal = glm::normalize(hitPoint);

    glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
    float intensity = glm::max(glm::dot(normal, -lightDir), 0.1f);

    glm::vec3 sphereColor(1, 0, 1);
    sphereColor *= intensity;
    return glm::vec4(sphereColor, 1);
}

uint32_t Renderer::convertToABGR(const glm::vec4 &color)
{
    uint8_t r = color.r * 255;
    uint8_t g = color.g * 255;
    uint8_t b = color.b * 255;
    uint8_t a = color.a * 255;

    return (a << 24) | (b << 16) | (g << 8) | r;
}
