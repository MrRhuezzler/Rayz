#include "imgui.h"
#include <iostream>
#include "stb/stb_image.h"
#include "glm/gtc/type_ptr.hpp"
#include "texture.h"
#include "jug/fileDialog.h"

SolidColor::SolidColor(const glm::vec3 &color)
    : color(color)
{
}

glm::vec3 SolidColor::value(float u, float v, const glm::vec3 &p) const
{
    return color;
}

bool SolidColor::renderUI()
{
    bool moved = false;
    if (ImGui::ColorEdit3("###", glm::value_ptr(color)))
        moved = true;
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
    {
        ImGui::SetTooltip("Albedo");
    }
    return moved;
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

bool CheckerTexture::renderUI()
{
    bool moved = false;
    if (odd->renderUI())
        moved = true;
    if (even->renderUI())
        moved = true;
    return moved;
}

ImageTexture::ImageTexture()
    : data(nullptr), width(0), height(0), stride(0), fileName("textures/default.jpeg")
{
}

ImageTexture::ImageTexture(const char *fileName)
    : fileName(fileName)
{
    loadData(fileName);
}

bool ImageTexture::loadData(const char *fileName)
{
    data = stbi_load(fileName, &width, &height, &channels, channels);
    if (!data)
    {
        std::cout << "error loading texture" << std::endl;
        width = 0;
        height = 0;
    }
    stride = channels * width;
    this->fileName = fileName;
    return width != 0;
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

bool ImageTexture::renderUI()
{
    bool moved = false;
    float x = ImGui::GetContentRegionAvail().x;
    if (ImGui::Button("Load Texture", ImVec2(x, 0.0f)))
    {
        std::string filePath = Jug::FileDialog::openFile("Image Files (*.png|*.jpeg|*.jpg)\0*.png;*.jpeg;*.jpg\0PNG (*.png)\0*.png\0JPEG (*.jpeg)\0*.jpeg\0JPG (*.jpg)\0*.jpg\0");
        if (!filePath.empty())
        {
            loadData(filePath.c_str());
            moved = true;
        }
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
    {
        ImGui::SetTooltip("Current: %s", fileName.c_str());
    }

    return moved;
}
