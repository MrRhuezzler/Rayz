#pragma once

#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include "jug/image.h"

#include "camera.h"
#include "ray.h"
#include "scene.h"

using namespace Jug;

class Renderer
{
public:
    struct Settings
    {
        bool accumulate = true;
        int maxFrames = 60;
    };

    Renderer();
    void onResize(uint32_t width, uint32_t height);
    void render(const Scene &scene, const Camera &camera);
    void resetFrameIndex();
    Settings &getSettings();

    std::shared_ptr<Image> getFinalImage();

private:
    struct HitPayload
    {
        float hitDistance;
        glm::vec3 worldPosition;
        glm::vec3 worldNormal;

        int objectIndex;
    };

    const Camera *activeCamera;
    const Scene *activeScene;
    Settings settings;

    std::shared_ptr<Image> finalImage;
    uint32_t *imageData;
    glm::vec4 *accumulationData = nullptr;

    int frameIndex = 1;

    std::vector<int> verticalIterator, horizontalIterator;

    glm::vec4 perPixel(int x, int y);
    HitPayload traceRay(const Ray &ray);
    HitPayload closetHit(const Ray &ray, float hitDistance, int objectIndex);
    HitPayload miss(const Ray &ray);

    static uint32_t convertToABGR(const glm::vec4 &color);
};
