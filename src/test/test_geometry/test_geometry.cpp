#include "../unit_test/unit_test.h"
#include "ce/geometry/vertex.h"
#include "ce/geometry/triangle.h"

void UnitTest::TestVertex0()
{
    Vertex vertex = Vertex();
    EXPECT_VALUES_EQUAL(vertex.GetPosition(), Vec4(0, 0, 0, 1));
    auto vertex_2 = new Vertex(Vec4(1, 2, 3, 4));
    EXPECT_VALUES_EQUAL(vertex_2->GetPosition(), Vec4(1, 2, 3, 4));
    delete vertex_2;
    std::unique_ptr<Vertex> vertex_3(new Vertex(Vec4(1, 2, 3, 4)));
    EXPECT_VALUES_EQUAL(vertex_3->GetPosition(), Vec4(1, 2, 3, 4));
}

void UnitTest::TestVertex1()
{
    Vertex* vertex[3] = {new Vertex(), new Vertex(), new Vertex()};
    vertex[0]->InsertNext(vertex[1]);
    vertex[1]->InsertNext(vertex[2]);
    vertex[2]->InsertNext(vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[0]->GetNext(), vertex[1]);
    EXPECT_VALUES_EQUAL(vertex[1]->GetNext(), vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[2]->GetNext(), vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[0]->GetPrev(), vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[1]->GetPrev(), vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[2]->GetPrev(), vertex[1]);
    Vertex* new_vertex = new Vertex();
    vertex[0]->InsertNext(new_vertex);
    EXPECT_VALUES_EQUAL(vertex[0]->GetNext(), new_vertex);
    EXPECT_VALUES_EQUAL(new_vertex->GetNext(), vertex[1]);
    EXPECT_VALUES_EQUAL(vertex[1]->GetNext(), vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[2]->GetNext(), vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[0]->GetPrev(), vertex[2]);
    EXPECT_VALUES_EQUAL(new_vertex->GetPrev(), vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[1]->GetPrev(), new_vertex);
    EXPECT_VALUES_EQUAL(vertex[2]->GetPrev(), vertex[1]);
    Vertex* new_vertex_2 = new Vertex();
    vertex[2]->InsertPrev(new_vertex_2);
    EXPECT_VALUES_EQUAL(vertex[0]->GetNext(), new_vertex);
    EXPECT_VALUES_EQUAL(new_vertex->GetNext(), vertex[1]);
    EXPECT_VALUES_EQUAL(vertex[1]->GetNext(), new_vertex_2);
    EXPECT_VALUES_EQUAL(new_vertex_2->GetNext(), vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[2]->GetNext(), vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[0]->GetPrev(), vertex[2]);
    EXPECT_VALUES_EQUAL(new_vertex->GetPrev(), vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[1]->GetPrev(), new_vertex);
    EXPECT_VALUES_EQUAL(new_vertex_2->GetPrev(), vertex[1]);
    EXPECT_VALUES_EQUAL(vertex[2]->GetPrev(), new_vertex_2);

    delete vertex[0];
    delete vertex[1];
    delete vertex[2];
    delete new_vertex;
}

void UnitTest::TestVertex2()
{
    Vertex* vert = new Vertex();
    EXPECT_EXPRESSION_THROW_TYPE(([&](){
        vert->InsertNext(nullptr);
    }), std::invalid_argument);
    EXPECT_EXPRESSION_THROW_TYPE(([&](){
        vert->InsertPrev(nullptr);
    }), std::invalid_argument);
    EXPECT_EXPRESSION_THROW_TYPE(([&](){
        vert->ConnectNext(nullptr);
    }), std::invalid_argument);
    EXPECT_EXPRESSION_THROW_TYPE(([&](){
        vert->ConnectPrev(nullptr);
    }), std::invalid_argument);

    delete vert;
}

void UnitTest::TestVertex3()
{
    Vertex* vertex = new Vertex[3];
    vertex[0].InsertNext(&vertex[1]);
    vertex[1].InsertNext(&vertex[2]);
    vertex[2].InsertNext(&vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[0].RemoveNext(), &vertex[1]);
    EXPECT_VALUES_EQUAL(vertex[0].GetNext(), &vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[0].GetPrev(), &vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[1].GetNext(), nullptr);
    EXPECT_VALUES_EQUAL(vertex[1].GetPrev(), nullptr);
    EXPECT_VALUES_EQUAL(vertex[2].GetNext(), &vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[2].GetPrev(), &vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[2].RemovePrev(), &vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[0].GetNext(), nullptr);
    EXPECT_VALUES_EQUAL(vertex[0].GetPrev(), nullptr);
    EXPECT_VALUES_EQUAL(vertex[2].GetNext(), &vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[2].GetPrev(), &vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[2].RemoveNext(), &vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[2].GetNext(), nullptr);
    EXPECT_VALUES_EQUAL(vertex[2].GetPrev(), nullptr);
    vertex[2].ConnectNext(&vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[2].GetNext(), &vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[2].GetPrev(), &vertex[2]);
    vertex[2].ConnectNext(&vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[2].GetNext(), &vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[2].GetPrev(), &vertex[0]);
    EXPECT_VALUES_EQUAL(vertex[0].GetNext(), &vertex[2]);
    EXPECT_VALUES_EQUAL(vertex[0].GetPrev(), &vertex[2]);
    delete[] vertex;
}

