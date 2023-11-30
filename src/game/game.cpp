#include "ce/game/game.h"
#include "ce/graphics/window.h"
#include "ce/managers/input_manager.h"
#include "ce/graphics/graphics.h"
#include <GLFW/glfw3.h>

Game* Game::instance = nullptr;

Game::Game(Window*&& p_window)
{
    main_window = std::unique_ptr<Window>(p_window);
    p_window = nullptr;
    input_manager = std::unique_ptr<InputManager>(new InputManager());
    main_window->AddWindowEventListener(input_manager.get());
}

void Game::Init(Window*&& p_window)
{
    if (instance == nullptr)
    {
        instance = new Game(std::move(p_window));
    }
}

void Game::Init(const Vec2s& p_size, const std::string& p_title)
{
    Init(new Window(p_size, p_title));
}

void Game::Init(size_t p_width, size_t p_height, const std::string& p_title)
{
    Init(new Window(p_width, p_height, p_title));
}

void Game::Init()
{
    Init(new Window());
}

Game* Game::GetInstance()
{
    if (instance == nullptr)
        throw std::runtime_error("Game instance is null");
    return instance;
}

Game::~Game()
{
    Graphics::TerminateGraphics();
}

void Game::Run()
{
    float frame_start;
    float delta = 0.01;
    while (!main_window->IsClosed())
    {
        frame_start = (float)glfwGetTime();
        Process(delta);
        delta = (float)glfwGetTime() - frame_start;
    }
}