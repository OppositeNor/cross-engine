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
#include <cmath>
Vector<bool, 3> rotate;

class UserCamera : public Camera
{
public:
    UserCamera(Window* p_context)
        : Camera(p_context)
    {}

    virtual void Process(float p_delta) override
    {
        auto context = GetContext();
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "forward") == InputManager::InputState::Pressed)
            Position() += GetDirection() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "backward") == InputManager::InputState::Pressed)
            Position() += -1 * GetDirection() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "left") == InputManager::InputState::Pressed)
            Position() += Vec4::Cross(Vec4::UP, GetDirection()).Normalized() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "right") == InputManager::InputState::Pressed)
            Position() += Vec4::Cross(GetDirection(), Vec4::UP).Normalized() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "up") == InputManager::InputState::Pressed)
            Position() += Vec4::UP * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "down") == InputManager::InputState::Pressed)
            Position() += -1 * Vec4::UP * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "rotate_x") == InputManager::InputState::Pressed)
        {
            if (GetDirection().Dot(Vec4::UP) < 0.99)
                Rotate(Vec4::Cross(GetDirection(), Vec4::UP), p_delta * 1.5);
        }
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "rotate_-x") == InputManager::InputState::Pressed)
        {
            if (GetDirection().Dot(Vec4::UP) > -0.99)
                Rotate(Vec4::Cross(Vec4::UP, GetDirection()), p_delta * 1.5);
        }
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "rotate_y") == InputManager::InputState::Pressed)
            Rotate(Vec4::UP, p_delta * 1.5);
        if (Game::GetInstance()->GetInputManager()->GetInputState(GetContext(), "rotate_-y") == InputManager::InputState::Pressed)
            Rotate(-1 * Vec4::UP, p_delta * 1.5);
    }
};

class UserWindow : public Window
{
    std::shared_ptr<UserWindow> window2;
    std::shared_ptr<VisualMesh> mesh;
    std::shared_ptr<DynamicMesh> box;
    std::shared_ptr<DynamicMesh> floor;
    std::shared_ptr<PointLight> light;
    std::string title;

    
public:
    virtual void OnEvent(std::shared_ptr<AEvent> p_event) override {
    }

