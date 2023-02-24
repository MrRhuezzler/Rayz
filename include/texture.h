#pragma once

#include <memory>
#include "glm/glm.hpp"

class Texture
{
public:
    virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const = 0;
    virtual bool renderUI()
    {
        return false;
    };
};

class SolidColor : public Texture
{
public:
    glm::vec3 color;
    SolidColor(const glm::vec3 &color);

    virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const override;
    virtual bool renderUI() override;
};

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

class ImageTexture : public Texture
{
public:
    int channels = 3;

    ImageTexture();
    ImageTexture(const char *fileName);
    void loadData(const char *fileName);
    ~ImageTexture();

    virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const override;
    virtual bool renderUI() override;

private:
    unsigned char *data;
    int width, height;
    int stride;
};