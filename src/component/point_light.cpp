#include "ce/component/point_light.h"
#include <type_traits>

PointLight::PointLight(Window* p_context)
    : ALight(p_context)
{
}

void* PointLight::GetData()
{
    data.position[0] = static_cast<const Component*>(this)->GetPosition()[0];
    data.position[1] = static_cast<const Component*>(this)->GetPosition()[1];
    data.position[2] = static_cast<const Component*>(this)->GetPosition()[2];
    data.color[0] = color[0];
    data.color[1] = color[1];
    data.color[2] = color[2];
    return &data;
}