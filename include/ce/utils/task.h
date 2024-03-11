#pragma once
#include "ce/defs.hpp"
#include <functional>
#include <iostream>

namespace CrossEngine
{
    class Window;

    struct Task
    {
        std::function<void()> task;
        float priority;
        Task(const std::function<void()>& p_task, float priority)
            : task(p_task), priority(priority) {}
        
        FORCE_INLINE auto operator <=>(const Task& p_other) const
        {
            return priority <=> p_other.priority;
        }
    };
}