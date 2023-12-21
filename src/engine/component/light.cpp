#include "ce/component/light.h"
#include "ce/graphics/window.h"

ALight::ALight(Window* p_context)
    : Component(p_context)
{

}

ALight::~ALight()
{

}

void ALight::Draw()
{
    Component::Draw();
    SetUniform(GetContext()->GetPointLightNextIndex());
}