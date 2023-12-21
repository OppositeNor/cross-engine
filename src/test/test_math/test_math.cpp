#include "../unit_test/unit_test.h"
#include "ce/math/math.hpp"

void UnitTest::TestVector0()
{
    Math::Vec<4> vec(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_VALUES_EQUAL(vec[0], 1.0f);
    EXPECT_VALUES_EQUAL(vec[1], 2.0f);
    EXPECT_VALUES_EQUAL(vec[2], 3.0f);
    EXPECT_VALUES_EQUAL(vec[3], 4.0f);
    Math::Vector<int, 4> vec2({1, 2, 3, 4});
    EXPECT_EXPRESSION_THROW_TYPE(([&]{
        vec.Cross(vec2);
    }), std::invalid_argument);
    vec[3] = 0.0f;
    EXPECT_EXPRESSION_THROW_TYPE(([](){
        Math::Vector<float, 4> vec3({1.0f, 2.0f, 3.0f});
    }), std::invalid_argument);
    vec2[3] = 0;
    EXPECT_VALUES_EQUAL(vec.Cross(vec2), (Math::Vector<float, 4>({0.0f, 0.0f, 0.0f, 0.0f})));
}
void UnitTest::TestVector1()
{

    class B
    {};
    class A
    {
    public:
        inline operator std::string() const
        {
            return "A";
        }
        constexpr bool operator ==(const A& p_other) const
        {
            return true;
        }
        constexpr bool operator !=(const B& p_other) const
        {
            return true;
        }
    };
    Math::Vector<A, 1> vec;
    EXPECT_VALUES_EQUAL(vec[0], A());
    EXPECT_VALUES_EQUAL((vec ==Math::Vector<A, 1>({A()})), true);
    EXPECT_VALUES_EQUAL((vec !=Math::Vector<B, 1>({B()})), true);
    EXPECT_VALUES_EQUAL((has_times<A, A>), false);
    EXPECT_VALUES_EQUAL((has_times<float, float>), true);
    EXPECT_STRINGS_EQUAL(std::string(vec), "Vector<" + std::string(typeid(A).name()) + ", 1>(A)");
    EXPECT_EXPRESSION_THROW_TYPE(([&](){vec.Length();}), std::domain_error);
}
void UnitTest::TestVector2()
{
    Math::Vector<int, 2> vec({3, 4});
    EXPECT_VALUES_EQUAL(vec.Length(), 5);
    EXPECT_VALUES_EQUAL((vec +Math::Vector<int, 2>({3, 5})), (Math::Vector<int, 2>({6, 9})));
    EXPECT_VALUES_EQUAL((vec +Math::Vector<float, 2>({3.0f, 5.0f})), (Math::Vector<int, 2>({6, 9})));
    vec.Normalize();
    EXPECT_VALUES_EQUAL(vec, (Math::Vector<int, 2>({0, 0})));
    EXPECT_VALUES_EQUAL(vec.Normalize(), (Math::Vector<int, 2>({0, 0})));
    EXPECT_STRINGS_EQUAL(std::string(vec), "Vector<int, 2>(0, 0)");
}
void UnitTest::TestVector3()
{
    EXPECT_EXPRESSION_THROW_TYPE(([](){
        Math::Vector<float, 2> vec({3.0f, 4.0f, 5.0f});
    }), std::invalid_argument);
}

void UnitTest::TestMatrix0()
{
     Math::Mat4 mat;
    EXPECT_VALUES_EQUAL(mat[0][0], 1.0f);
    EXPECT_VALUES_EQUAL((mat[0, 1]), 0.0f);
    EXPECT_VALUES_EQUAL((mat[0, 2]), 0.0f);
    EXPECT_VALUES_EQUAL(mat[0][3], 0.0f);
    EXPECT_VALUES_EQUAL((mat[1, 0]), 0.0f);
    EXPECT_VALUES_EQUAL(mat[1][1], 1.0f);
    EXPECT_VALUES_EQUAL((mat[1, 2]), 0.0f);
    EXPECT_VALUES_EQUAL(mat[1][3], 0.0f);
    EXPECT_VALUES_EQUAL((mat[2, 0]), 0.0f);
    EXPECT_VALUES_EQUAL(mat[2][1], 0.0f);
    EXPECT_VALUES_EQUAL(mat[2][2], 1.0f);
    EXPECT_VALUES_EQUAL((mat[2, 3]), 0.0f);

    EXPECT_VALUES_EQUAL(mat, 
        (Math::Mat4({1.0f, 0.0f, 0.0f, 0.0f,
               0.0f, 1.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 1.0f, 0.0f,
               0.0f, 0.0f, 0.0f, 1.0f})));
}
void UnitTest::TestMatrix1()
{
     Math::Mat4 mat = Math::Mat4({
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        -1.0f, -2.0f, -3.0f, -4.0f,
        -5.0f, -6.0f, -7.0f, -8.0f
    });
     Math::Mat4 mat2 = Math::Mat4({
        1.0f, 3.0f, 5.0f, 7.0f,
        2.0f, 4.0f, 6.0f, 8.0f,
        -1.0f, -2.0f, -3.0f, -4.0f,
        -5.0f, -6.0f, -7.0f, -8.0f
    });

    EXPECT_VALUES_EQUAL(
        mat + mat2,
        (Math::Mat4({
            2.0f, 5.0f, 8.0f, 11.0f,
            7.0f, 10.0f, 13.0f, 16.0f,
            -2.0f, -4.0f, -6.0f, -8.0f,
            -10.0f, -12.0f, -14.0f, -16.0f
        }))
    );
    
    EXPECT_VALUES_EQUAL(
        mat - mat2,
        (Math::Mat4({
            0.0f, -1.0f, -2.0f, -3.0f,
            3.0f, 2.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        }))
    );

     Math::Mat4 mat3 = mat;

    EXPECT_VALUES_EQUAL(
        mat += mat2,
        (Math::Mat4({
            2.0f, 5.0f, 8.0f, 11.0f,
            7.0f, 10.0f, 13.0f, 16.0f,
            -2.0f, -4.0f, -6.0f, -8.0f,
            -10.0f, -12.0f, -14.0f, -16.0f
        }))
    );

    EXPECT_VALUES_EQUAL(
        mat3 -= mat2,
        (Math::Mat4({
            0.0f, -1.0f, -2.0f, -3.0f,
            3.0f, 2.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        }))
    );

    EXPECT_VALUES_EQUAL(
        (mat3 *Math::Vector<int, 4>({1, 2, 3, 4})),
        (Math::Vector<int, 4>({-20, 10, 0, 0}))
    );
}
void UnitTest::TestMatrix2()
{
    class B {};
    class A
    {
    public:
        inline operator std::string() const
        {
            return "A";
        }
        constexpr bool operator ==(const A& p_other) const
        {
            return true;
        }
        constexpr bool operator !=(const B& p_other) const
        {
            return true;
        }
    };

    Math::Matrix<A, 2, 2> mat;

    EXPECT_VALUES_EQUAL(mat, 
        (Math::Matrix<A, 2, 2>({
            A(), A(),
            A(), A()
        })));

    EXPECT_VALUES_EQUAL((mat != 
        (Math::Matrix<B, 2, 2>({
            B(), B(),
            B(), B()
        }))), true);
}
void UnitTest::TestMatrix3()
{
     Math::Mat4 mat = Math::Mat4({
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        -1.0f, -2.0f, -3.0f, -4.0f,
        -5.0f, -6.0f, -7.0f, -8.0f
    });

    EXPECT_VALUES_EQUAL(
        mat * 2,
        (Math::Mat4({
            2.0f, 4.0f, 6.0f, 8.0f,
            10.0f, 12.0f, 14.0f, 16.0f,
            -2.0f, -4.0f, -6.0f, -8.0f,
            -10.0f, -12.0f, -14.0f, -16.0f
        }))
    );

     Math::Mat4 mat2 = Math::Mat4({
        1.0f, 3.0f, 5.0f, 7.0f,
        2.0f, 4.0f, 6.0f, 8.0f,
        -1.0f, -2.0f, -3.0f, -4.0f,
        -5.0f, -6.0f, -7.0f, -8.0f
    });

    EXPECT_VALUES_EQUAL(
        mat * mat2,
        (Math::Mat4({
            -18, -19, -20, -21, 
            -30, -23, -16, -9, 
            18, 19, 20, 21, 
            30, 23, 16, 9
        }))
    );

    EXPECT_VALUES_EQUAL(
        mat *= mat2,
        (Math::Mat4({
            -18, -19, -20, -21, 
            -30, -23, -16, -9, 
            18, 19, 20, 21, 
            30, 23, 16, 9
        }))
    );

    EXPECT_VALUES_EQUAL(
        mat2 *= 2,
        (Math::Mat4({
            2.0f, 6.0f, 10.0f, 14.0f,
            4.0f, 8.0f, 12.0f, 16.0f,
            -2.0f, -4.0f, -6.0f, -8.0f,
            -10.0f, -12.0f, -14.0f, -16.0f
        }))
    );

    EXPECT_VALUES_EQUAL(
        2 * mat2,
        (Math::Matrix<float, 4, 4>({
            4, 12, 20, 28,
            8, 16, 24, 32,
            -4, -8, -12, -16,
            -20, -24, -28, -32
        }))
    );
}

void UnitTest::TestTransformation0()
{
    Math::Vec4 vector(1.0f, 2.0f, 3.0f, 1.0f);
    EXPECT_VALUES_EQUAL(Math::Trans(3.0f, 1.0f, 2.0f) * vector, (Math::Vec4(4.0f, 3.0f, 5.0f, 1.0f)));
    EXPECT_VALUES_EQUAL(Math::Trans(-20.0f, 3.0f, 0.0f) * vector, (Math::Vec4(-19.0f, 5.0f, 3.0f, 1.0f)));
    EXPECT_VALUES_EQUAL(Math::Scale(3.0f, 1.0f, 2.0f) * vector, (Math::Vec4(3.0f, 2.0f, 6.0f, 1.0f)));
    EXPECT_VALUES_EQUAL(Math::Scale(-20.0f, 3.0f, 0.0f) * vector, (Math::Vec4(-20.0f, 6.0f, 0.0f, 1.0f)));
    auto temp_vec = Math::Pitch(3.0f) * vector;
    EXPECT_VALUES_EQUAL(temp_vec[0], 1.0f);
    EXPECT_VALUES_EQUAL(temp_vec[1], -1.556625f);
    EXPECT_VALUES_EQUAL(temp_vec[2], -3.252217f);
    EXPECT_VALUES_EQUAL(temp_vec[3], 1.0f);
    temp_vec = Math::Yaw(3.0f) * vector;
    EXPECT_VALUES_EQUAL(temp_vec[0], -1.4133f);
    EXPECT_VALUES_EQUAL(temp_vec[1], 2.0f);
    EXPECT_VALUES_EQUAL(temp_vec[2], -2.8288f);
    EXPECT_VALUES_EQUAL(temp_vec[3], 1.0f);
    temp_vec = Math::Roll(3.0f) * vector;
    EXPECT_VALUES_EQUAL(temp_vec[0], -0.7077f);
    EXPECT_VALUES_EQUAL(temp_vec[1], -2.1211f);
    EXPECT_VALUES_EQUAL(temp_vec[2], 3.0f);
    EXPECT_VALUES_EQUAL(temp_vec[3], 1.0f);
}
void UnitTest::TestTransformation1()
{
    Math::Vec4 direction(1.0f, 2.0f, 3.0f, 0.0f);
    EXPECT_VALUES_EQUAL(Math::Trans(3.0f, 1.0f, 2.0f) * direction, direction);
    EXPECT_VALUES_EQUAL(Math::Trans(-20.0f, 3.0f, 0.0f) * direction, direction);
}
void UnitTest::TestTransformation2()
{
    Math::Vec4 vector(1.0f, 2.0f, 3.0f, 1.0f);
    EXPECT_VALUES_EQUAL(Math::ProjOrtho(1.0f, -1.0f, 1.0f, -1.0f, 2.0f, 1.0f) * vector, (Math::Vec4(1.0f, 2.0f, -3.0f, 1.0f)));
    vector = {0.0f, 1.0f, 1.0f, 1.0f};
    EXPECT_VALUES_EQUAL(Math::ProjOrtho(1.0f, -1.0f, 1.0f, -1.0f, 2.0f, 1.0f) * vector, (Math::Vec4(0.0f, 1.0f, 1.0f, 1.0f)));
}
void UnitTest::TestTransformation3()
{
    //todo: Proj matrix test
}
void UnitTest::TestLerp0()
{
    EXPECT_VALUES_EQUAL(Math::Lerp(0.0f, 1.0f, 0.5f), 1.0f);
    EXPECT_VALUES_EQUAL(Math::Lerp(0.5f, 1.0f, 0.0f), 0.5f);
    EXPECT_VALUES_EQUAL(Math::Lerp(0.5, 1.25, 2.5), 1.875);
}
void UnitTest::TestLerp1()
{
    EXPECT_VALUES_EQUAL((Math::Lerp(0.2f, Math::Vec4(1.0f, 1.0f, 2.0f, 1.0f), Math::Vec4(4.0f, 4.0f, 4.0f, 4.0f))), Math::Vec4(1.6f, 1.6f, 2.4f, 1.6f));
    auto result = Lerp(Math::Vec4(0.1, 0.2, 0.3, 0.0), Math::Vec4(1.0f, 2.0f, 3.0f, 1.0f), Math::Vec4(4.0f, -4.0f, 4.0f, 4.0f));
    EXPECT_VALUES_EQUAL(result[0], 1.3f);
    EXPECT_VALUES_EQUAL(result[1], 0.8f);
    EXPECT_VALUES_EQUAL(result[2], 3.3f);
    EXPECT_VALUES_EQUAL(result[3], 1.0f);
}
void UnitTest::TestLerp2()
{
    
}
void UnitTest::TestLerp3()
{
    
}