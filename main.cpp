#include "imgui.h"
#include "jug/application.h"
#include "jug/timer.h"
#include "renderer.h"

using namespace Jug;
class ExampleLayer : public Layer
{
public:
    virtual void OnUpdate(float ts) override
    {
        render();
    }

    virtual void OnUIRender() override
    {
        ImGui::Begin("Settings");
        ImGui::Text("Last Render: %.3fms", lastRenderTime);
        ImGui::End();

        ImGui::Begin("Viewport");
        viewportWidth = ImGui::GetContentRegionAvail().x;
        viewportHeight = ImGui::GetContentRegionAvail().y;

        auto image = renderer.getFinalImage();
        if (image)
            ImGui::Image((void *)(intptr_t)image->getDescriptor(), {(float)image->getWidth(), (float)image->getHeight()}, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }

    void render()
    {
        Timer timer;
        renderer.onResize(viewportWidth, viewportHeight);
        renderer.render();
        lastRenderTime = timer.getTimeElapsedMillis();
    }

private:
    Renderer renderer;
    uint32_t viewportWidth = 0,
             viewportHeight = 0;
    float lastRenderTime = 0;
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