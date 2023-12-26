extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

#include "ce/geometry/triangle.h"
#include "ce/graphics/window.h"
#include "ce/graphics/graphics.h"
#include "ce/resource/resource.h"
#include "ce/component/dynamic_mesh.h"
#include "ce/component/static_mesh.h"
#include "ce/component/camera.h"
#include "ce/managers/input_manager.h"
#include "ce/managers/event_manager.h"
#include "ce/game/game.h"
#include "ce/materials/valued_material.h"
#include "ce/texture/static_texture.h"
#include "ce/component/parallel_light.h"
#include "ce/event/window_event.h"
#include <cmath>
#include <sstream>
Math::Vector<bool, 3> rotate;

class UserCamera : public Camera, public IEventListener
{
public:
    UserCamera(Window* p_context)
        : Camera(p_context)
    {}


    Math::Vector<double, 2> new_pos;
    Math::Vector<double, 2> last_pos;
    virtual void Process(float p_delta) override
    {
        auto context = GetContext();
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "forward") == InputManager::InputState::Pressed)
            Position() += GetDirection() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "backward") == InputManager::InputState::Pressed)
            Position() += -1 * GetDirection() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "left") == InputManager::InputState::Pressed)
            Position() +=Math::Cross(Math::UP<4>, GetDirection()).Normalized() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "right") == InputManager::InputState::Pressed)
            Position() +=Math::Cross(GetDirection(), Math::UP<4>).Normalized() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "up") == InputManager::InputState::Pressed)
            Position() +=Math::UP<4> * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "down") == InputManager::InputState::Pressed)
            Position() += -1 *Math::UP<4> * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "rotate_x") == InputManager::InputState::Pressed)
        {
            if (GetDirection().Dot(Math::UP<4>) < 0.99)
                Rotate(GetRight(), p_delta * 1.5);
        }
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "rotate_-x") == InputManager::InputState::Pressed)
        {
            if (GetDirection().Dot(Math::UP<4>) > -0.99)
                Rotate(Math::Cross(Math::UP<4>, GetDirection()), p_delta * 1.5);
        }
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "rotate_y") == InputManager::InputState::Pressed)
            Rotate(Math::UP<4>, p_delta * 1.5);
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "rotate_-y") == InputManager::InputState::Pressed)
            Rotate(-1 *Math::UP<4>, p_delta * 1.5);
        auto delta = new_pos - last_pos;
        last_pos = Math::Lerp(0.7, last_pos, new_pos);
        Rotate(Math::UP<4>, -delta[0] * 0.0008);
        Rotate(GetRight(), -delta[1] * 0.0008);
    }

    virtual void OnEvent(std::shared_ptr<AEvent> p_event) override 
    {
        if (p_event->GetEventType() == EventType::OnMouseMove)
        {
            auto event = std::dynamic_pointer_cast<OnMouseMoveEvent>(p_event);
            auto context = GetContext();
            if (event->window == context)
            {
                new_pos = event->pos;
            }
        }
    }
};

class UserWindow : public Window
{
    std::shared_ptr<UserWindow> window2;
    std::shared_ptr<VisualMesh> mesh;
    std::shared_ptr<DynamicMesh> box;
    std::shared_ptr<DynamicMesh> floor;
    std::shared_ptr<PointLight> light;
    std::shared_ptr<ParallelLight> parallel_light;
    std::shared_ptr<UserCamera> camera;
    std::string title;

    
public:
    virtual void OnEvent(std::shared_ptr<AEvent> p_event) override {
    }

    UserWindow(const std::string& p_title)
        : Window(Math::Vec2s(1260, 720), p_title, false, false)
    {
        title = p_title;
    }

