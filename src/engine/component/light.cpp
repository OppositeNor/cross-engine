#include "ce/component/light.h"
#include "ce/graphics/window.h"

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
    {
        Component::Draw(p_context);
    }
}