#include "imgui.h"
#include <execution>
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
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

    resetFrameIndex();
}

void Renderer::render(const Scene &scene, const Camera &camera)
{
    activeCamera = &camera;
    activeScene = &scene;

    if (frameIndex == 1)
        memset(accumulationData, 0, finalImage->getWidth() * finalImage->getHeight() * sizeof(glm::vec4));

#if OPENMP_PARALLEL
#pragma omp parallel for collapse(2)
    for (int y = 0; y < finalImage->getHeight(); y++)
        for (int x = 0; x < finalImage->getWidth(); x++)
        {
            auto color = perPixel(x, y);
            if (frameIndex < settings.maxFrames)
                accumulationData[x + y * finalImage->getWidth()] += color;

            glm::vec4 accumulatedColor = accumulationData[x + y * finalImage->getWidth()];
            accumulatedColor /= (float)frameIndex;
            accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
            imageDataToTexture[x + y * finalImage->getWidth()] = convertToABGR(accumulatedColor);
        }
#elif INHERENT_PARALLEL
    std::for_each(std::execution::par, verticalIterator.begin(), verticalIterator.end(),
                  [this](int y)
                  {
                      std::for_each(std::execution::par, horizontalIterator.begin(), horizontalIterator.end(),
                                    [this, y](int x)
                                    {
                                        if (frameIndex < settings.maxFrames)
                                        {
                                            auto color = perPixel(x, y);
                                            accumulationData[x + y * finalImage->getWidth()] += color;

                                            glm::vec4 accumulatedColor = accumulationData[x + y * finalImage->getWidth()];
                                            accumulatedColor /= (float)frameIndex;
                                            accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
                                            imageDataToTexture[x + y * finalImage->getWidth()] = convertToABGR(accumulatedColor);
                                        }
                                    });
                  });
#else
    for (int y = 0; y < finalImage->getHeight(); y++)
    {
        for (int x = 0; x < finalImage->getWidth(); x++)
        {
            auto color = perPixel(x, y);
            if (frameIndex < settings.maxFrames)
                accumulationData[x + y * finalImage->getWidth()] += color;

            glm::vec4 accumulatedColor = accumulationData[x + y * finalImage->getWidth()];
            accumulatedColor /= (float)frameIndex;
            accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
            imageDataToTexture[x + y * finalImage->getWidth()] = convertToABGR(accumulatedColor);
        }
    }
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

void Renderer::renderUI()
{
    ImGui::Begin("Renderer");

    ImGui::SeparatorText("Status");
    ImGui::Text("Samples: %d / %d", frameIndex, settings.maxFrames);

    ImGui::SeparatorText("Settings");
    ImGui::Checkbox("Accumulate", &settings.accumulate);
    ImGui::InputInt("Max Sample frames", &settings.maxFrames);
    if (ImGui::ColorEdit3("Background Color", glm::value_ptr(settings.backgroundColor)))
    {
        resetFrameIndex();
    }

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

    glm::vec3 color(0.0f);
    glm::vec3 attenuation(1.0f);

    int bounces = 10;
    for (int i = 0; i < bounces; i++)
    {
        if (activeScene->hit(ray, 0.001f, std::numeric_limits<float>::max(), payload))
        {
            glm::vec3 emission = payload.mat->emitted(ray, payload, payload.u, payload.v, payload.worldPosition);
            if (payload.mat->scatter(ray, payload, attenuation, scattered))
            {
                color += (attenuation * emission);
                ray.origin = scattered.origin;
                ray.direction = scattered.direction;
            }
            else
            {
                color += (attenuation * emission);
                break;
            }
        }
        else
        {
            color += (attenuation * settings.backgroundColor);
            break;
        }
    }

    return glm::vec4(color, 1.0f);
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