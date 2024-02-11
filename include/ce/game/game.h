#pragma once
#include "ce/math/math.hpp"
#include <memory>
#include <mutex>
#include <set>

namespace CrossEngine
{
    class Window;
    class EventManager;
    class InputManager;
    class AEvent;
    class Component;
    class Game
    {
    protected:
        std::shared_ptr<Window> main_window;
        std::shared_ptr<EventManager> event_manager;
        std::shared_ptr<InputManager> input_manager;

        static std::mutex initialize_mutex;
        Game();

        static Game* instance;
        std::shared_ptr<Component> base_component;
        std::set<Window*> available_contexts;
        std::mutex available_context_mutex;
    public:

        /**
         * @brief Initialize the game with the default window.
         * 
         */
        static void Init();

        /**
         * @brief Get the instance of the game.
         * 
         */
        static Game* GetInstance();

        /**
         * @brief Check if the game is initialized or not.
         * 
         * @return true The game is initialized.
         * @return false The game is not initialized.
         */
        static bool IsInitialized() { return instance != nullptr; }

        /**
         * @brief Set the main window of the game
         * 
         */
        void SetMainWindow(std::shared_ptr<Window> p_main_window);

        /**
         * @brief Set the main window of the game.
         * 
         * @param p_main_window The main window of the game.
         */
        void SetMainWindow(Window*&& p_main_window);

        /**
         * @brief Get the base component.
         * 
         * @return Component* The base component.
         */
        std::shared_ptr<Component> GetBaseComponent();

        /**
         * @brief Get the base component.
         *
         * @return const Component* The base component.
         */
        const std::shared_ptr<Component>& GetBaseComponent() const { return base_component; }

        /**
         * @brief Check if a context is available
         * 
         * @param p_context The context to check availability.
         * @return true The context is available.
         * @return false The context is not available.
         */
        bool IsContextAvailable(Window* p_context);

        /**
         * @brief Make a context available.
         * 
         * @param p_context The context to be set to available
         */
        void SetContextAvailable(Window* p_context);

        /**
         * @brief Make a context unavailable.
         * 
         * @param p_context The context to be set to unavailable
         */
        void SetContextUnAvailable(Window* p_context);

        /**
         * @brief Get the main window.
         * 
         * @return std::shared_ptr<Window> The main window.
         */
        FORCE_INLINE std::shared_ptr<Window> GetMainWindow() { return main_window; }

        /**
         * @brief Get the main window.
         * 
         * @return std::shared_ptr<Window> The main window.
         */
        FORCE_INLINE const std::shared_ptr<Window>& GetMainWindow() const { return main_window; }

        /**
         * @brief Destroy the Game object.
         */
        virtual ~Game();


        /**
         * @brief Get the input manager.
         * 
         * @return InputManager& The input manager.
         */
        FORCE_INLINE std::shared_ptr<InputManager> GetInputManager() { return input_manager; }

        /**
         * @brief Get the input manager.
         * 
         * @return InputManager& The input manager.
         */
        FORCE_INLINE const std::shared_ptr<InputManager> GetInputManager() const { return input_manager; }

        /**
         * @brief Get the event manager.
         * 
         * @return EventManager& The event manager.
         */
        FORCE_INLINE std::shared_ptr<EventManager> GetEventManager() { return event_manager; }

        /**
         * @brief Get the event manager.
         * 
         * @return EventManager& The event manager.
         */
        FORCE_INLINE const std::shared_ptr<EventManager> GetEventManager() const { return event_manager; }

        /**
         * @brief Update the input for the window.
         * 
         * @param p_context The context of the input.
         */
        void UpdateInput(Window* p_context);

        /**
         * @brief Dispatch an event.
         * 
         * @param p_event The event to be dispatched.
         */
        void DispatchEvent(std::shared_ptr<AEvent> p_event);

        /**
         * @brief Run the game.
         * 
         */
        void Run();

        /**
         * @brief Terminate the game.
         * 
         */
        void Terminate();

        /**
         * @brief Called once when the game is ready.
         * 
         */
        virtual void Ready() {};

        /**
         * @brief Called every frame.
         * 
         * @param p_delta The time in seconds since the last frame.
         */
        virtual void Process(float p_delta) {};
    };
}