#include "../unit_test/unit_test.h"
#include "ce/resource/resource.h"

void UnitTest::TestLoadFile0()
{
    auto data = Resource::LoadFile(Resource::GetExeDirectory() + "/test_file.txt");
    const char* expected = "This is a test file.";
    for (size_t i = 0; i < strlen(expected); ++i)
        EXPECT_VALUES_EQUAL(data[i], expected[i]);
    delete data;
}
void UnitTest::TestLoadFile1()
{
    byte_t buff[256];
    size_t size = 0;
    Resource::LoadFile(Resource::GetExeDirectory() + "/some_binary_file", buff, 256, size);
    EXPECT_VALUES_EQUAL(size, 75);
    EXPECT_VALUES_EQUAL((int)buff[0], 0x11);
    EXPECT_VALUES_EQUAL((int)buff[1], 0x22);
    EXPECT_VALUES_EQUAL((int)buff[2], 0x33);
    EXPECT_STRINGS_EQUAL((buff + 13), "I am a monster. I will eat all of you.");
    EXPECT_VALUES_EQUAL((int)buff[size - 1], 0x05);
}
void UnitTest::TestLoadFile2()
{
    byte_t buff[3];
    size_t size = 0;
    Resource::LoadFile(Resource::GetExeDirectory() + "/some_binary_file", buff, 3, size);
    EXPECT_VALUES_EQUAL(size, 3);
    EXPECT_VALUES_EQUAL((int)buff[0], 0x11);
    EXPECT_VALUES_EQUAL((int)buff[1], 0x22);
    EXPECT_VALUES_EQUAL((int)buff[2], 0x33);
}
void UnitTest::TestLoadFile3()
{
    
}

void UnitTest::TestLoadTris0()
{
    auto result = Resource::LoadTris(Resource::GetExeDirectory() + "/teapot_bezier0.tris");
    EXPECT_VALUES_EQUAL(result[0]->GetVertex(0)->GetPosition(), Math::Vec4f(0.000000, 2.250000, 1.500000, 1.0));
    EXPECT_STRINGS_EQUAL((std::string)result[256]->GetVertex(2)->GetPosition(), (std::string)Math::Vec4f(1.39922, 2.35664, 0, 1));
    EXPECT_STRINGS_EQUAL((std::string)result[3487]->GetVertex(0)->GetPosition(), (std::string)Math::Vec4f(0.000000, 2.550000, -0.200000, 1.0));
    EXPECT_STRINGS_EQUAL((std::string)result[3487]->GetVertex(1)->GetPosition(), (std::string)Math::Vec4f(0.056000, 2.550000, -0.200000, 1.0));
    EXPECT_STRINGS_EQUAL((std::string)result[3487]->GetVertex(2)->GetPosition(), (std::string)Math::Vec4f(0.000000, 2.533333, -0.222222, 1.0));
    for (auto i : result)
        delete i;
}