    virtual void Ready() override
    {
        if (title == "")
        {
            window2 = std::make_shared<UserWindow>("Window 2");
            Game::GetInstance()->GetEventManager()->AddEventListener(window2);
        }
        else
            window2 = nullptr;
        
        mesh = std::make_shared<StaticMesh>(this);
        if (title == "")
        {
            mesh->LoadTriangles(Resource::LoadModel(Resource::GetExeDirectory() + "/monkey.obj"));
            mesh->Position() =Math::Vec4(0, 1.5, 0, 1);
        }
        else
        {
            mesh->LoadTriangles(Resource::LoadModel(Resource::GetExeDirectory() + "/jet.obj"));
            mesh->Position() =Math::Vec4(0, 0, 0, 1);
            HideAndLockCursor();
        }
        mesh->Scale() =Math::Vec4(1.5, 1.5, 1.5);
        GetBaseComponent()->AddChild(mesh);
        auto mesh_material = std::make_shared<PBRMaterial>(Math::Vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.95f, 1.0f, this);
        mesh_material->Albedo() = std::make_shared<StaticTexture>(this, Resource::GetExeDirectory() + 
            "/textures/monkey_albedo.png");
        mesh_material->Roughness() = std::make_shared<StaticTexture>(this, Resource::GetExeDirectory() +
            "/textures/monkey_roughness.png");
        mesh_material->AO() = std::make_shared<StaticTexture>(this, Resource::GetExeDirectory() +
            "/textures/monkey_ao.png");
        
        mesh->SetMaterial(mesh_material);
        mesh->SetVisible(true);
        
        light = std::make_shared<PointLight>(Math::Vec4(1.0f, 1.0f, 1.0f, 1.0f), 400, this);
        light->Position() =Math::Vec4(0.0f, 10.0f, 10.0f, 1.0f);

        //parallel_light = std::make_shared<ParallelLight>(Math::Vec4(1.0f, -1.0f, 1.0f), Math::Vec4(1, 1, 1, 1), 10, this);
        parallel_light = std::make_shared<ParallelLight>(this);
        parallel_light->Direction() = Math::Vec4(1, -1, 1);
        parallel_light->Intensity() = 8;
        
        camera = std::make_shared<UserCamera>(this);
        camera->Position() =Math::Vec4(0, 0, -20, 1.0f);
        GetBaseComponent()->AddChild(camera);
        SetUsingCamera(camera);

        box = std::make_shared<DynamicMesh>(this);
        // Box mesh clock wise
        //front face
        box->Triangles().push_back(new Triangle(Math::Vec4(1, -1, 1, 1), Math::Vec4(-1, -1, 1, 1), Math::Vec4(1, 1, 1, 1)));
        box->Triangles().push_back(new Triangle(Math::Vec4(1, 1, 1, 1), Math::Vec4(-1, -1, 1, 1), Math::Vec4(-1, 1, 1, 1)));
        //back face
        box->Triangles().push_back(new Triangle(Math::Vec4(-1, -1, -1, 1), Math::Vec4(1, -1, -1, 1), Math::Vec4(1, 1, -1, 1)));
        box->Triangles().push_back(new Triangle(Math::Vec4(-1, -1, -1, 1), Math::Vec4(1, 1, -1, 1), Math::Vec4(-1, 1, -1, 1)));
        //left face
        box->Triangles().push_back(new Triangle(Math::Vec4(-1, -1, 1, 1), Math::Vec4(-1, -1, -1, 1), Math::Vec4(-1, 1, 1, 1)));
        box->Triangles().push_back(new Triangle(Math::Vec4(-1, 1, 1, 1), Math::Vec4(-1, -1, -1, 1), Math::Vec4(-1, 1, -1, 1)));
        //right face
        box->Triangles().push_back(new Triangle(Math::Vec4(1, -1, -1, 1), Math::Vec4(1, -1, 1, 1), Math::Vec4(1, 1, 1, 1)));
        box->Triangles().push_back(new Triangle(Math::Vec4(1, -1, -1, 1), Math::Vec4(1, 1, 1, 1), Math::Vec4(1, 1, -1, 1)));
        //top face
        box->Triangles().push_back(new Triangle(Math::Vec4(-1, 1, 1, 1), Math::Vec4(-1, 1, -1, 1), Math::Vec4(1, 1, 1, 1)));
        box->Triangles().push_back(new Triangle(Math::Vec4(1, 1, 1, 1), Math::Vec4(-1, 1, -1, 1), Math::Vec4(1, 1, -1, 1)));
        //bottom face
        box->Triangles().push_back(new Triangle(Math::Vec4(-1, -1, -1, 1), Math::Vec4(-1, -1, 1, 1), Math::Vec4(1, -1, 1, 1)));
        box->Triangles().push_back(new Triangle(Math::Vec4(-1, -1, -1, 1), Math::Vec4(1, -1, 1, 1), Math::Vec4(1, -1, -1, 1)));


        light->AddChild(box);

        floor = std::make_shared<DynamicMesh>(this);
        
        floor->Triangles().push_back(new Triangle(Math::Vec4(-1, 0, -1, 1), Math::Vec4(-1, 0, 1, 1), Math::Vec4(1, 0, 1, 1)));
        floor->Triangles().push_back(new Triangle(Math::Vec4(-1, 0, -1, 1), Math::Vec4(1, 0, 1, 1), Math::Vec4(1, 0, -1, 1)));
        floor->Triangles()[0]->GetVertex(0)->UV() =Math::Vec2(0, 10);
        floor->Triangles()[0]->GetVertex(1)->UV() =Math::Vec2(0, 0);
        floor->Triangles()[0]->GetVertex(2)->UV() =Math::Vec2(10, 0);
        floor->Triangles()[1]->GetVertex(0)->UV() =Math::Vec2(0, 10);
        floor->Triangles()[1]->GetVertex(1)->UV() =Math::Vec2(10, 0);
        floor->Triangles()[1]->GetVertex(2)->UV() =Math::Vec2(10, 10);
        GetBaseComponent()->AddChild(floor);

        box->Scale() =Math::Vec4(1.5f, 1.5f, 1.5f);
        floor->Position() =Math::Vec4(0, 0, 0, 1.0f);
        floor->Scale() =Math::Vec4(50, 1, 50);

        auto floor_material = std::make_shared<PBRMaterial>(Math::Vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.8f, 1.0f, this);
        floor_material->Albedo() = std::make_shared<StaticTexture>(this, Resource::GetExeDirectory() + 
            "/textures/floor_albedo.png");
        floor_material->Normal() = std::make_shared<StaticTexture>(this, Resource::GetExeDirectory() +
            "/textures/floor_normal.png");
        floor_material->Roughness() = std::make_shared<StaticTexture>(this, Resource::GetExeDirectory() +
            "/textures/floor_roughness.png");
        floor_material->AO() = std::make_shared<StaticTexture>(this, Resource::GetExeDirectory() +
            "/textures/floor_ao.png");
        floor->SetMaterial(floor_material);
        floor->SetVisible(false);
        light->AddChild(box);
        GetBaseComponent()->AddChild(light);
        GetBaseComponent()->AddChild(parallel_light);

        Game::GetInstance()->GetEventManager()->AddEventListener(camera);
    }

