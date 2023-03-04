#include <iostream>

#include "glm/gtc/constants.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "hittable.h"

Hittable::Hittable(const std::string &name)
    : name(name)
{
}
