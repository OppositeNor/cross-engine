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
    


    if (test_failed_count == 0)
        std::cout << "Unit test finished. " << test_count <<" test run, All tests passed.\n";
    else
        std::cout << "Unit test finished. " << test_count << " tests run, " << test_failed_count << " tests failed.\n";
}

