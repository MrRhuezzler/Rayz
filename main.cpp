#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include "jug/application.h"
#include "jug/timer.h"
#include "jug/input.h"

#include "camera.h"
#include "renderer.h"

using namespace Jug;
class ExampleLayer : public Layer
{
public:
    ExampleLayer()
        : camera(45.0f, 0.1f, 100.0f)
    {
        {
            Sphere sphere;
            sphere.position = {0.0f, 0.0f, 0.0f};
            sphere.radius = 0.5f;
            sphere.mat.albedo = {1.0f, 0.0f, 1.0f};
            scene.spheres.push_back(sphere);
        }

        {
            Sphere sphere;
            sphere.position = {1.0f, 0.0f, -5.0f};
            sphere.radius = 1.5f;
            sphere.mat.albedo = {0.2f, 0.3f, 1.0f};
            scene.spheres.push_back(sphere);
        }
    }

    virtual void OnUpdate(float ts) override
    {
        if (camera.onUpdate(ts))
            renderer.resetFrameIndex();
    }

    virtual void OnUIRender() override
    {
        ImGui::Begin("Settings");
        ImGui::Text("Last Render: %.3fms", lastRenderTime);
        ImGui::Text("Frame Rate: %.3fFPS", frameRate);
        ImGui::End();

        ImGui::Begin("Properties");
        for (size_t i = 0; i < scene.spheres.size(); i++)
        {
            ImGui::PushID(i);
            Sphere &sphere = scene.spheres[i];
            if (ImGui::DragFloat3("Position", glm::value_ptr(sphere.position), 0.1f))
                renderer.resetFrameIndex();
            if (ImGui::DragFloat("Radius", &sphere.radius, 0.01f))
                renderer.resetFrameIndex();
            if (ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.mat.albedo)))
                renderer.resetFrameIndex();
            if (ImGui::DragFloat("Roughness", &sphere.mat.roughness, 0.01f))
                renderer.resetFrameIndex();
            if (ImGui::DragFloat("Metallic", &sphere.mat.metallic, 0.01f))
                renderer.resetFrameIndex();
            ImGui::Separator();
            ImGui::PopID();
        }
        ImGui::End();

        ImGui::Begin("Viewport");
        viewportWidth = ImGui::GetContentRegionAvail().x;
        viewportHeight = ImGui::GetContentRegionAvail().y;

        auto image = renderer.getFinalImage();
        if (image)
            ImGui::Image((void *)(intptr_t)image->getDescriptor(), {(float)image->getWidth(), (float)image->getHeight()}, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        render();
    }

    void render()
    {
        Timer timer;
        camera.onResize(viewportWidth, viewportHeight);
        renderer.onResize(viewportWidth, viewportHeight);
        renderer.render(scene, camera);
        lastRenderTime = timer.getTimeElapsedMillis();
        frameRate = 100.0f / lastRenderTime;
    }

private:
    Renderer renderer;
    uint32_t viewportWidth = 0,
             viewportHeight = 0;
    float lastRenderTime = 0;
    float frameRate = 0;
    Camera camera;
    Scene scene;
};

int main()
{
    Application *app = Application::createInstance("Rayz", 800, 600);

    std::shared_ptr<Layer> layer = std::make_shared<ExampleLayer>();
    app->addLayer(layer);

    app->run();
    delete app;
    return 0;
}