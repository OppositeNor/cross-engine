#include "ce/geometry/polygon.h"

namespace CrossEngine
{
    PolygonN::PolygonN()
    {
    }

    PolygonN::PolygonN(std::initializer_list<Vertex*>&& p_vertices)
        : vertices(p_vertices)
    {
        vertices.reserve(p_vertices.size());
        for (size_t i = 0; i < p_vertices.size(); i++)
        {
            vertices.push_back(p_vertices.begin()[i]);
            vertices[i]->InsertNext(vertices[(i + 1) % p_vertices.size()]);
        }
    }

    PolygonN::~PolygonN()
    {
        for (size_t i = 0; i < vertices.size(); i++)
            delete vertices[i];
    }

    void PolygonN::AddVertex(size_t p_index, Vertex* p_vertex)
    {
        if (vertices.size() == 0 && p_index <= 1)
        {
            vertices.push_back(p_vertex);
            p_vertex->ConnectPrev(p_vertex);
            return;
        }
        if (p_index > vertices.size())
            throw std::out_of_range("p_index is out of range");
        vertices.insert(vertices.begin() + p_index, p_vertex);
        vertices[p_index - 1]->InsertNext(p_vertex);
    }

    void PolygonN::Triangulate(std::vector<Triangle*>& p_triangles) const
    {
        if (vertices.size() < 3)
            throw std::runtime_error("Cannot triangulate a PolygonN with less than 3 vertices");
        if (vertices.size() == 3)
        {
            p_triangles.push_back(new Triangle(*(vertices[0]), *(vertices[1]), *(vertices[2])));
            return;
        }
        auto current_vertex = vertices[0];
        p_triangles.reserve(vertices.size() - 2);
        while (current_vertex->GetPrev() != current_vertex->GetNext())
        {
            if (/*current_vertex->IsEar()*/ true)
            {
                p_triangles.push_back(new Triangle(*(current_vertex->GetPrev()), *current_vertex, *(current_vertex->GetNext())));
                auto temp = current_vertex->GetNext();
                current_vertex->GetPrev()->RemoveNext();
                current_vertex = temp;
            }
            current_vertex = current_vertex->GetNext();
        }
        p_triangles.push_back(new Triangle(*(current_vertex->GetPrev()), *current_vertex, *(current_vertex->GetNext())));
    }
}