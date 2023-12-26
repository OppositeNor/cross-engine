#pragma once

enum class EventType
{
    OnWindowClose,
    OnWindowFocus,
    OnWindowResize,
    OnWindowMove,
    OnKey,
    OnMouseMove
};

class AEvent
{
    EventType event_type;
protected:
    AEvent(EventType p_event_type);
public:
    virtual ~AEvent();

    EventType GetEventType() const { return event_type; }
};