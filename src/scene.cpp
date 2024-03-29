#include "imgui.h"
#include "scene.h"
#include "objects.h"
#include "materials.h"
#include "textures.h"

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
        if (ImGui::MenuItem("Sphere"))
            currentAdding = Scene::OBJECTS::SPHERE;
        if (ImGui::MenuItem("Plane"))
            currentAdding = Scene::OBJECTS::PLANE;
        ImGui::EndPopup();
    }

    ImGui::Begin("Add Object");
    if (currentAdding == Scene::OBJECTS::SPHERE)
    {
        ImGui::Text("Adding Sphere");
        ImGui::Separator();

        static char sphereNameBuf[64] = "";
        ImGui::InputText("Name", sphereNameBuf, 64);

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            add(std::make_shared<Sphere>(sphereNameBuf));
            moved = true;
            currentAdding = Scene::OBJECTS::NONE;
        }
    }

    else if (currentAdding == Scene::OBJECTS::PLANE)
    {
        ImGui::Text("Adding Plane");
        ImGui::Separator();

        static char planeNameBuf[64] = "";
        ImGui::InputText("Name", planeNameBuf, 64);

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            add(std::make_shared<Plane>(planeNameBuf, glm::vec3(0.0f, -0.6f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), std::make_shared<Lambertian>(glm::vec3(1.0f, 0.0f, 0.0f))));
            moved = true;
            currentAdding = Scene::OBJECTS::NONE;
        }
    }
    ImGui::End();

    if (treeopen)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            const auto &object = objects[i];
            if (ImGui::TreeNode((void *)(intptr_t)i, object->name.c_str(), i))
            {
                ImGui::PushID(i + 1);
                float x = ImGui::GetContentRegionAvail().x;
                ImGui::PushItemWidth(x);
                if (object->renderUI())
                    moved = true;
                ImGui::PopItemWidth();
                ImGui::PopID();
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
    ImGui::End();

    return moved;
}
