#include "ce/geometry/triangle.h"
#include "ce/graphics/window.h"
#include "ce/graphics/graphics.h"
#include "ce/resource/resource.h"
#include "ce/component/dynamic_mesh.h"
#include "ce/component/camera.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cmath>

Vec4 new_rot = Vec4(0, 0, 0, 1.0f);
Vector<bool, 3> rotate;

class UserWindow : public Window
{
    std::unique_ptr<Window> window1;
    std::unique_ptr<Window> window2;

    
    DynamicMesh* mesh = nullptr;
    PointLight* light = nullptr;
    Camera* camera = nullptr;
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
        light->Position() = Vec4(10, 10, 0.0f, 1.0f);
        camera = new Camera(this);
        camera->Position() = Vec4(0, 0, -20, 1.0f);
    }

    virtual void Process(float p_delta) override
    {
        mesh->Rotation() = Lerp(p_delta * 2, mesh->GetRotation(), new_rot);
        mesh->Update(p_delta);
        glBindVertexArray(mesh->GetVAO());
        auto time = glfwGetTime();

        if (rotate[0])
            new_rot = EulerToQuat(Vec3(time, 0, 0), EulerRotOrder::PRY);
        else if (rotate[1])
            new_rot = EulerToQuat(Vec3(0, time, 0), EulerRotOrder::PRY);
        else if (rotate[2])
            new_rot = EulerToQuat(Vec3(0, 0, time), EulerRotOrder::PRY);
        camera->Rotate(Vec4(0, 1, 0), p_delta * 0.5f);
    }

    virtual void Draw() override
    {
        GetShaderProgram()->SetUniform("model", mesh->GetSubspaceMatrix());
        GetShaderProgram()->SetUniform("view", camera->GetViewMatrix());
        GetShaderProgram()->SetUniform("proj", Mat4::ProjPersp(3.5f, -3.5f, 2.0f, -2.0f, 5.0f, 40.0f));
        light->SetUniform();
        GetShaderProgram()->Use();
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertexCount());
        glBindVertexArray(0);
    }

    virtual void OnClose() override
    {
        delete mesh;
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