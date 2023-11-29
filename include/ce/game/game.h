#pragma once
#include "ce/math/math.hpp"
#include <memory>

class Window;
class InputManager;
class Game
{
protected:
    std::unique_ptr<Window> main_window;
    std::unique_ptr<InputManager> input_manager;
    Game(Window*&& p_window);

    static Game* instance;

public:

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
    static void Init(Window*&& p_window);

    /**
     * @brief Initialize the game.
     * 
     * @param p_size The size of the main window.
     * @param p_title The title of the main window.
     */
    static void Init(const Vec2s& p_size, const std::string& p_title = "");

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
     * @brief Destroy the Game object.
     */
    virtual ~Game();


    /**
     * @brief Get the input handler.
     * 
     * @return InputManager& The input handler.
     */
    FORCE_INLINE InputManager* GetInputManager() { return input_manager.get(); }

    /**
     * @brief Get the input handler.
     * 
     * @return InputManager& The input handler.
     */
    FORCE_INLINE const InputManager* GetInputManager() const { return input_manager.get(); }

    /**
     * @brief Run the game.
     * 
     */
    void Run();

    /**
     * @brief Called every frame.
     * 
     * @param p_delta The time in seconds since the last frame.
     */
    virtual void Process(float p_delta) {};
};