    UserWindow(const std::string& p_title)
        : Window(Vec2s(1260, 720), p_title, false, false)
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
            mesh->Position() = Vec4(0, 1.5, 0, 1);
        }
        else
        {
            mesh->LoadTriangles(Resource::LoadModel(Resource::GetExeDirectory() + "/jet.obj"));
            mesh->Position() = Vec4(0, 0, 0, 1);
        }
        mesh->Scale() = Vec4(1.5, 1.5, 1.5);
        GetBaseComponent()->AddChild(mesh);
        auto mesh_material = std::make_shared<PBRMaterial>(Vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.95f, 1.0f, this);
        mesh_material->Albedo() = std::make_shared<StaticTexture>(this, Resource::GetExeDirectory() + 
            "/textures/monkey_albedo.png");
        mesh_material->Roughness() = std::make_shared<StaticTexture>(this, Resource::GetExeDirectory() +
            "/textures/monkey_roughness.png");
        mesh_material->AO() = std::make_shared<StaticTexture>(this, Resource::GetExeDirectory() +
            "/textures/monkey_ao.png");
        
        mesh->SetMaterial(mesh_material);
        mesh->SetVisible(true);
        
        light = std::make_shared<PointLight>(Vec4(1.0f, 1.0f, 1.0f, 1.0f), 400, this);
        light->Position() = Vec4(0.0f, 10.0f, 10.0f, 1.0f);
        auto camera = std::make_shared<UserCamera>(this);
        camera->Position() = Vec4(0, 0, -20, 1.0f);
        GetBaseComponent()->AddChild(camera);
        SetUsingCamera(camera);

        box = std::make_shared<DynamicMesh>(this);
        // Box mesh clock wise
        //front face
        box->Triangles().push_back(new Triangle(Vec4(1, -1, 1, 1), Vec4(-1, -1, 1, 1), Vec4(1, 1, 1, 1)));
        box->Triangles().push_back(new Triangle(Vec4(1, 1, 1, 1), Vec4(-1, -1, 1, 1), Vec4(-1, 1, 1, 1)));
        //back face
        box->Triangles().push_back(new Triangle(Vec4(-1, -1, -1, 1), Vec4(1, -1, -1, 1), Vec4(1, 1, -1, 1)));
        box->Triangles().push_back(new Triangle(Vec4(-1, -1, -1, 1), Vec4(1, 1, -1, 1), Vec4(-1, 1, -1, 1)));
        //left face
        box->Triangles().push_back(new Triangle(Vec4(-1, -1, 1, 1), Vec4(-1, -1, -1, 1), Vec4(-1, 1, 1, 1)));
        box->Triangles().push_back(new Triangle(Vec4(-1, 1, 1, 1), Vec4(-1, -1, -1, 1), Vec4(-1, 1, -1, 1)));
        //right face
        box->Triangles().push_back(new Triangle(Vec4(1, -1, -1, 1), Vec4(1, -1, 1, 1), Vec4(1, 1, 1, 1)));
        box->Triangles().push_back(new Triangle(Vec4(1, -1, -1, 1), Vec4(1, 1, 1, 1), Vec4(1, 1, -1, 1)));
        //top face
        box->Triangles().push_back(new Triangle(Vec4(-1, 1, 1, 1), Vec4(-1, 1, -1, 1), Vec4(1, 1, 1, 1)));
        box->Triangles().push_back(new Triangle(Vec4(1, 1, 1, 1), Vec4(-1, 1, -1, 1), Vec4(1, 1, -1, 1)));
        //bottom face
        box->Triangles().push_back(new Triangle(Vec4(-1, -1, -1, 1), Vec4(-1, -1, 1, 1), Vec4(1, -1, 1, 1)));
        box->Triangles().push_back(new Triangle(Vec4(-1, -1, -1, 1), Vec4(1, -1, 1, 1), Vec4(1, -1, -1, 1)));


        light->AddChild(box);

        floor = std::make_shared<DynamicMesh>(this);
        
        floor->Triangles().push_back(new Triangle(Vec4(-1, 0, -1, 1), Vec4(-1, 0, 1, 1), Vec4(1, 0, 1, 1)));
        floor->Triangles().push_back(new Triangle(Vec4(-1, 0, -1, 1), Vec4(1, 0, 1, 1), Vec4(1, 0, -1, 1)));
        floor->Triangles()[0]->GetVertex(0)->UV() = Vec2(0, 10);
        floor->Triangles()[0]->GetVertex(1)->UV() = Vec2(0, 0);
        floor->Triangles()[0]->GetVertex(2)->UV() = Vec2(10, 0);
        floor->Triangles()[1]->GetVertex(0)->UV() = Vec2(0, 10);
        floor->Triangles()[1]->GetVertex(1)->UV() = Vec2(10, 0);
        floor->Triangles()[1]->GetVertex(2)->UV() = Vec2(10, 10);
        GetBaseComponent()->AddChild(floor);

        box->Scale() = Vec4(1.5f, 1.5f, 1.5f);
        floor->Position() = Vec4(0, 0, 0, 1.0f);
        floor->Scale() = Vec4(50, 1, 50);

        auto floor_material = std::make_shared<PBRMaterial>(Vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.8f, 1.0f, this);
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
    }

    virtual void Process(float p_delta) override
    {        
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_left") == InputManager::InputState::Pressed)
            light->Position() += Vec4::Cross(Vec4::UP, GetUsingCamera()->GetDirection()).Normalize() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_right") == InputManager::InputState::Pressed)
            light->Position() += Vec4::Cross(GetUsingCamera()->GetDirection(), Vec4::UP).Normalize() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_up") == InputManager::InputState::Pressed)
            light->Position() += Vec4::UP * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_down") == InputManager::InputState::Pressed)
            light->Position() -= Vec4::UP * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_forward") == InputManager::InputState::Pressed)
            light->Position() += Vec4::Cross(Vec4::UP, Vec4::Cross(GetUsingCamera()->GetDirection(), Vec4::UP)).Normalize() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_backward") == InputManager::InputState::Pressed)
            light->Position() += Vec4::Cross(Vec4::Cross(GetUsingCamera()->GetDirection(), Vec4::UP), Vec4::UP).Normalize() * p_delta * 10;
        if (window2 != nullptr && window2->IsClosed())
        {
            window2.reset();
            window2 = nullptr;
        }
        
    }

    virtual void Draw() override
    {
        Window::Draw();
        light->SetUniform(0);
        GetShaderProgram()->SetUniform("point_light_count", 1);
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

        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("forward", Input::KEY_W);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("backward", Input::KEY_S);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("left", Input::KEY_A);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("right", Input::KEY_D);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("down", Input::KEY_Q);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("up", Input::KEY_E);

        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("rotate_x", Input::KEY_UP);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("rotate_-x", Input::KEY_DOWN);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("rotate_y", Input::KEY_LEFT);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("rotate_-y", Input::KEY_RIGHT);

        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_left", Input::KEY_J);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_right", Input::KEY_L);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_up", Input::KEY_O);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_down", Input::KEY_U);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_forward", Input::KEY_I);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_backward", Input::KEY_K);

        Game::GetInstance()->Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}