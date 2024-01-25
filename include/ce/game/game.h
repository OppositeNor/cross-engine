#pragma once
#include "ce/math/math.hpp"
#include <memory>
#include <mutex>

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
    Game(Window*&& p_window);
    Game(std::shared_ptr<Window> p_window);

    static Game* instance;
    std::shared_ptr<Component> base_component;

public:


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
     * @brief Initialize the game with the default window.
     * 
     */
    static void Init();
    
    /**
     * @brief Initialize the game.
     * 
     * @param p_window The window to use.
     */
    static void Init(std::shared_ptr<Window> p_window);

    /**
     * @brief Initialize the game.
     * 
     * @param p_window The window to use.
     */
    static void Init(Window*&& p_window);

    /**
     * @brief Initialize the game.
     * 
     * @param p_size The size of the main window.
     * @param p_title The title of the main window.
     */
    static void Init(const Math::Vec2s& p_size, const std::string& p_title = "");

    /**
     * @brief Initialize the game.
     * 
     * @param p_width The width of the main window.
     * @param p_height The height of the main window.
     * @param p_title The title of the main window.
     */
    static void Init(size_t p_width, size_t p_height, const std::string& p_title = "");

    /**
     * @brief Get the instance of the game.
     * 
     */
    static Game* GetInstance();

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