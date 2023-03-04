#pragma once

#include "materials/material.h"
#include "hittable.h"

class Scene : public Hittable
{
private:
    // static std:
public:
    Scene(const std::string &name);
    Scene(const std::string &name, const std::shared_ptr<Hittable> &object);

    void clear();
    void add(const std::shared_ptr<Hittable> &object);

    const std::vector<std::shared_ptr<Hittable>> &getObjects() const;

    virtual bool hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const override;
    virtual bool boundingBox(AABB &outputox) const override;
    virtual bool renderUI() override;

    std::vector<std::shared_ptr<Hittable>> objects;
};
