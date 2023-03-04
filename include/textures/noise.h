#pragma once
#include "texture.h"

class NoiseTexture : public Texture
{
public:
    std::shared_ptr<Texture> texture;
    float scale;

    NoiseTexture(glm::vec3 color, float scale);
    NoiseTexture(std::shared_ptr<Texture> texture, float scale);

    virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const override;
    virtual bool renderUI() override;

private:
    static float turbulence(const glm::vec3 &p, int depth = 7);
};

