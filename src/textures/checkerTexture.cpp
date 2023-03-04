#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "textures/solidColor.h"
#include "textures/checker.h"

CheckerTexture::CheckerTexture(glm::vec3 odd, glm::vec3 even)
    : CheckerTexture(std::make_shared<SolidColor>(odd), std::make_shared<SolidColor>(even))
{
}

CheckerTexture::CheckerTexture(const std::shared_ptr<Texture> &odd, const std::shared_ptr<Texture> &even)
    : odd(odd), even(even)
{
}

glm::vec3 CheckerTexture::value(float u, float v, const glm::vec3 &p) const
{
    float sines = glm::sin(10.0f * p.x) * glm::sin(10.0f * p.y) * glm::sin(10.0f * p.z);
    if (sines < 0.0f)
        return odd->value(u, v, p);
    else
        return even->value(u, v, p);
}

bool CheckerTexture::renderUI()
{
    bool moved = false;
    if (odd->renderUI())
        moved = true;
    if (even->renderUI())
        moved = true;
    return moved;
}
