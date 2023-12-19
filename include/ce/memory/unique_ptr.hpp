#pragma once
#include "ce/defs.hpp"

template<typename T>
struct DefaultDeleter
{
    DefaultDeleter() = default;
    FORCE_INLINE void operator()(T*& p_ptr)
    {
        delete p_ptr;
        p_ptr = nullptr;
    }
};

template<typename T>
struct DefaultDeleter<T[]>
{
    DefaultDeleter() = default;
    FORCE_INLINE void operator()(T*& p_ptr)
    {
        delete[] p_ptr;
        p_ptr = nullptr;
    }
};

template<typename T, typename Td = DefaultDeleter<T>>
class UniquePtr
{
    T* ptr = nullptr;
    Td deleter;
public:
    UniquePtr() = default;
    FORCE_INLINE UniquePtr(T* p_ptr, const Td& p_deleter = DefaultDeleter<T>())
        : ptr(p_ptr), deleter(p_deleter)
    {}
    
    UniquePtr(const UniquePtr& p_other) = delete;

    FORCE_INLINE UniquePtr(UniquePtr&& p_other) noexcept
    {
        ptr = p_other.ptr;
        p_other.ptr = nullptr;
    }

    FORCE_INLINE ~UniquePtr()
    {
        if (ptr != nullptr)
            deleter(ptr);
    }

    FORCE_INLINE T* Get() noexcept
    {
        return ptr;
    }

    FORCE_INLINE T* operator->() noexcept
    {
        return ptr;
    }

    FORCE_INLINE const T* operator->() const
    {
        return ptr;
    }

    FORCE_INLINE auto& operator=(T* p_ptr)
    {
        if (ptr != nullptr)
            deleter(ptr);
        ptr = p_ptr;
        return *this;
    }

    FORCE_INLINE auto& operator=(UniquePtr&& p_other) noexcept
    {
        if (ptr != nullptr)
            deleter(ptr);
        ptr = p_other.ptr;
        p_other.ptr = nullptr;
        return *this;
    }

    FORCE_INLINE bool operator==(const T* p_ptr) const
    {
        return ptr == p_ptr;
    }

    FORCE_INLINE bool operator!=(const T* p_ptr) const
    {
        return ptr != p_ptr;
    }

    FORCE_INLINE bool operator==(const UniquePtr& p_other) const
    {
        return ptr == p_other.ptr;
    }

    FORCE_INLINE bool operator!=(const UniquePtr& p_other) const
    {
        return ptr != p_other.ptr;
    }

    FORCE_INLINE void ForceDelete()
    {
        if (ptr != nullptr)
            deleter(ptr);
        ptr = nullptr;
    }
};

template<typename T, typename Td>
class UniquePtr<T[], Td>
{
    T* ptr = nullptr;
    Td deleter;
public:
    UniquePtr() = default;
    FORCE_INLINE UniquePtr(T* p_ptr, const Td& p_deleter = DefaultDeleter<T[]>())
        : ptr(p_ptr), deleter(p_deleter)
    {}

    UniquePtr(const UniquePtr& p_other) = delete;

    FORCE_INLINE UniquePtr(UniquePtr&& p_other) noexcept
    {
        ptr = p_other.ptr;
        p_other.ptr = nullptr;
    }

    FORCE_INLINE ~UniquePtr()
    {
        if (ptr != nullptr)
            deleter(ptr);
    }

    FORCE_INLINE T* Get() noexcept
    {
        return ptr;
    }

    FORCE_INLINE T* operator->() noexcept
    {
        return ptr;
    }

    FORCE_INLINE const T* operator->() const
    {
        return ptr;
    }

    FORCE_INLINE T& operator[](int i) noexcept
    {
        return ptr[i];
    }

    FORCE_INLINE auto& operator=(T* p_ptr)
    {
        if (ptr != nullptr)
            deleter(ptr);
        ptr = p_ptr;
        return *this;
    }

    FORCE_INLINE auto& operator=(UniquePtr&& p_other) noexcept
    {
        if (ptr != nullptr)
            deleter(ptr);
        ptr = p_other.ptr;
        p_other.ptr = nullptr;
        return *this;
    }

    FORCE_INLINE bool operator==(const T* p_ptr) const
    {
        return ptr == p_ptr;
    }

    FORCE_INLINE bool operator!=(const T* p_ptr) const
    {
        return ptr != p_ptr;
    }

    FORCE_INLINE bool operator==(const UniquePtr& p_other) const
    {
        return ptr == p_other.ptr;
    }

    FORCE_INLINE bool operator!=(const UniquePtr& p_other) const
    {
        return ptr != p_other.ptr;
    }

    FORCE_INLINE void ForceDelete()
    {
        if (ptr != nullptr)
            deleter(ptr);
        ptr = nullptr;
    }
};