#include "ce/geometry/triangle.h"

Triangle::Triangle()
    : AGeometry()
{
    vertices[0] = new Vertex();
    vertices[1] = new Vertex();
    vertices[2] = new Vertex();
    vertices[0]->InsertNext(vertices[1]);
    vertices[1]->InsertNext(vertices[2]);
    vertices[2]->InsertNext(vertices[0]);
}

Triangle::Triangle(const Triangle& p_other)
    : AGeometry(p_other)
{
    vertices[0] = new Vertex(*p_other.vertices[0]);
    vertices[1] = new Vertex(*p_other.vertices[1]);
    vertices[2] = new Vertex(*p_other.vertices[2]);
    vertices[0]->InsertNext(vertices[1]);
    vertices[1]->InsertNext(vertices[2]);
    vertices[2]->InsertNext(vertices[0]);
}

Triangle::Triangle(Triangle&& p_other) noexcept
    : AGeometry(std::move(p_other))
{
    vertices[0] = p_other.vertices[0];
    vertices[1] = p_other.vertices[1];
    vertices[2] = p_other.vertices[2];
    p_other.vertices[0] = nullptr;
    p_other.vertices[1] = nullptr;
    p_other.vertices[2] = nullptr;
}

Triangle::Triangle(const Vec4& p_v1, const Vec4& p_v2, const Vec4& p_v3)
{
    vertices[0] = new Vertex(p_v1);
    vertices[1] = new Vertex(p_v2);
    vertices[2] = new Vertex(p_v3);
    vertices[0]->InsertNext(vertices[1]);
    vertices[1]->InsertNext(vertices[2]);
    vertices[2]->InsertNext(vertices[0]);
}

Triangle::Triangle(const Vertex& p_v1, const Vertex& p_v2, const Vertex& p_v3)
{
    vertices[0] = new Vertex(p_v1);
    vertices[1] = new Vertex(p_v2);
    vertices[2] = new Vertex(p_v3);
    vertices[0]->InsertNext(vertices[1]);
    vertices[1]->InsertNext(vertices[2]);
    vertices[2]->InsertNext(vertices[0]);
}
Triangle::Triangle(Vertex*&& p_v1, Vertex*&& p_v2, Vertex*&& p_v3) noexcept
{
    vertices[0] = p_v1;
    vertices[1] = p_v2;
    vertices[2] = p_v3;
    p_v1 = nullptr;
    p_v2 = nullptr;
    p_v3 = nullptr;
}

Triangle::~Triangle()
{
    delete vertices[0];
    delete vertices[1];
    delete vertices[2];
}

float* Triangle::GetVertexArray(float* p_buff, size_t p_buff_size) const
{
    if (p_buff_size < TRIANGLE_ARRAY_SIZE)
        throw std::domain_error("The buffer size is too small.");
    for (size_t i = 0; i < 3; ++i)
        vertices[i]->GetArray(p_buff + i * Vertex::ARRAY_SIZE, Vertex::ARRAY_SIZE);
    return p_buff;
}