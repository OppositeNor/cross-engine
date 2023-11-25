#include "ce/geometry/triangle.h"
#include "ce/graphics/window.h"
#include "ce/graphics/graphics.h"
#include "ce/resource/resource.h"
#include "ce/component/dynamic_mesh.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cmath>

Vec4 new_rot = Vec4(0, 0, 0, 1.0f);

class UserWindow : public Window
{
    std::unique_ptr<Window> window1;
    std::unique_ptr<Window> window2;

    
    DynamicMesh* mesh;
public:
    UserWindow(const std::string& title)
        : Window(1260, 720, title)
    {
    }

    virtual void Ready() override
    {
        mesh = new DynamicMesh(this);
        mesh->LoadTriangles(Resource::GetExeDirectory() + "/teapot_bezier0.tris");
    }

    virtual void Process(float p_delta) override
    {
        mesh->Rotation() = Lerp(p_delta * 2, mesh->GetRotation(), new_rot);
    }

    virtual void Draw() override
    {
        mesh->Update(0.001f);
        glBindVertexArray(mesh->GetVAO());
        mesh->Position() = Vec4(0, 0, 10, 1.0f);
        auto time = glfwGetTime();
        mesh->SetRotationEuler(Vec4(time, time, time, 1.0f), EulerRotOrder::EULAR_ROTATION_ORDER_PRY);
        GetShaderProgram()->SetUniform("model", mesh->GetSubspaceMatrix());
        GetShaderProgram()->SetUniform("proj", Mat4::ProjPersp(7.0f, -7.0f, 4.0f, -4.0f, 5.0f, 30.0f));
        GetShaderProgram()->Use();
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertexCount());
        glBindVertexArray(0);
    }

    virtual void OnClose() override
    {
        delete mesh;
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
                if (input == "exit")
                    break;
                else if (input[0] == 'x')
                {
                    new_rot[0] = std::stof(input.substr(2));
                }
                else if (input[0] == 'y')
                {
                    new_rot[1] = std::stof(input.substr(2));
                }
                else if (input[0] == 'z')
                {
                    new_rot[2] = std::stof(input.substr(2));
                }
                else if (input[0] == 'w')
                {
                    new_rot[3] = std::stof(input.substr(2));
                }
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