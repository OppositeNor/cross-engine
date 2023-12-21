#include "ce/geometry/vertex.h"



Vertex::Vertex(const Vec4& p_position, const Vec4& p_normal, const Vec2& p_uv)
    : position(p_position), normal(p_normal), uv(p_uv)
{

}

Vertex::Vertex(const Vec4& p_position)
    : Vertex(p_position, Vec4(), Vec2())
{
    position = p_position;
}

Vertex::Vertex()
    : Vertex(Pos())
{
}

Vertex::Vertex(const Vertex& p_other)
    : position(p_other.position), 
    prev(nullptr), next(nullptr), normal(p_other.normal), uv(p_other.uv)
{
    
}

Vertex::Vertex(Vertex&& p_other) noexcept
    : position(p_other.position), 
    prev(p_other.prev), next(p_other.next), normal(p_other.normal), uv(p_other.uv)
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

float* Vertex::GetArray(float* p_buff, size_t p_buff_size, const Vec4& p_tangent) const
{
    if (p_buff_size < ARRAY_SIZE)
        throw std::out_of_range("The buffer size is too small.");
    for (size_t i = 0; i < 4; ++i)
        p_buff[i] = position[i];
    
    if (normal == Vec4())
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
    for (size_t i = 0; i < 2; ++i)
        p_buff[i + 8] = uv[i];
    for (size_t i = 0; i < 4; ++i)
        p_buff[i + 10] = p_tangent[i];
    return p_buff;
}

Vec4 Vertex::GetInducedNormal() const
{
    if (prev == nullptr || next == nullptr)
        throw std::domain_error("The vertex is not fully connected.");
    auto prev_vec = position - prev->position;
    auto next_vec = next->position - position;
    return Vec4::Cross(next_vec, prev_vec).Normalize();
}

bool Vertex::IsEar() const
{
    if (prev == nullptr || next == nullptr)
        throw std::domain_error("The vertex is not fully connected.");
    auto prev_vec = position - prev->position;
    auto next_vec = next->position - position;
    auto normal = Vec4::Cross(next_vec, prev_vec).Normalize();
    auto current_vertex = next->next;
    while (current_vertex != prev)
    {
        auto current_vec = current_vertex->position - position;
        if (Vec4::Dot(normal, Vec4::Cross(current_vec, prev_vec)) < 0)
            return false;
        current_vertex = current_vertex->next;
    }
    return true;
}

Vec4 Vertex::GetTangent() const
{
    if (prev == nullptr || next == nullptr)
        throw std::domain_error("The vertex is not fully connected.");
    
    Vec4 delta_pos1 = GetNext()->GetPosition() - GetPosition();
    Vec4 delta_pos2 = GetPrev()->GetPosition() - GetPosition();
    Vec2 delta_uv1 = GetNext()->GetUV() - GetUV();
    Vec2 delta_uv2 = GetPrev()->GetUV() - GetUV();
    float temp = 1.0f / (delta_uv1[0] * delta_uv2[1] - delta_uv2[0] * delta_uv1[1]);
    Vec4 result;
    result[0] = temp * (delta_uv2[1] * delta_pos1[0] - delta_uv1[1] * delta_pos2[0]);
    result[1] = temp * (delta_uv2[1] * delta_pos1[1] - delta_uv1[1] * delta_pos2[1]);
    result[2] = temp * (delta_uv2[1] * delta_pos1[2] - delta_uv1[1] * delta_pos2[2]);
    return result.Normalize();
}