#include "ce/geometry/vertex.h"


Vertex::Vertex()
    : position(Pos())
{
    
}

Vertex::Vertex(const Vec4& p_position)
    : position(p_position)
{

}

Vertex::Vertex(const Vertex& p_other)
    : position(p_other.position), 
    prev(nullptr), next(nullptr)
{}

Vertex::Vertex(Vertex&& p_other) noexcept
    : position(p_other.position), 
    prev(p_other.prev), next(p_other.next)
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

float* Vertex::GetArray(float* p_buff, size_t p_buff_size) const
{
    if (p_buff_size < ARRAY_SIZE)
        throw std::domain_error("The buffer size is too small.");
    p_buff[0] = position[0];
    p_buff[1] = position[1];
    p_buff[2] = position[2];
    p_buff[3] = position[3];
    return p_buff;
}

Vec4 Vertex::GetInducedNormal() const
{
    if (prev == nullptr || next == nullptr)
        throw std::domain_error("prev or next is nullptr");
    auto prev_vec = prev->position - position;
    auto next_vec = next->position - position;
    return Vec4::Cross(prev_vec, next_vec).Normalize();
}