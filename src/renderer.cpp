#include <execution>
#include "glm/gtc/random.hpp"
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

    delete[] imageData;
    imageData = new uint32_t[width * height];

    delete[] accumulationData;
    accumulationData = new glm::vec4[width * height];

    verticalIterator.resize(height);
    for (int i = 0; i < height; i++)
        verticalIterator[i] = i;

    horizontalIterator.resize(width);
    for (int i = 0; i < width; i++)
        horizontalIterator[i] = i;
}

void Renderer::render(const Scene &scene, const Camera &camera)
{
    activeCamera = &camera;
    activeScene = &scene;

    if (frameIndex == 1)
        memset(accumulationData, 0, finalImage->getWidth() * finalImage->getHeight() * sizeof(glm::vec4));

#define MT
#ifndef MT
    for (int y = 0; y < finalImage->getHeight(); y++)
    {
        for (int x = 0; x < finalImage->getWidth(); x++)
        {
            // ray.direction = camera.getRayDirections()[x + y * finalImage->getWidth()];
            // auto color = traceRay(ray);
            auto color = perPixel(x, y);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            imageData[x + y * finalImage->getWidth()] = convertToABGR(color);
        }
    }
#else
    std::for_each(std::execution::par, verticalIterator.begin(), verticalIterator.end(),
                  [this](int y)
                  {
                      std::for_each(std::execution::par, horizontalIterator.begin(), horizontalIterator.end(),
                                    [this, y](int x)
                                    {
                                        auto color = perPixel(x, y);
                                        if (frameIndex < settings.maxFrames)
                                            accumulationData[x + y * finalImage->getWidth()] += color;

                                        glm::vec4 accumulatedColor = accumulationData[x + y * finalImage->getWidth()];
                                        accumulatedColor /= (float)frameIndex;
                                        accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
                                        imageData[x + y * finalImage->getWidth()] = convertToABGR(accumulatedColor);
                                    });
                  });
#endif
    finalImage->setData(imageData);

    if (settings.accumulate)
    {
        if (frameIndex < settings.maxFrames)
            frameIndex++;
    }
    else
        frameIndex = 1;
}

void Renderer::resetFrameIndex()
{
    frameIndex = 1;
}

Renderer::Settings &Renderer::getSettings()
{
    return settings;
}

glm::vec4 Renderer::perPixel(int x, int y)
{
    Ray ray;
    ray.origin = activeCamera->getPosition();
    ray.direction = activeCamera->getRayDirections()[x + y * finalImage->getWidth()];

    glm::vec3 color(0.0f);
    glm::vec3 skyColor(0.53f, 0.81f, 0.92f);

    float multiplier = 1.0f;
    int bounces = 5;

    for (int i = 0; i < bounces; i++)
    {
        auto payload = traceRay(ray);
        if (payload.hitDistance < 0)
        {
            color += skyColor * multiplier;
            break;
        }

        const Sphere &closetSphere = activeScene->spheres[payload.objectIndex];
        glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
        float lightIntensity = glm::max(glm::dot(payload.worldNormal, -lightDir), 0.0f); // == cos(angle)

        glm::vec3 sphereColor = closetSphere.mat.albedo;
        sphereColor *= lightIntensity;
        color += sphereColor * multiplier;

        multiplier *= 0.3f;

        ray.origin = payload.worldPosition + payload.worldNormal * 0.0001f;
        glm::vec3 roughnessMul = glm::vec3(glm::linearRand(glm::vec2(-0.5f), glm::vec2(0.5f)), 0);
        glm::vec3 reflectNormal = payload.worldNormal + closetSphere.mat.roughness * roughnessMul;
        ray.direction = glm::reflect(ray.direction, reflectNormal);
    }

    return glm::vec4(color, 1.0f);
}

Renderer::HitPayload Renderer::traceRay(const Ray &ray)
{

    int closetIndex = -1;
    float hitDistance = std::numeric_limits<float>::max();

    for (int i = 0; i < activeScene->spheres.size(); i++)
    {
        const Sphere &sphere = activeScene->spheres[i];
        glm::vec3 origin = ray.origin - sphere.position;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(origin, ray.direction);
        float c = glm::dot(origin, origin) - sphere.radius * sphere.radius;

        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0.0f)
            continue;

        float closetT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
        if (closetT > 0.0f && closetT < hitDistance)
        {
            hitDistance = closetT;
            closetIndex = i;
        }
    }

    if (closetIndex < 0)
        return miss(ray);

    return closetHit(ray, hitDistance, closetIndex);
}

Renderer::HitPayload Renderer::closetHit(const Ray &ray, float hitDistance, int objectIndex)
{

    Renderer::HitPayload payload;
    payload.hitDistance = hitDistance;
    payload.objectIndex = objectIndex;

    const Sphere &closetSphere = activeScene->spheres[objectIndex];

    glm::vec3 origin = ray.origin - closetSphere.position;
    payload.worldPosition = origin + hitDistance * ray.direction;
    payload.worldNormal = glm::normalize(payload.worldPosition);

    payload.worldPosition += closetSphere.position;

    return payload;
}

Renderer::HitPayload Renderer::miss(const Ray &ray)
{
    Renderer::HitPayload payload;
    payload.hitDistance = -1;
    return payload;
}

uint32_t Renderer::convertToABGR(const glm::vec4 &color)
{
    uint8_t r = color.r * 255;
    uint8_t g = color.g * 255;
    uint8_t b = color.b * 255;
    uint8_t a = color.a * 255;

    return (a << 24) | (b << 16) | (g << 8) | r;
}

std::shared_ptr<Image> Renderer::getFinalImage()
{
    return finalImage;
}