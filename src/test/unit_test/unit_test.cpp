#include "unit_test.h"

size_t UnitTest::test_count = 0;
size_t UnitTest::test_failed_count = 0;

void UnitTest::CheckExpect(bool p_condition, const std::string& p_message, const char* p_file, int p_line)
{
    ++test_count;
    if (!p_condition)
    {
        std::cout << "Test failed at file: " << p_file << ":" << p_line << ".\nOutput: " << p_message << '\n';
        ++test_failed_count;
    }
}

void UnitTest::Start()
{
    std::cout << "Running unit test..." << '\n';

    RUN_TEST(TestVector0);
    RUN_TEST(TestVector1);
    RUN_TEST(TestVector2);
    RUN_TEST(TestVector3);

    RUN_TEST(TestMatrix0);
    RUN_TEST(TestMatrix1);
    RUN_TEST(TestMatrix2);
    RUN_TEST(TestMatrix3);

    RUN_TEST(TestTransformation0);
    RUN_TEST(TestTransformation1);
    RUN_TEST(TestTransformation2);
    RUN_TEST(TestTransformation3);

    RUN_TEST(TestLerp0);
    RUN_TEST(TestLerp1);
    RUN_TEST(TestLerp2);
    RUN_TEST(TestLerp3);

    RUN_TEST(TestVertex0);
    RUN_TEST(TestVertex1);
    RUN_TEST(TestVertex2);
    RUN_TEST(TestVertex3);
    RUN_TEST(TestVertex4);
    
    RUN_TEST(TestTriangle0);
    RUN_TEST(TestTriangle1);
    RUN_TEST(TestTriangle2);
    RUN_TEST(TestTriangle3);

    RUN_TEST(TestLoadFile0);
    RUN_TEST(TestLoadFile1);
    RUN_TEST(TestLoadFile2);
    RUN_TEST(TestLoadFile3);

    RUN_TEST(TestLoadTris0);

    RUN_TEST(TestComponent0);
    RUN_TEST(TestComponent1);
    RUN_TEST(TestComponent2);
    RUN_TEST(TestComponent3);
    RUN_TEST(TestComponent4);
    
    
    


    if (test_failed_count == 0)
        std::cout << "Unit test finished. " << test_count <<" test run, All tests passed.\n";
    else
        std::cout << "Unit test finished. " << test_count << " tests run, " << test_failed_count << " tests failed.\n";
}

