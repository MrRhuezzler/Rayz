#include <memory>

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include "jug/application.h"
#include "jug/timer.h"
#include "jug/input.h"

#include "objects.h"
#include "textures.h"
#include "materials.h"
#include "scene.h"

#include "camera.h"
#include "renderer.h"
// #include "bvhNode.h"

using namespace Jug;
class RayTracingLayer : public Layer
{
public:
    RayTracingLayer()
        : camera(45.0f, 0.1f, 100.0f), scene("Main Scene")
    {
        auto earthImage = std::make_shared<ImageTexture>("textures/earthmap.jpg");
        auto earth = std::make_shared<Lambertian>(earthImage);
        auto colored1 = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.2f, 0.1f));
        auto colored2 = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.2f, 0.1f));
        auto emissive = std::make_shared<DiffuseLight>(glm::vec3(1.0f, 1.0f, 1.0f));
        auto mirror = std::make_shared<Dieletric>(glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);

        scene.add(std::make_shared<Plane>("P1", glm::vec3(0.0f, -0.6f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), mirror));
        scene.add(std::make_shared<Triangle>("T1", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), emissive));
        scene.add(std::make_shared<Triangle>("T2", glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 1.0f, 0.0f), emissive));
        scene.add(std::make_shared<Sphere>("S1", glm::vec3(0.0f, 0.5f, 0.8f), 0.5f, earth));
    }

    virtual void OnUpdate(float ts) override
    {
        if (camera.onUpdate(ts))
            renderer.resetFrameIndex();
    }

    virtual void OnUIRender() override
    {
        ImGui::Begin("Status");
        ImGui::Text("Last Render: %.3fms", lastRenderTime);
        ImGui::Text("Frame Rate: %.3fFPS", frameRate);
        ImGui::End();

        renderer.renderUI();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("Viewport", nullptr);
        viewportWidth = ImGui::GetContentRegionAvail().x;
        viewportHeight = ImGui::GetContentRegionAvail().y;

        auto image = renderer.getFinalImage();
        if (image)
            ImGui::Image((void *)(intptr_t)image->getDescriptor(), {(float)image->getWidth(), (float)image->getHeight()}, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        ImGui::PopStyleVar(3);

        if (scene.renderUI())
            renderer.resetFrameIndex();

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
    Application *app = Application::createInstance("Rayz", 1000, 700);

    std::shared_ptr<Layer> layer = std::make_shared<RayTracingLayer>();
    app->addLayer(layer);

    app->run();
    delete app;
    return 0;
}