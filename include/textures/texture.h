#pragma once

#include <string>
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
