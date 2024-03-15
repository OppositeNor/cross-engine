#include "ce/managers/event_manager.h"
#include "ce/event/i_event_listener.h"
#include <future>
#include <iostream>

namespace CrossEngine
{
    void EventManager::AddEventListener(std::weak_ptr<IEventListener> p_event_listener)
    {
        std::unique_lock<std::shared_mutex> lock(event_listeners_mutex);
        if (p_event_listener.expired())
            return;
        for (auto& event_listener : event_listeners)
        {
            if (event_listener.expired())
                continue;
            if (event_listener.lock().get() == p_event_listener.lock().get())
                return;
        }
        event_listeners.push_back(p_event_listener);
    }

    void EventManager::RemoveEventListener(std::weak_ptr<IEventListener> p_event_listener)
    {
        std::unique_lock<std::shared_mutex> lock(event_listeners_mutex);
        for (auto it = event_listeners.begin(); it != event_listeners.end(); ++it)
        {
            if ((*it).lock().get() == p_event_listener.lock().get())
            {
                event_listeners.erase(it);
                return;
            }
        }
    }

    void EventManager::RegisterEvent(std::shared_ptr<AEvent> p_event)
    {
        std::lock_guard lock(events_mutex);
        events.push_back(p_event);
    }

    void EventManager::DispatchEvents()
    {
        auto future = std::async(std::launch::async, &EventManager::FEventsDispatch, this);
    }

    void EventManager::UpdateEventListeners()
    {
        for (auto it = event_listeners.begin(); it != event_listeners.end();)
        {
            if ((*it).expired())
                it = event_listeners.erase(it);
            else
                ++it;
        }
    }

    void EventManager::FEventsDispatch()
    {
        std::shared_lock<std::shared_mutex> lock(event_listeners_mutex);
        std::shared_lock<std::shared_mutex> lock2(events_mutex);
        for (auto& event_listener : event_listeners)
        {
            if (event_listener.expired())
                continue;
            for (auto& event : events)
                event_listener.lock()->OnEvent(event);
        }
        events.clear();
    }
}