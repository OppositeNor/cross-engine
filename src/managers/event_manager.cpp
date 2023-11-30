#include "ce/managers/event_manager.h"
#include "ce/event/i_event_listener.h"

void EventManager::AddEventListener(std::weak_ptr<IEventListener> p_event_listener)
{
    std::lock_guard<std::mutex> lock(event_listeners_mutex);
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
    std::lock_guard<std::mutex> lock(event_listeners_mutex);
    for (auto it = event_listeners.begin(); it != event_listeners.end(); ++it)
    {
        if ((*it).lock().get() == p_event_listener.lock().get())
        {
            event_listeners.erase(it);
            return;
        }
    }
}

void EventManager::DispatchEvent(std::shared_ptr<AEvent> p_event)
{
    UpdateEventListeners();
    std::lock_guard<std::mutex> lock(event_listeners_mutex);
    for (auto& event_listener : event_listeners)
    {
        if (event_listener.expired())
            continue;
        event_listener.lock()->OnEvent(p_event);
    }
}

void EventManager::UpdateEventListeners()
{
    std::lock_guard<std::mutex> lock(event_listeners_mutex);
    for (auto it = event_listeners.begin(); it != event_listeners.end();)
    {
        if ((*it).expired())
            it = event_listeners.erase(it);
        else
            ++it;
    }
}