#pragma once
#include <vector>
#include <memory>
#include <mutex>

class IEventListener;
class AEvent;
class EventManager
{
    std::vector<std::weak_ptr<IEventListener>> event_listeners;
    std::mutex event_listeners_mutex;

    void UpdateEventListeners();

public:
    /**
     * @brief Construct a new Event Manager object.
     * 
     */
    EventManager() {}
    /**
     * @brief Destroy the Event Manager object.
     * 
     */
    ~EventManager() {}

    /**
     * @brief Add an event listener.
     * 
     * @param p_event_listener The event listener to be added.
     */
    void AddEventListener(std::weak_ptr<IEventListener> p_event_listener);

    /**
     * @brief Remove an event listener.
     * 
     * @param p_event_listener The event listener to be removed.
     */
    void RemoveEventListener(std::weak_ptr<IEventListener> p_event_listener);

    /**
     * @brief Dispatch an event.
     * 
     * @param p_event The event to be dispatched.
     */
    void DispatchEvent(std::shared_ptr<AEvent> p_event);
};