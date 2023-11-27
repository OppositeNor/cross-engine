#include "ce/geometry/triangle.h"
#include "ce/graphics/window.h"
#include "ce/graphics/graphics.h"
#include "ce/resource/resource.h"
#include "ce/component/dynamic_mesh.h"
#include "ce/component/camera.h"
#include "ce/handlers/input_handler.h"
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
        if (GetContext()->GetInputHandler()->GetInputState("forward") & InputHandler::InputState::Pressed)
            Position() += GetDirection() * p_delta * 10;
        if (GetContext()->GetInputHandler()->GetInputState("backward") & InputHandler::InputState::Pressed)
            Position() += -1 * GetDirection() * p_delta * 10;
        if (GetContext()->GetInputHandler()->GetInputState("left") & InputHandler::InputState::Pressed)
            Position() += Vec4::Cross(Vec4::UP, GetDirection()).Normalized() * p_delta * 10;
        if (GetContext()->GetInputHandler()->GetInputState("right") & InputHandler::InputState::Pressed)
            Position() += Vec4::Cross(GetDirection(), Vec4::UP).Normalized() * p_delta * 10;
        if (GetContext()->GetInputHandler()->GetInputState("up") & InputHandler::InputState::Pressed)
            Position() += Vec4::UP * p_delta * 10;
        if (GetContext()->GetInputHandler()->GetInputState("down") & InputHandler::InputState::Pressed)
            Position() += -1 * Vec4::UP * p_delta * 10;
        if (GetContext()->GetInputHandler()->GetInputState("rotate_x") & InputHandler::InputState::Pressed)
            Rotate(Vec4::Cross(GetDirection(), Vec4::UP), p_delta * 1.5);
        if (GetContext()->GetInputHandler()->GetInputState("rotate_-x") & InputHandler::InputState::Pressed)
            Rotate(Vec4::Cross(Vec4::UP, GetDirection()), p_delta * 1.5);
        if (GetContext()->GetInputHandler()->GetInputState("rotate_y") & InputHandler::InputState::Pressed)
            Rotate(Vec4::UP, p_delta * 1.5);
        if (GetContext()->GetInputHandler()->GetInputState("rotate_-y") & InputHandler::InputState::Pressed)
            Rotate(-1 * Vec4::UP, p_delta * 1.5);
    }
};

class UserWindow : public Window
{
    std::unique_ptr<Window> window1;
    std::unique_ptr<Window> window2;

    
    DynamicMesh* mesh = nullptr;
    DynamicMesh* mesh2 = nullptr;
    PointLight* light = nullptr;
    UserCamera* camera = nullptr;

public:
    UserWindow(const std::string& title)
        : Window(1260, 720, title)
    {
        
    }

