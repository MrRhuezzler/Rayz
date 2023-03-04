#pragma once
#include "textures/texture.h"

class ImageTexture : public Texture
{
public:
    int channels = 3;

    ImageTexture();
    ImageTexture(const char *fileName);
    bool loadData(const char *fileName);
    ~ImageTexture();

    virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const override;
    virtual bool renderUI() override;

private:
    unsigned char *data;
    int width, height;
    int stride;
    std::string fileName;
};