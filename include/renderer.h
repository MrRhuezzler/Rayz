#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <memory>

#include "glm/glm.hpp"
#include "jug/image.h"

using namespace Jug;

class Renderer
{
public:
    Renderer();
    void onResize(uint32_t width, uint32_t height);
    void render();

    std::shared_ptr<Image> getFinalImage();

private:
    std::shared_ptr<Image> finalImage;
    uint32_t *imageData;

    glm::vec4 perPixel(glm::vec2 coord);

    static uint32_t convertToABGR(const glm::vec4 &color);
};

#endif // __RENDERER_H__