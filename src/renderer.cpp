#include "imgui.h"
#include <execution>
#include "glm/gtc/random.hpp"
#include "renderer.h"
#include "jug/fileDialog.h"

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

    delete[] imageDataToTexture;
    imageDataToTexture = new uint32_t[width * height];

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
                                        imageDataToTexture[x + y * finalImage->getWidth()] = convertToABGR(accumulatedColor);
                                    });
                  });
#endif
    finalImage->setData(imageDataToTexture);

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

void Renderer::onUIRender()
{
    ImGui::Begin("Renderer");

    ImGui::SeparatorText("Status");
    ImGui::Text("Samples: %d / %d", frameIndex, settings.maxFrames);

    ImGui::SeparatorText("Settings");
    ImGui::Checkbox("Accumulate", &settings.accumulate);
    ImGui::InputInt("Max Sample frames", &settings.maxFrames);

    ImGui::SeparatorText("Output");
    if (ImGui::Button("Save"))
    {
        saveImage();
    }

    ImGui::End();
}

void Renderer::saveImage()
{
    std::string filePath = FileDialog::saveFile("PNG (*.png)\0*.png\0");
    if (!filePath.empty())
        Image::saveData(filePath.c_str(), finalImage->getWidth(), finalImage->getHeight(), 4, imageDataToTexture, finalImage->getWidth() * sizeof(uint32_t));
}

Renderer::Settings &Renderer::getSettings()
{
    return settings;
}

Renderer::Status Renderer::getStatus()
{
    return {frameIndex};
}

glm::vec4 Renderer::perPixel(int x, int y)
{
    Ray ray, scattered;
    ray.origin = activeCamera->getPosition();
    ray.direction = activeCamera->getRayDirections()[x + y * finalImage->getWidth()];

    HitPayload payload;

    float multiplier = 1.0f;

    glm::vec3 skyColor(0.5f, 0.7f, 1.0f);
    glm::vec3 black(0.0f);
    glm::vec3 color(1.0f);

    int bounces = 10;
    for (int i = 0; i < bounces; i++)
    {
        if (activeScene->hit(ray, 0.001f, std::numeric_limits<float>::max(), payload))
        {
            glm::vec3 attenuation;
            if (payload.mat->scatter(ray, payload, attenuation, scattered))
            {
                color *= attenuation;
            }
            else
            {
                color *= black;
            }

            ray.origin = scattered.origin;
            ray.direction = scattered.direction;
        }
        else
        {
            color *= skyColor;
            break;
        }
    }

    return glm::vec4(color, 1.0f);
}

// HitPayload Renderer::traceRay(const Ray &ray)
// {
// }

// HitPayload Renderer::closetHit(const Ray &ray, float hitDistance, int objectIndex)
// {

//     HitPayload payload;
//     payload.hitDistance = hitDistance;
//     payload.objectIndex = objectIndex;

//     const Sphere &closetSphere = activeScene->spheres[objectIndex];

//     glm::vec3 origin = ray.origin - closetSphere.position;
//     payload.worldPosition = origin + hitDistance * ray.direction;
//     payload.worldNormal = glm::normalize(payload.worldPosition);

//     payload.worldPosition += closetSphere.position;

//     return payload;
// }

// HitPayload Renderer::miss(const Ray &ray)
// {
//     HitPayload payload;
//     payload.hitDistance = -1;
//     return payload;
// }

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