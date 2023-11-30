#pragma once
#include <memory>

class AEvent;
class IEventListener
{
public:
    virtual void OnEvent(std::shared_ptr<AEvent> p_event) = 0;
};