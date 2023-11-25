#include "ce/component/light.h"

ALight::ALight(Window* p_context)
    : Component(p_context)
{

}

ALight::ALight(const Vec3& p_color, Window* p_context)
    : ALight(p_context)
{
    color = p_color;
}

ALight::~ALight()
{

}
