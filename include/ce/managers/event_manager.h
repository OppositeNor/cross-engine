#pragma once
#include <vector>
#include <memory>
#include <shared_mutex>

namespace CrossEngine
{
    class IEventListener;
    class AEvent;
    class EventManager
    {
    private:
        std::vector<std::weak_ptr<IEventListener>> event_listeners;
        std::shared_mutex event_listeners_mutex;

        std::vector<std::shared_ptr<AEvent>> events;
        std::shared_mutex events_mutex;

        void UpdateEventListeners();

        void FEventsDispatch();

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
         * @brief Register a event to the event system.
         * 
         * @param p_event The event to be registered.
         */
        void RegisterEvent(std::shared_ptr<AEvent> p_event);

        /**
         * @brief Dispatch the events to the event listeners.
         * 
         */
        void DispatchEvents();
    };
}