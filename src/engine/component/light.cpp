#include "ce/component/light.h"
#include "ce/graphics/window.h"

namespace CrossEngine
{
    ALight::ALight()
        : Component()
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