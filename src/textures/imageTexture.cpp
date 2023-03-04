#include <iostream>
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "stb/stb_image.h"
#include "jug/fileDialog.h"
#include "textures/image.h"


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

