#pragma once
#include "ce/component/component3D.h"

namespace CrossEngine
{
    /**
     * @brief A camera of the scene.
     * 
     */
    class Camera : public Component3D
    {

    public:

        Camera(const std::string& p_component_name = "camera");

        Math::Mat4 GetViewMatrix() const;

        /**
         * @brief Rotate the camera to look at a target.
         * 
         * @param p_target The target to look at.
         */
        void LookAt(const Math::Vec4& p_target);
    };
}