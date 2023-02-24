#pragma once

#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include "jug/image.h"

#include "camera.h"
#include "ray.h"
#include "hittable.h"

using namespace Jug;

class Renderer
{
public:
    struct Settings
    {
        bool accumulate = true;
        int maxFrames = 1000;
        glm::vec3 backgroundColor = glm::vec3(0.5f, 0.7f, 1.0f);
    };

    struct Status
    {
        int currentSample = 0;
    };

    Renderer();
    void onResize(uint32_t width, uint32_t height);
    void render(const Scene &scene, const Camera &camera);
    void resetFrameIndex();

    void renderUI();
    void saveImage();

    Settings &getSettings();
    Status getStatus();

    std::shared_ptr<Image>
    getFinalImage();

private:
    const Camera *activeCamera;
    const Scene *activeScene;
    Settings settings;

    std::shared_ptr<Image> finalImage;
    uint32_t *imageDataToTexture = nullptr;
    glm::vec4 *accumulationData = nullptr;

    int frameIndex = 1;

    std::vector<int> verticalIterator, horizontalIterator;

    glm::vec4 perPixel(int x, int y);
    // HitPayload traceRay(const Ray &ray);
    // HitPayload closetHit(const Ray &ray, float hitDistance, int objectIndex);
    // HitPayload miss(const Ray &ray);

    static uint32_t convertToABGR(const glm::vec4 &color);
};
