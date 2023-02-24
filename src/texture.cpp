#include <iostream>
#include "stb/stb_image.h"
#include "texture.h"

SolidColor::SolidColor(const glm::vec3 &color)
    : color(color)
{
}

glm::vec3 SolidColor::value(float u, float v, const glm::vec3 &p) const
{
    return color;
}

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

ImageTexture::ImageTexture()
    : data(nullptr), width(0), height(0), stride(0)
{
}

ImageTexture::ImageTexture(const char *fileName)
{
    data = stbi_load(fileName, &width, &height, &channels, channels);

    if (!data)
    {
        std::cout << "error loading texture" << std::endl;
        width = 0;
        height = 0;
    }

    stride = channels * width;
}

ImageTexture::~ImageTexture()
{
    delete data;
}

glm::vec3 ImageTexture::value(float u, float v, const glm::vec3 &p) const
{
    if (!data)
        return glm::vec3(0, 1, 1);

    float cU = glm::clamp<float>(u, 0.0, 1.0);
    float cV = 1.0 - glm::clamp<float>(v, 0.0, 1.0);

    int i = (int)(cU * width);
    int j = (int)(cV * height);

    if (i >= width)
        i = width - 1;

    if (j >= height)
        j - height - 1;

    const float colorScale = 1.0f / 255.0f;
    auto pixel = data + i * channels + j * stride;

    return glm::vec3(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
}
