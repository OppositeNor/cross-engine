#include "ce/game/game.h"
#include "ce/graphics/window.h"
#include "ce/managers/input_manager.h"
#include "ce/managers/event_manager.h"
#include "ce/graphics/graphics.h"
#include "ce/component/component.h"
#include <GLFW/glfw3.h>

namespace CrossEngine
{
    Game* Game::instance = nullptr;
    std::mutex Game::initialize_mutex;

    Game::Game(std::shared_ptr<Window> p_window)
    {
        main_window = p_window;
        input_manager = std::make_shared<InputManager>();
        event_manager = std::make_shared<EventManager>();
        base_component = std::make_shared<Component>();

        event_manager->AddEventListener(input_manager);
    }

    Game::Game(Window*&& p_window)
        : Game(std::shared_ptr<Window>(p_window))
    {
    }

    void Game::Init(std::shared_ptr<Window> p_window)
    {
        if (instance == nullptr)
        {
            std::lock_guard<std::mutex> lock(initialize_mutex);
            if (instance == nullptr)
            {
                instance = new Game(p_window);
            }
        }
    }

    void Game::Init(Window*&& p_window)
    {
        if (instance == nullptr)
        {
            std::lock_guard<std::mutex> lock(initialize_mutex);
            if (instance == nullptr)
                instance = new Game(std::move(p_window));
        }
    }

    void Game::Init(const Math::Vec2s& p_size, const std::string& p_title)
    {
        Init(std::make_shared<Window>(p_size, p_title));
    }

    void Game::Init(size_t p_width, size_t p_height, const std::string& p_title)
    {
        Init(std::make_shared<Window>(p_width, p_height, p_title));
    }

    std::shared_ptr<Component> Game::GetBaseComponent()
    {
        return base_component;
    }

    void Game::Init()
    {
        Init(std::make_shared<Window>());
    }

    Game* Game::GetInstance()
    {
        if (instance == nullptr)
            throw std::runtime_error("Game is not initialized.");
        return instance;
    }

    bool Game::IsContextAvailable(Window* p_context)
    {
        std::lock_guard<std::mutex> lock(available_context_mutex);
        for (auto i : available_contexts)
        {
            if (i == p_context)
                return true;
        }
        return false;
    }

    void Game::SetContextAvailable(Window* p_context)
    {
        std::lock_guard<std::mutex> lock(available_context_mutex);
        available_contexts.insert(p_context);
    }

    void Game::SetContextUnAvailable(Window* p_context)
    {
        std::lock_guard<std::mutex> lock(available_context_mutex);
        available_contexts.erase(p_context);
    }

    Game::~Game()
    {
        main_window.reset();
        Graphics::TerminateGraphics();
    }

    void Game::UpdateInput(Window* p_context)
    {
        input_manager->UpdateInput(p_context);
    }

    void Game::DispatchEvent(std::shared_ptr<AEvent> p_event)
    {
        event_manager->DispatchEvent(p_event);
    }

    void Game::Run()
    {
        float frame_start;
        float delta = 0.01;
        base_component->Ready();
        while (!main_window->IsClosed())
        {
            frame_start = (float)glfwGetTime();
            Process(delta);
            base_component->Update(delta);
            Graphics::Update();
            Sleep(1);
            delta = (float)glfwGetTime() - frame_start;
        }
        Game::GetInstance()->Terminate();
    }

    void Game::Terminate()
    {
        if (instance != nullptr)
        {
            std::lock_guard<std::mutex> lock(initialize_mutex);
            if (instance != nullptr)
            {
                delete instance;
                instance = nullptr;
            }
        }
    }
}