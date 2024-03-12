#include "ce/geometry/triangle.h"

namespace CrossEngine
{
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

    Triangle::Triangle(const Math::Vec4& p_v1, const Math::Vec4& p_v2, const Math::Vec4& p_v3)
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
            throw std::out_of_range("The buffer size is too small.");
        auto tangent = GetTangent();
        for (size_t i = 0; i < 3; ++i)
        {
            vertices[i]->GetArray(p_buff + Vertex::ARRAY_SIZE * i, Vertex::ARRAY_SIZE);
        }
        return p_buff;
    }

    Math::Vec4 Triangle::GetTangent() const
    {
        Math::Vec4 delta_pos1 = vertices[1]->GetPosition() - vertices[0]->GetPosition();
        Math::Vec4 delta_pos2 = vertices[2]->GetPosition() - vertices[0]->GetPosition();
        Math::Vec2 delta_uv1 = vertices[1]->GetUV() - vertices[0]->GetUV();
        Math::Vec2 delta_uv2 = vertices[2]->GetUV() - vertices[0]->GetUV();
        float temp = 1.0f / (delta_uv1[0] * delta_uv2[1] - delta_uv2[0] * delta_uv1[1]);
        Math::Vec4 result;
        result[0] = temp * (delta_uv2[1] * delta_pos1[0] - delta_uv1[1] * delta_pos2[0]);
        result[1] = temp * (delta_uv2[1] * delta_pos1[1] - delta_uv1[1] * delta_pos2[1]);
        result[2] = temp * (delta_uv2[1] * delta_pos1[2] - delta_uv1[1] * delta_pos2[2]);
        
        return result.Normalize();
    }

    Math::Vec4 Triangle::GetCenter() const
    {
        return (vertices[0]->GetPosition() + vertices[1]->GetPosition() + vertices[2]->GetPosition()) / 3.0f;
    }

    const Vertex* Triangle::GetClosest(const Math::Mat4& p_subspace_matrix, const Math::Vec4& p_point) const
    {
        const Vertex* result = vertices[0];
        float min_dist = (p_subspace_matrix * vertices[0]->GetPosition() - p_point).LengthSquared();
        for (size_t i = 1; i < 3; ++i)
        {
            float dist = (p_subspace_matrix * vertices[i]->GetPosition() - p_point).LengthSquared();
            if (dist < min_dist)
            {
                min_dist = dist;
                result = vertices[i];
            }
        }
        return result;
    }

    Math::Vec4 Triangle::GetClosestPosition(const Math::Mat4& p_subspace_matrix, const Math::Vec4& p_point) const
    {
        Math::Vec4 result = p_subspace_matrix * vertices[0]->GetPosition();
        float min_dist = (result - p_point).LengthSquared();
        for (size_t i = 1; i < 3; ++i)
        {
            auto global_position = p_subspace_matrix * vertices[i]->GetPosition();
            float dist = (global_position - p_point).LengthSquared();
            if (dist < min_dist)
            {
                min_dist = dist;
                result = global_position;
            }
        }
        return result;
    }

    float Triangle::GetLeastDepth(const Math::Mat4& p_subspace_matrix, const Math::Vec4& p_point, Math::Vec4 p_dir) const
    {
        auto global_position = p_subspace_matrix * vertices[0]->GetPosition();
        float min_depth = (global_position - p_point).Dot(p_dir.Normalize());
        for (size_t i = 1; i < 3; ++i)
        {
            global_position = p_subspace_matrix * vertices[i]->GetPosition();
            float depth = (global_position - p_point).Dot(p_dir);
            if (depth < min_depth)
                min_depth = depth;
        }
        return min_depth;
    }
}