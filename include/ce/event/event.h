#pragma once

namespace CrossEngine
{
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
    private:
        EventType event_type;
    protected:
        AEvent(EventType p_event_type);
    public:
        virtual ~AEvent();

        EventType GetEventType() const { return event_type; }
    };
}