void UnitTest::TestVertex4()
{
    Vertex* vertex = new Vertex();
    vertex->SetPosition(Vec4(3.0f, 144.0f, 3.25f, 1.0f));
    float buff[Vertex::ARRAY_SIZE];
    vertex->GetArray(buff, Vertex::ARRAY_SIZE);
    EXPECT_EXPRESSION_THROW_TYPE(([&](){
        vertex->GetArray(buff, Vertex::ARRAY_SIZE - 1);
    }), std::domain_error);
    EXPECT_VALUES_EQUAL(buff[0], 3.0f);
    EXPECT_VALUES_EQUAL(buff[1], 144.0f);
    EXPECT_VALUES_EQUAL(buff[2], 3.25f);
    EXPECT_VALUES_EQUAL(buff[3], 1.0f);
    delete vertex;
}

void UnitTest::TestTriangle0()
{
    Triangle triangle;
    EXPECT_VALUES_EQUAL(triangle.GetVertex(0)->GetPosition(), Vec4(0, 0, 0, 1));
    EXPECT_VALUES_EQUAL(triangle.GetVertex(1)->GetPosition(), Vec4(0, 0, 0, 1));
    EXPECT_VALUES_EQUAL(triangle.GetVertex(2)->GetPosition(), Vec4(0, 0, 0, 1));
    EXPECT_VALUES_EQUAL(triangle.GetVertex(0)->GetNext(), triangle.GetVertex(1));
    EXPECT_VALUES_EQUAL(triangle.GetVertex(1)->GetNext(), triangle.GetVertex(2));
    EXPECT_VALUES_EQUAL(triangle.GetVertex(2)->GetNext(), triangle.GetVertex(0));
    Triangle triangle1 = triangle;
    Triangle triangle2 = std::move(triangle);
    EXPECT_VALUES_EQUAL(triangle1.GetVertex(0)->GetPosition(), Vec4(0, 0, 0, 1));
    EXPECT_VALUES_EQUAL(triangle1.GetVertex(1)->GetPosition(), Vec4(0, 0, 0, 1));
    EXPECT_VALUES_EQUAL(triangle1.GetVertex(2)->GetPosition(), Vec4(0, 0, 0, 1));
    EXPECT_VALUES_EQUAL(triangle1.GetVertex(0)->GetNext(), triangle1.GetVertex(1));
    EXPECT_VALUES_EQUAL(triangle1.GetVertex(1)->GetNext(), triangle1.GetVertex(2));
    EXPECT_VALUES_EQUAL(triangle1.GetVertex(2)->GetNext(), triangle1.GetVertex(0));

    EXPECT_VALUES_EQUAL(triangle2.GetVertex(0)->GetPosition(), Vec4(0, 0, 0, 1));
    EXPECT_VALUES_EQUAL(triangle2.GetVertex(1)->GetPosition(), Vec4(0, 0, 0, 1));
    EXPECT_VALUES_EQUAL(triangle2.GetVertex(2)->GetPosition(), Vec4(0, 0, 0, 1));
    EXPECT_VALUES_EQUAL(triangle2.GetVertex(0)->GetNext(), triangle2.GetVertex(1));
    EXPECT_VALUES_EQUAL(triangle2.GetVertex(1)->GetNext(), triangle2.GetVertex(2));
    EXPECT_VALUES_EQUAL(triangle2.GetVertex(2)->GetNext(), triangle2.GetVertex(0));
    EXPECT_VALUES_EQUAL(triangle.GetVertex(0), nullptr);
    EXPECT_VALUES_EQUAL(triangle.GetVertex(1), nullptr);
    EXPECT_VALUES_EQUAL(triangle.GetVertex(2), nullptr);
}
void UnitTest::TestTriangle1()
{
    Triangle triangle;
    triangle.GetVertex(0)->SetPosition(Vec4(1, 2, 3, 1));
    triangle.GetVertex(1)->SetPosition(Vec4(4, 5, 6, 1));
    triangle.GetVertex(2)->SetPosition(Vec4(7, 8, 9, 1));
    float buff[3 * Vertex::ARRAY_SIZE];
    triangle.GetVertexArray(buff, 3 * Vertex::ARRAY_SIZE);
    EXPECT_EXPRESSION_THROW_TYPE(([&](){
        triangle.GetVertexArray(buff, 2 * Vertex::ARRAY_SIZE);
    }), std::domain_error);
    EXPECT_VALUES_EQUAL(buff[0], 1.0f);
    EXPECT_VALUES_EQUAL(buff[1], 2.0f);
    EXPECT_VALUES_EQUAL(buff[2], 3.0f);
    EXPECT_VALUES_EQUAL(buff[3], 1.0f);
    EXPECT_VALUES_EQUAL(buff[4], 4.0f);
    EXPECT_VALUES_EQUAL(buff[5], 5.0f);
    EXPECT_VALUES_EQUAL(buff[6], 6.0f);
    EXPECT_VALUES_EQUAL(buff[7], 1.0f);
    EXPECT_VALUES_EQUAL(buff[8], 7.0f);
    EXPECT_VALUES_EQUAL(buff[9], 8.0f);
    EXPECT_VALUES_EQUAL(buff[10], 9.0f);
    EXPECT_VALUES_EQUAL(buff[11], 1.0f);

}
void UnitTest::TestTriangle2()
{

}
void UnitTest::TestTriangle3()
{

}