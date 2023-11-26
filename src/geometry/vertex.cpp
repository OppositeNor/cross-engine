#include "ce/geometry/vertex.h"


Vertex::Vertex()
{
}

Vertex::Vertex(const Vec4& p_position)
    : Vertex()
{
    position = p_position;
}

Vertex::Vertex(const Vertex& p_other)
    : position(p_other.position), 
    prev(nullptr), next(nullptr), normal(p_other.normal)
{}

Vertex::Vertex(Vertex&& p_other) noexcept
    : position(p_other.position), 
    prev(p_other.prev), next(p_other.next), normal(p_other.normal)
{
    p_other.prev = nullptr;
    p_other.next = nullptr;
}

Vertex::~Vertex()
{
    if (prev != nullptr)
        prev->next = next;
    if (next != nullptr)
        next->prev = prev;
    prev = nullptr;
    next = nullptr;
}

void Vertex::InsertPrev(Vertex* p_vertex)
{
    if (p_vertex == nullptr)
        throw std::invalid_argument("p_vertex cannot be nullptr");
    if (next == nullptr)
    {
        ConnectPrev(p_vertex);
        return;
    }
    prev->next = p_vertex;
    p_vertex->prev = this->prev;
    p_vertex->next = this;
    prev = p_vertex;
}

void Vertex::InsertNext(Vertex* p_vertex)
{
    if (p_vertex == nullptr)
        throw std::invalid_argument("p_vertex cannot be nullptr");
    if (next == nullptr)
    {
        ConnectNext(p_vertex);
        return;
    }
    next->prev = p_vertex;
    p_vertex->next = this->next;
    p_vertex->prev = this;
    next = p_vertex;
}

void Vertex::ConnectPrev(Vertex* p_vertex)
{
    if (p_vertex == nullptr)
        throw std::invalid_argument("p_vertex cannot be nullptr");
    if (prev != nullptr)
    {
        prev->next = p_vertex;
        p_vertex->prev = prev;
    }
    prev = p_vertex;
    p_vertex->next = this;
}

void Vertex::ConnectNext(Vertex* p_vertex)
{
    if (p_vertex == nullptr)
        throw std::invalid_argument("p_vertex cannot be nullptr");
    if (next != nullptr)
    {
        next->prev = p_vertex;
        p_vertex->next = next;
    }
    next = p_vertex;
    p_vertex->prev = this;
}

Vertex* Vertex::RemovePrev()
{
    if (prev == nullptr)
        throw std::invalid_argument("prev cannot be nullptr");
    auto temp = prev;
    prev = prev->prev;
    if (prev != nullptr)
        prev->next = this;
    temp->prev = nullptr;
    temp->next = nullptr;
    return temp;
}

Vertex* Vertex::RemoveNext()
{
    if (next == nullptr)
        throw std::invalid_argument("next cannot be nullptr");
    auto temp = next;
    next = next->next;
    if (next != nullptr)
        next->prev = this;
    temp->prev = nullptr;
    temp->next = nullptr;
    return temp;
}

float* Vertex::GetArray(float* p_buff, size_t p_buff_size, bool p_use_induced_normal) const
{
    if (p_buff_size < ARRAY_SIZE)
        throw std::domain_error("The buffer size is too small.");
    for (size_t i = 0; i < 4; ++i)
        p_buff[i] = position[i];
    
    if (p_use_induced_normal)
    {
        Vec4 temp_normal = GetInducedNormal();
        for (size_t i = 0; i < 4; ++i)
            p_buff[i + 4] = temp_normal[i];
    }
    else
    {
        for (size_t i = 0; i < 4; ++i)
            p_buff[i + 4] = normal[i];
    }

    return p_buff;
}

Vec4 Vertex::GetInducedNormal() const
{
    if (prev == nullptr || next == nullptr)
        throw std::domain_error("The vertex is not fully connected.");
    auto prev_vec = prev->position - position;
    auto next_vec = next->position - position;
    return Vec4::Cross(prev_vec, next_vec).Normalize();
}