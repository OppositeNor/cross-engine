#include "ce/geometry/polygon.h"

Polygon::Polygon(std::initializer_list<Vertex*>&& p_vertices)
    : vertices(p_vertices)
{
    vertices.reserve(p_vertices.size());
    for (size_t i = 0; i < p_vertices.size(); i++)
    {
        vertices.push_back(p_vertices.begin()[i]);
        vertices[i]->InsertNext(vertices[(i + 1) % p_vertices.size()]);
    }
}

Polygon::~Polygon()
{
    for (size_t i = 0; i < vertices.size(); i++)
        delete vertices[i];
}

void Polygon::AddVertex(size_t p_index, Vertex* p_vertex)
{
    p_index %= vertices.size();
    vertices.insert(vertices.begin() + p_index, p_vertex);
    vertices[p_index - 1]->InsertNext(p_vertex);
}

void Polygon::Triangulate(std::vector<Triangle*>& p_triangles) const
{
    if (vertices.size() < 3)
        throw std::runtime_error("Cannot triangulate a polygon with less than 3 vertices");
    if (vertices.size() == 3)
    {
        p_triangles.push_back(new Triangle(vertices[0]->GetPosition(), vertices[1]->GetPosition(), vertices[2]->GetPosition()));
        return;
    }
    std::vector<Vertex*> verticesCopy(vertices);
    p_triangles.reserve(vertices.size() - 2);
    while (verticesCopy.size() > 3)
    {
        for (auto i = verticesCopy.begin(); i != verticesCopy.end();)
        {
            if ((*i)->IsEar())
            {
                p_triangles.push_back(new Triangle((*i)->GetPrev()->GetPosition(), (*i)->GetPosition(), (*i)->GetNext()->GetPosition()));
                (*i)->GetPrev()->RemoveNext();
                verticesCopy.erase(i);
            }
            else
                ++i;
        }
    }
}