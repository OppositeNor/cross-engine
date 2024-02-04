#pragma once
#include <memory>

namespace CrossEngine
{
    class AEvent;
    class IEventListener
    {
    public:
        virtual void OnEvent(std::shared_ptr<AEvent> p_event) = 0;
    };
}