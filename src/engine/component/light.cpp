#include "ce/component/light.h"
#include "ce/graphics/window.h"
#include "ce/graphics/renderer/renderer.h"

namespace CrossEngine
{
    ALight::ALight(const std::string& p_component_name)
        : Component3D(p_component_name)
    {

    }

    ALight::~ALight()
    {

    }

    void ALight::Draw(Window* p_context)
    {}

    bool ALight::RegisterDraw(Window* p_context)
    {
        if (Component3D::RegisterDraw(p_context))
        {
            p_context->GetRenderer()->AddRenderTask(Task([this, p_context](){Draw(p_context);}, 0));
            return true;
        }
        return false;
    }
}