    virtual void Ready() override
    {
        mesh = new DynamicMesh(this);
        mesh->LoadTriangles(Resource::GetExeDirectory() + "/teapot_bezier0.tris");
        mesh->Scale() = Vec4(1.5f, 1.5f, 1.5f);
        mesh->Position() = Vec4(0, 0, 0, 1.0f);
        light = new PointLight(Vec4(1.0f, 1.0f, 1.0f, 1.0f), this);
        light->Position() = Vec4(10.0f, 10.0f, 10.0f, 1.0f);
        light->Intensity() = 8.0f;
        camera = new UserCamera(this);
        camera->Position() = Vec4(0, 0, -20, 1.0f);

        mesh2 = new DynamicMesh(this);
        // Box mesh
        mesh2->GetTriangles().push_back(new Triangle(Vec4(1, 1, -1, 1), Vec4(1, -1, -1, 1), Vec4(-1, -1, -1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(1, 1, -1, 1), Vec4(-1, -1, -1, 1), Vec4(-1, 1, -1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(1, 1, -1, 1), Vec4(1, 1, 1, 1), Vec4(1, -1, 1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(1, 1, -1, 1), Vec4(1, -1, 1, 1), Vec4(1, -1, -1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(1, 1, 1, 1), Vec4(-1, 1, 1, 1), Vec4(-1, -1, 1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(1, 1, 1, 1), Vec4(-1, -1, 1, 1), Vec4(1, -1, 1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(-1, 1, 1, 1), Vec4(-1, 1, -1, 1), Vec4(-1, -1, -1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(-1, 1, 1, 1), Vec4(-1, -1, -1, 1), Vec4(-1, -1, 1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(1, 1, 1, 1), Vec4(1, 1, -1, 1), Vec4(-1, 1, -1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(1, 1, 1, 1), Vec4(-1, 1, -1, 1), Vec4(-1, 1, 1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(1, -1, 1, 1), Vec4(1, -1, -1, 1), Vec4(-1, -1, -1, 1)));
        mesh2->GetTriangles().push_back(new Triangle(Vec4(1, -1, 1, 1), Vec4(-1, -1, -1, 1), Vec4(-1, -1, 1, 1)));


        mesh2->Scale() = Vec4(1.5f, 1.5f, 1.5f);
        light->AddChild(mesh2);
        // mesh2->Position() = Vec4(5, 5, 5, 1.0f);
        std::cout << mesh2->GetGlobalPosition() << std::endl;

        GetInputHandler()->AddInput("forward", GLFW_KEY_W);
        GetInputHandler()->AddInput("backward", GLFW_KEY_S);
        GetInputHandler()->AddInput("left", GLFW_KEY_A);
        GetInputHandler()->AddInput("right", GLFW_KEY_D);
        GetInputHandler()->AddInput("down", GLFW_KEY_Q);
        GetInputHandler()->AddInput("up", GLFW_KEY_E);

        GetInputHandler()->AddInput("rotate_x", GLFW_KEY_UP);
        GetInputHandler()->AddInput("rotate_-x", GLFW_KEY_DOWN);
        GetInputHandler()->AddInput("rotate_y", GLFW_KEY_LEFT);
        GetInputHandler()->AddInput("rotate_-y", GLFW_KEY_RIGHT);

        GetInputHandler()->AddInput("light_move_left", GLFW_KEY_J);
        GetInputHandler()->AddInput("light_move_right", GLFW_KEY_L);
        GetInputHandler()->AddInput("light_move_up", GLFW_KEY_O);
        GetInputHandler()->AddInput("light_move_down", GLFW_KEY_U);
        GetInputHandler()->AddInput("light_move_forward", GLFW_KEY_I);
        GetInputHandler()->AddInput("light_move_backward", GLFW_KEY_K);
    }

    virtual void Process(float p_delta) override
    {
        mesh->Rotation() = Lerp(p_delta * 2, mesh->GetRotation(), new_rot);
        mesh->Update(p_delta);
        camera->Update(p_delta);
        light->Update(p_delta);
        auto time = glfwGetTime();

        if (rotate[0])
            new_rot = EulerToQuat(Vec3(time, 0, 0), EulerRotOrder::PRY);
        else if (rotate[1])
            new_rot = EulerToQuat(Vec3(0, time, 0), EulerRotOrder::PRY);
        else if (rotate[2])
            new_rot = EulerToQuat(Vec3(0, 0, time), EulerRotOrder::PRY);
        
        if (GetInputHandler()->GetInputState("light_move_left") & InputHandler::InputState::Pressed)
            light->Position() += Vec4::Cross(Vec4::UP, camera->GetDirection()).Normalized() * p_delta * 10;
        if (GetInputHandler()->GetInputState("light_move_right") & InputHandler::InputState::Pressed)
            light->Position() += Vec4::Cross(camera->GetDirection(), Vec4::UP).Normalized() * p_delta * 10;
        if (GetInputHandler()->GetInputState("light_move_up") & InputHandler::InputState::Pressed)
            light->Position() += Vec4::UP * p_delta * 10;
        if (GetInputHandler()->GetInputState("light_move_down") & InputHandler::InputState::Pressed)
            light->Position() -= Vec4::UP * p_delta * 10;
        if (GetInputHandler()->GetInputState("light_move_forward") & InputHandler::InputState::Pressed)
            light->Position() += Vec4::Cross(Vec4::UP, Vec4::Cross(camera->GetDirection(), Vec4::UP)) * p_delta * 10;
        if (GetInputHandler()->GetInputState("light_move_backward") & InputHandler::InputState::Pressed)
            light->Position() += Vec4::Cross(Vec4::Cross(camera->GetDirection(), Vec4::UP), Vec4::UP) * p_delta * 10;
        
    }

    virtual void Draw() override
    {
        GetShaderProgram()->SetUniform("model", mesh->GetSubspaceMatrix());
        GetShaderProgram()->SetUniform("view", camera->GetViewMatrix());
        GetShaderProgram()->SetUniform("proj", Mat4::ProjPersp(7.0f, -7.0f, 4.0f, -4.0f, 10.0f, 40.0f));
        light->SetUniform();
        GetShaderProgram()->Use();

        glBindVertexArray(mesh->GetVAO());
        GetShaderProgram()->SetUniform("model", mesh->GetSubspaceMatrix());
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertexCount());
        glBindVertexArray(mesh2->GetVAO());
        GetShaderProgram()->SetUniform("model", mesh2->GetSubspaceMatrix());
        glDrawArrays(GL_TRIANGLES, 0, mesh2->GetVertexCount());
        glBindVertexArray(0);
    }

    virtual void OnClose() override
    {
        delete mesh;
        delete camera;
        delete mesh2;
        delete light;
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
        Graphics::InitGraphics();
        UserWindow* mod_window = new UserWindow("");

        while (1)
        {
            try
            {
                if (mod_window->IsClosed())
                    break;
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
        }

        delete mod_window;
        Graphics::TerminateGraphics();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}