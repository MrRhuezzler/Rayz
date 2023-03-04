#pragma once
#include "textures/texture.h"

class CheckerTexture : public Texture
{
public:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;

    CheckerTexture(glm::vec3 odd, glm::vec3 even);
    CheckerTexture(const std::shared_ptr<Texture> &odd, const std::shared_ptr<Texture> &even);

    virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const override;
    virtual bool renderUI() override;
};
