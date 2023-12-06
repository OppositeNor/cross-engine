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
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cmath>

Vec4 new_rot = Vec4(0, 0, 0, 1.0f);
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
    std::shared_ptr<UserWindow> window2 = nullptr;
    VisualMesh* mesh = nullptr;
    DynamicMesh* box = nullptr;
    DynamicMesh* box2 = nullptr;
    PointLight* light = nullptr;
    PointLight* light2 = nullptr;
    UserCamera* camera = nullptr;
    std::string title;

    
public:
    virtual void OnEvent(std::shared_ptr<AEvent> p_event) override {
    }

    UserWindow(const std::string& p_title)
        : Window(1260, 720, p_title)
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
        
        mesh = new StaticMesh(this);
        if (title == "")
            mesh->LoadTriangles(Resource::LoadModel(Resource::GetExeDirectory() + "/monkey.obj"));
        else
            mesh->LoadTriangles(Resource::LoadModel(Resource::GetExeDirectory() + "/teapot_bezier2.norm"));
        mesh->Scale() = Vec4(1.5, 1.5, 1.5);
        mesh->Position() = Vec4(0, 0, 0, 1.0f);
        light = new PointLight(Vec4(1.0f, 1.0f, 1.0f, 1.0f), 20, this);
        light->Position() = Vec4(10.0f, 10.0f, 10.0f, 1.0f);
        camera = new UserCamera(this);
        camera->Position() = Vec4(0, 0, -20, 1.0f);

        box = new DynamicMesh(this);
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

        box2 = new DynamicMesh(this);
        // Box mesh counter clock wise
        //front face
        box2->Triangles().push_back(new Triangle(Vec4(-1, -1, 1, 1), Vec4(1, -1, 1, 1), Vec4(1, 1, 1, 1)));
        box2->Triangles().push_back(new Triangle(Vec4(-1, -1, 1, 1), Vec4(1, 1, 1, 1), Vec4(-1, 1, 1, 1)));
        //back face
        box2->Triangles().push_back(new Triangle(Vec4(1, -1, -1, 1), Vec4(-1, -1, -1, 1), Vec4(1, 1, -1, 1)));
        box2->Triangles().push_back(new Triangle(Vec4(1, 1, -1, 1), Vec4(-1, -1, -1, 1), Vec4(-1, 1, -1, 1)));
        //left face
        box2->Triangles().push_back(new Triangle(Vec4(-1, -1, -1, 1), Vec4(-1, -1, 1, 1), Vec4(-1, 1, 1, 1)));
        box2->Triangles().push_back(new Triangle(Vec4(-1, -1, -1, 1), Vec4(-1, 1, 1, 1), Vec4(-1, 1, -1, 1)));
        //right face
        box2->Triangles().push_back(new Triangle(Vec4(1, -1, 1, 1), Vec4(1, -1, -1, 1), Vec4(1, 1, 1, 1)));
        box2->Triangles().push_back(new Triangle(Vec4(1, 1, 1, 1), Vec4(1, -1, -1, 1), Vec4(1, 1, -1, 1)));
        //top face
        box2->Triangles().push_back(new Triangle(Vec4(-1, 1, -1, 1), Vec4(-1, 1, 1, 1), Vec4(1, 1, 1, 1)));
        box2->Triangles().push_back(new Triangle(Vec4(-1, 1, -1, 1), Vec4(1, 1, 1, 1), Vec4(1, 1, -1, 1)));
        //bottom face
        box2->Triangles().push_back(new Triangle(Vec4(-1, -1, 1, 1), Vec4(-1, -1, -1, 1), Vec4(1, -1, 1, 1)));
        box2->Triangles().push_back(new Triangle(Vec4(1, -1, 1, 1), Vec4(-1, -1, -1, 1), Vec4(1, -1, -1, 1)));

        box->Scale() = Vec4(1.5f, 1.5f, 1.5f);
        box2->Position() = Vec4(-5, 0, 5, 1.0f);
        light->AddChild(box);

        light2 = new PointLight(this);
    }

    virtual void Process(float p_delta) override
    {
        mesh->Rotation() = Lerp(p_delta * 2, mesh->GetRotation(), new_rot);
        mesh->Update(p_delta);
        box2->Update(p_delta);
        camera->Update(p_delta);
        light->Update(p_delta);

        auto time = glfwGetTime();
        box2->Position()[1] = std::sin(time) * 5;

        if (rotate[0])
            new_rot = EulerToQuat(Vec3(time, 0, 0), EulerRotOrder::PRY);
        else if (rotate[1])
            new_rot = EulerToQuat(Vec3(0, time, 0), EulerRotOrder::PRY);
        else if (rotate[2])
            new_rot = EulerToQuat(Vec3(0, 0, time), EulerRotOrder::PRY);
        
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_left") == InputManager::InputState::Pressed)
            light->Position() += Vec4::Cross(Vec4::UP, camera->GetDirection()).Normalized() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_right") == InputManager::InputState::Pressed)
            light->Position() += Vec4::Cross(camera->GetDirection(), Vec4::UP).Normalized() * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_up") == InputManager::InputState::Pressed)
            light->Position() += Vec4::UP * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_down") == InputManager::InputState::Pressed)
            light->Position() -= Vec4::UP * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_forward") == InputManager::InputState::Pressed)
            light->Position() += Vec4::Cross(Vec4::UP, Vec4::Cross(camera->GetDirection(), Vec4::UP)) * p_delta * 10;
        if (Game::GetInstance()->GetInputManager()->GetInputState(this, "light_move_backward") == InputManager::InputState::Pressed)
            light->Position() += Vec4::Cross(Vec4::Cross(camera->GetDirection(), Vec4::UP), Vec4::UP) * p_delta * 10;
        if (window2 != nullptr && window2->IsClosed())
        {
            window2.reset();
            window2 = nullptr;
        }
        
    }

    virtual void Draw() override
    {
        GetShaderProgram()->Use();
        GetShaderProgram()->SetUniform("view", camera->GetViewMatrix());
        GetShaderProgram()->SetUniform("proj", Mat4::ProjPersp(1.75f, -1.75f, 1.0f, -1.0f, 2.5f, 40.0f));
        GetShaderProgram()->SetUniform("ambient_color", Vec4(1.0f, 1.0f, 1.0f, 1.0f));
        GetShaderProgram()->SetUniform("ambient_intensity", 0.7f);
        GetShaderProgram()->SetUniform("camera_position", camera->GetGlobalPosition());
        light->SetUniform(0);
        GetShaderProgram()->SetUniform("point_light_count", 1);

        mesh->Draw();
        box->Draw();
        box2->Draw();
        glBindVertexArray(0);
    }

    virtual void OnClose() override
    {
        delete mesh;
        delete camera;
        delete box;
        delete light;
        delete light2;
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
        

        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("forward", GLFW_KEY_W);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("backward", GLFW_KEY_S);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("left", GLFW_KEY_A);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("right", GLFW_KEY_D);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("down", GLFW_KEY_Q);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("up", GLFW_KEY_E);

        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("rotate_x", GLFW_KEY_UP);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("rotate_-x", GLFW_KEY_DOWN);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("rotate_y", GLFW_KEY_LEFT);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("rotate_-y", GLFW_KEY_RIGHT);

        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_left", GLFW_KEY_J);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_right", GLFW_KEY_L);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_up", GLFW_KEY_O);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_down", GLFW_KEY_U);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_forward", GLFW_KEY_I);
        Game::GetInstance()->Game::GetInstance()->GetInputManager()->AddInput("light_move_backward", GLFW_KEY_K);

        Game::GetInstance()->Run();

        Game::GetInstance()->Terminate();
#if 0
        try
        {
            std::cout << ">";
            std::string input;
            std::cin >> input;
            if (input == "EulerPitch")
                rotate = Vector<bool, 3>(true, false, false);
            else if (input == "EulerYaw")
                rotate = Vector<bool, 3>(false, true, false);
            else if (input == "EulerRoll")
                rotate = Vector<bool, 3>(false, false, true);
            else
                rotate = Vector<bool, 3>(false, false, false);

            if (input == "exit")
                break;
            else if (input[0] == 'x')
                new_rot[0] = std::stof(input.substr(2));
            else if (input[0] == 'y')
                new_rot[1] = std::stof(input.substr(2));
            else if (input[0] == 'z')
                new_rot[2] = std::stof(input.substr(2));
            else if (input[0] == 'w')
                new_rot[3] = std::stof(input.substr(2));
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }

        delete mod_window;
#endif
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}