    virtual void Process(float p_delta) override
    {
        auto input_manager = Game::GetInstance()->GetInputManager();
        if (input_manager->GetInputState(this, "light_move_left") == InputManager::InputState::Pressed)
            light->Position() +=Math::Cross(Math::UP<4>, GetUsingCamera()->GetDirection()).Normalize() * p_delta * 10;
        if (input_manager->GetInputState(this, "light_move_right") == InputManager::InputState::Pressed)
            light->Position() +=Math::Cross(GetUsingCamera()->GetDirection(), Math::UP<4>).Normalize() * p_delta * 10;
        if (input_manager->GetInputState(this, "light_move_up") == InputManager::InputState::Pressed)
            light->Position() +=Math::UP<4> * p_delta * 10;
        if (input_manager->GetInputState(this, "light_move_down") == InputManager::InputState::Pressed)
            light->Position() -=Math::UP<4> * p_delta * 10;
        if (input_manager->GetInputState(this, "light_move_forward") == InputManager::InputState::Pressed)
            light->Position() +=Math::Cross(Math::UP<4>, Math::Cross(GetUsingCamera()->GetDirection(), Math::UP<4>)).Normalize() * p_delta * 10;
        if (input_manager->GetInputState(this, "light_move_backward") == InputManager::InputState::Pressed)
            light->Position() +=Math::Cross(Math::Cross(GetUsingCamera()->GetDirection(), Math::UP<4>), Math::UP<4>).Normalize() * p_delta * 10;
        if (input_manager->GetInputState(this, "esc") == InputManager::InputState::JustPressed)
            ShowAndUnlockCursor();
        if (input_manager->GetInputState(this, "lb") == InputManager::InputState::JustPressed)
            HideAndLockCursor();
        if (window2 != nullptr && window2->IsClosed())
        {
            window2.reset();
            window2 = nullptr;
        }
    }

    virtual void Draw() override
    {
        Window::Draw();
    }

    virtual void OnClose() override
    {
        if (window2 != nullptr)
        {
            window2->Close();
        }
    }
};


#ifdef WIN32_MAIN
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
#else
int main()
#endif
{
    try
    {
        Game::Init(std::make_shared<UserWindow>(""));

        Game::GetInstance()->GetInputManager()->AddInput("forward", Input::KEY_W);
        Game::GetInstance()->GetInputManager()->AddInput("backward", Input::KEY_S);
        Game::GetInstance()->GetInputManager()->AddInput("left", Input::KEY_A);
        Game::GetInstance()->GetInputManager()->AddInput("right", Input::KEY_D);
        Game::GetInstance()->GetInputManager()->AddInput("down", Input::KEY_Q);
        Game::GetInstance()->GetInputManager()->AddInput("up", Input::KEY_E);

        Game::GetInstance()->GetInputManager()->AddInput("rotate_x", Input::KEY_UP);
        Game::GetInstance()->GetInputManager()->AddInput("rotate_-x", Input::KEY_DOWN);
        Game::GetInstance()->GetInputManager()->AddInput("rotate_y", Input::KEY_LEFT);
        Game::GetInstance()->GetInputManager()->AddInput("rotate_-y", Input::KEY_RIGHT);

        Game::GetInstance()->GetInputManager()->AddInput("light_move_left", Input::KEY_J);
        Game::GetInstance()->GetInputManager()->AddInput("light_move_right", Input::KEY_L);
        Game::GetInstance()->GetInputManager()->AddInput("light_move_up", Input::KEY_O);
        Game::GetInstance()->GetInputManager()->AddInput("light_move_down", Input::KEY_U);
        Game::GetInstance()->GetInputManager()->AddInput("light_move_forward", Input::KEY_I);
        Game::GetInstance()->GetInputManager()->AddInput("light_move_backward", Input::KEY_K);
    
        Game::GetInstance()->GetInputManager()->AddInput("esc", Input::KEY_ESCAPE);
        Game::GetInstance()->GetInputManager()->AddInput("lb", Input::MOUSE_BUTTON_LEFT);

        Game::GetInstance()->Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}