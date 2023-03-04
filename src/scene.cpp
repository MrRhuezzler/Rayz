#include "imgui.h"
#include "scene.h"

Scene::Scene(const std::string &name)
    : Hittable(name)
{
}

Scene::Scene(const std::string &name, const std::shared_ptr<Hittable> &object)
    : Hittable(name)
{
    add(object);
}

void Scene::clear()
{
    objects.clear();
}

void Scene::add(const std::shared_ptr<Hittable> &object)
{
    objects.push_back(object);
}

const std::vector<std::shared_ptr<Hittable>> &Scene::getObjects() const
{
    return objects;
}

bool Scene::hit(const Ray &ray, float tMin, float tMax, HitPayload &payload) const
{
    HitPayload tempPayload;
    bool hitAnything = false;

    auto closestSoFar = tMax;

    for (const auto &object : objects)
    {
        if (object->hit(ray, tMin, closestSoFar, tempPayload))
        {
            hitAnything = true;
            closestSoFar = tempPayload.hitDistance;
            payload = tempPayload;
        }
    }

    return hitAnything;
}

bool Scene::boundingBox(AABB &outputBox) const
{
    if (objects.empty())
        return false;

    AABB temp;
    bool firstBox = true;

    for (const auto &object : objects)
    {
        if (!object->boundingBox(temp))
            return false;
        outputBox = firstBox ? temp : AABB::surroundingBox(outputBox, temp);
        firstBox = false;
    }

    return true;
}

bool Scene::renderUI()
{
    bool moved = false;
    ImGui::Begin(name.c_str());

    bool treeopen = ImGui::TreeNodeEx("Objects", ImGuiTreeNodeFlags_AllowItemOverlap);
    ImGui::SameLine();

    if (ImGui::Button("+"))
        ImGui::OpenPopup("Object List");

    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Add Object");

    if (ImGui::BeginPopup("Object List"))
    {
        ImGui::MenuItem("element 1");
        ImGui::MenuItem("element 2");
        ImGui::EndPopup();
    }

    if (treeopen)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            const auto &object = objects[i];
            // ImGui::PushID(i + 1);
            if (ImGui::TreeNode((void *)(intptr_t)i, object->name.c_str(), i))
            {
                float x = ImGui::GetContentRegionAvail().x;
                ImGui::PushItemWidth(x);
                if (object->renderUI())
                    moved = true;
                ImGui::PopItemWidth();
                ImGui::TreePop();
            }
            // ImGui::PopID();
        }
        ImGui::TreePop();
    }
    ImGui::End();

    return moved;
}
