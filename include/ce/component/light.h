#pragma once
#include "ce/component/component3D.h"

namespace CrossEngine
{
    class Window;
    /**
     * @brief A light in the scene.
     */
    class ALight : public Component3D
    {
        bool cast_shadow = true;
    protected:

        /**
         * @brief Draw the light.
         * 
         */
        virtual void Draw(Window* p_context) override;
        
    public:
        ALight(const std::string& p_component_name = "light");
        virtual ~ALight();

        /**
         * @brief Set the light should cast shadow or not.
         * 
         * @param p_cast_shadow Should the light cast shadow.
         */
        FORCE_INLINE void SetCastShadow(bool p_cast_shadow) { cast_shadow = p_cast_shadow; }

        /**
         * @brief Should the light cast shadow or not.
         * 
         * @return true The light should cast shadow.
         * @return false The light should not cast shadow.
         */
        FORCE_INLINE bool const ShouldCastShadow() { return cast_shadow; }

        /**
         * @brief Get the name of the uniform variable.
         * 
         * @return std::string The name of the uniform variable.
         */
        virtual std::string UniformName() const = 0;

        /**
         * @brief Set the uniform of the light's data.
         * 
         */
        virtual void SetUniform(Window* p_context, size_t p_index) = 0;

        /**
         * @brief Register the draw of the light.
         * 
         */
        virtual bool RegisterDraw(Window* p_context) override;
    };
}