#pragma once
#include "ce/defs.hpp"
#include "ce/event/i_event_listener.h"
#include <map>
#include <vector>
class AEvent;
class Window;
/**
 * @brief The input manager class.
 * @tparam KeyType The type of the event key type
 */
class InputManager
    : public IEventListener, public std::enable_shared_from_this<InputManager>
{
public:
    enum InputState
    {
        /**
         * @brief Is the key released.
         */
        Released = 0b0010,
        /**
         * @brief Is the key just released.
         * This is equivalent to (InputState::Just & InputState::Released)
         */
        JustReleased = 0b0011,
        /**
         * @brief Is the key pressed.
         */
        Pressed = 0b0100,
        /**
         * @brief Is the key just pressed.
         */
        JustPressed = 0b0101,

        /**
         * @brief Is the state happened just now. You can combine this with other states
         * by using bitwise or.
         * @example
         * @code
         * if (GetInputState("key_example") & InputState::Just)
         * {// the key just changed its state
         * ...}
         * if (GetInputState("key_example") == (InputState::Just & InputState::Pressed)   
         * {// the key just pressed. This is equivalent to InputState::JustPressed
         * ...}
         * @endcode
         */
        Just = 0b0001
    };
private:

    using InputStateMap = std::map<std::string, InputState>;
    std::map<int, std::vector<std::string>> input_map;
    std::map <const Window*, InputStateMap> context_map;

    /**
     * @brief Set the key state to pressed.
     * If the key code is not in the input map, this function will do nothing.
     * 
     * @param p_context The context of the input.
     * @param p_key The key code.
     */
    void SetInputStatePressed(Window* p_context, int p_key);

    /**
     * @brief Set the key state to released.
     * If the key code is not in the input map, this function will do nothing.
     * 
     * @param p_context The context of the input.
     * @param p_key The key code.
     */
    void SetInputStateReleased(Window* p_context, int p_key);
public:

    /**
     * @brief Construct a new Input Manager object
     * 
     */
    InputManager();
    
    /**
     * @brief Get the input state of a key.
     * 
     * @param p_context The context of the input.
     * @param p_context The context of the input.
     * @param p_key The key to get the state.
     * @return InputState The state of the key.
     */
    InputState GetInputState(const Window* p_context, const std::string& p_key) const;

    /**
     * @brief Add a input to the input manager.
     * 
     * @param input_name The name of the input.
     * @param p_key The key code of the input.
     */
    void AddInput(const std::string& input_name, int p_key);

    /**
     * @brief Clear all the input.
     */
    void ClearInput();

    /**
     * @brief Update the input state.
     * 
     * @param p_context The context of the input.
     */
    void UpdateInput(const Window* p_context);

    /**
     * @brief Is the input pressed.
     * 
     * @param p_context The context of the input.
     * @param p_key The name of the input.
     * @return true The input is pressed.
     * @return false The input is not pressed.
     */
    bool IsInputPressed(const Window* p_context, const std::string& p_key) const;

    /**
     * @brief Is the input released.
     * 
     * @param p_context The context of the input.
     * @param p_key The name of the input.
     * @return true The input is released.
     * @return false The input is not released.
     */
    bool IsInputReleased(const Window* p_context, const std::string& p_key) const;

    /**
     * @brief Is the input just pressed.
     * 
     * @param p_context The context of the input.
     * @param p_key The name of the input.
     * @return true The input is just pressed.
     * @return false The input is not just pressed.
     */
    bool IsInputJustPressed(const Window* p_context, const std::string& p_key) const;

    /**
     * @brief Is the input just released.
     * 
     * @param p_context The context of the input.
     * @param p_key The name of the input.
     * @return true The input is just released.
     * @return false The input is not just released.
     */
    bool IsInputJustReleased(const Window* p_context, const std::string& p_key) const;
    
    virtual void OnEvent(std::shared_ptr<AEvent> p_event) override;
};