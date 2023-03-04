#pragma once
#include "textures/texture.h"

class SolidColor : public Texture
{
public:
    glm::vec3 color;
    SolidColor(const glm::vec3 &color);

    virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const override;
    virtual bool renderUI() override;
};
