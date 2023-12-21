#pragma once
#include "ce/defs.hpp"

#include <functional>
#include <iostream>
#include <cmath>
#include <sstream>

#define CHECK_EXPECT(p_condition, p_message) \
    try {\
        UnitTest::CheckExpect((p_condition), (p_message), __FILE__, __LINE__);\
    }\
    catch(std::exception& e) {\
        UnitTest::CheckExpect(false, "Check expect catched an error: " + std::string(e.what()), __FILE__, __LINE__);\
    }(void(0))

#define EXPECT_VALUES_EQUAL(val_1, val_2) \
    try {\
        UnitTest::ExpectValuesEqual(__FILE__, __LINE__,(val_1), (val_2));\
    }\
    catch(std::exception& e) {\
        UnitTest::CheckExpect(false, "Expect values equal catched an error: " + std::string(e.what()), __FILE__, __LINE__);\
    }(void(0))
#define EXPECT_STRINGS_EQUAL(str_1, str_2) \
    try {\
        UnitTest::ExpectStringsEqual(__FILE__, __LINE__, (str_1), (str_2));\
    }\
    catch(std::exception& e) {\
        UnitTest::CheckExpect(false, "Expect strings equal catched an error: " + std::string(e.what()), __FILE__, __LINE__);\
    }(void(0))
#define EXPECT_EXPRESSION_THROW(p_func) UnitTest::ExpectExpressionThrow(__FILE__, __LINE__, (p_func))
#define EXPECT_EXPRESSION_THROW_MSG(p_func, p_throw_msg) UnitTest::ExpectExpressionThrow(__FILE__, __LINE__, (p_func), (p_throw_msg))
#define EXPECT_EXPRESSION_THROW_TYPE(p_func, p_type) UnitTest::ExpectExpressionThrow<p_type>(__FILE__, __LINE__, (p_func))

#define RUN_TEST(p_func, ...) \
    try {\
        UnitTest::test_count++;\
        p_func(__VA_ARGS__);\
    }\
    catch(std::exception& e) {\
        UnitTest::test_failed_count++;\
        std::cerr << "Test throwed an exception at file: " << __FILE__ << ":" << __LINE__ << ".\n" << e.what() << '\n';\
    }(void(0))

/**
 * @brief Unit test class.
 * This class is used for unit testing.
 */
class UnitTest
{
public:

    /**
     * @brief Start the unit test.
     */
    static void Start();

    static constexpr float TOLARENCE = (float)0.0001;
private:

    /**
     * @brief The number of tests that have been run.
     */
    static size_t test_count;
    /**
     * @brief The number of tests that have failed.
     */
    static size_t test_failed_count;

    /**
     * @brief Check if the condition is true. If not, fail a test and print out a message.
     * @param p_message The message to print out.
     * @param p_condition The condition of the test.
     */
    static void CheckExpect(bool p_condition, const std::string& p_message, const char* p_file, int p_line);

    /**
     * @brief Check if two values are equal.
     * 
     * @tparam T1 Type of value 1.
     * @tparam T2 Type of value 2.
     * @param val_1 Value 1.
     * @param val_2 Value 2.
     */
    template <typename T1, typename T2>
    inline static auto ExpectValuesEqual(const char* p_file, int p_line, T1&& val_1, T2&& val_2)
        ->std::enable_if_t<std::is_convertible_v<std::remove_reference_t<T1>, std::string> &&
                           std::is_convertible_v<std::remove_reference_t<T2>, std::string>,
                           decltype((void)(val_1 == val_2), void())>
    {
        CheckExpect(val_1 == val_2, "Expected to get value: " + (std::string)(val_2) + 
            ", but get " + (std::string)(val_1) + " instead.", p_file, p_line);
    }

    /**
     * @brief Check if two values are equal.
     * 
     * @tparam T1 Type of value 1.
     * @tparam T2 Type of value 2.
     * @param val_1 Value 1.
     * @param val_2 Value 2.
     */
    template <typename T1, typename T2>
    inline static auto ExpectValuesEqual(const char* p_file, int p_line, T1&& val_1, T2&& val_2)
        ->std::enable_if_t<!std::is_convertible_v<std::remove_reference_t<T1>, std::string> &&
                           !std::is_convertible_v<std::remove_reference_t<T2>, std::string>,
                           decltype((void)(val_1 == val_2), (void)(std::declval<std::ostream>() << val_1 << val_2), void())>
    {
        std::stringstream ss;
        ss << "Expected to get value: " << val_2 << ", but get " << val_1 << " instead.";
        CheckExpect(val_1 == val_2, ss.str(), p_file, p_line);
    }

    inline static void ExpectValuesEqual(const char* p_file, int p_line, float val_1, float val_2)
    {
        CheckExpect(std::abs(val_1 - val_2) <= std::abs(TOLARENCE * val_2), "Expected to get value: " + std::to_string(val_2) + 
            ", but get " + std::to_string(val_1) + " instead.", p_file, p_line);
    }
    
    /**
     * @brief This function is called when the two values are not comparable or a KEString operator is missing.
     * 
     * @param ... 
     */
    inline static void ExpectValuesEqual(const char* p_file, int p_line, ...)
    {
        std::cerr << "Warning: " << p_file << ":" << p_line << "The value cannot be displayed or cannot be compared. The test is skipped.\n";
    }

    /**
     * @brief Check if two strings are equal.
     * 
     * @param p_str_1 The first string.
     * @param p_str_2 The second string.
     */
    inline static void ExpectStringsEqual(const char* p_file, int p_line, const std::string& p_str_1, const std::string& p_str_2)
    {
        CheckExpect(p_str_1 == p_str_2, "Expected to get string: \"" + p_str_2 + 
            "\", but get \"" + p_str_1 + "\" instead.", p_file, p_line);
    }
    /**
     * @brief Test if an expression throws an exception.
     * 
     * @param p_func The expression to test.
     */
    static void ExpectExpressionThrow(const char* p_file, int p_line, const std::function<void()>& p_func)
    {
        bool is_throw = false;
        try
        {
            p_func();
        }
        catch (...)
        {
            CheckExpect(true, "Passes.", p_file, p_line);
            return;
        }
        CheckExpect(true, "Expected to throw an exception, but not.", p_file, p_line);
    }
    /**
     * @brief Test if an expression throws an exception with a specific message.
     * 
     * @param p_func The expression to test.
     * @param p_throw_msg The expected message.
     */
    static void ExpectExpressionThrow(const char* p_file, int p_line, const std::function<void()>& p_func, const std::string& p_throw_msg)
    {
        bool is_throw = false;
        try
        {
            p_func();
        }
        catch (const std::exception& e)
        {
            CheckExpect(e.what() == p_throw_msg, "Expected to throw an exeption with message: " + p_throw_msg + 
                ", but get \"" + e.what() + "\" instead.", p_file, p_line);
            return;
        }
        catch (...)
        {
            CheckExpect(false, "Expected to throw an exeption with message: " + p_throw_msg + 
                ", but get an exception with unknown message instead.", p_file, p_line);
            return;
        }
        CheckExpect(is_throw, "Expected to throw an exception, but not.", p_file, p_line);
    }

    template <typename T>
    static void ExpectExpressionThrow(const char* p_file, int p_line, const std::function<void()>& p_func)
    {
        bool is_throw = false;
        try
        {
            p_func();
        }
        catch (const T& e)
        {
            CheckExpect(true, "Passes.", p_file, p_line);
            return;
        }
        catch (...)
        {
            CheckExpect(false, "Expected to throw an exeption with type: " + std::string(typeid(T).name()) + 
                ", but get an exception with other type instead.", p_file, p_line);
            return;
        }
        CheckExpect(is_throw, "Expected to throw an exception, but not.", p_file, p_line);
    }

    /** Math Test Start **/
    /**Math::Vector Test Start**/
    static void TestVector0();
    static void TestVector1();
    static void TestVector2();
    static void TestVector3();
    /**Math::Vector Test End **/
    /** Matrix Test Start **/
    static void TestMatrix0();
    static void TestMatrix1();
    static void TestMatrix2();
    static void TestMatrix3();
    /** Matrix Test End **/
    /** Transformation Test Start **/
    static void TestTransformation0();
    static void TestTransformation1();
    static void TestTransformation2();
    static void TestTransformation3();
    /** Transformation Test End **/
    /** Lerp Test Start **/
    static void TestLerp0();
    static void TestLerp1();
    static void TestLerp2();
    static void TestLerp3();
    /** Lerp Test End **/
    /** Math Test End **/

    /** Geometry Test Start **/
    /** Vertex Test Start**/
    static void TestVertex0();
    static void TestVertex1();
    static void TestVertex2();
    static void TestVertex3();
    static void TestVertex4();
    /** Vertex Test End **/
    /** Triangle Test Start**/
    static void TestTriangle0();
    static void TestTriangle1();
    static void TestTriangle2();
    static void TestTriangle3();
    /** Triangle Test End **/
    /** Geometry Test End **/

    /** Model Test Start **/
    static void TestTeapot();
    /** Model Test End **/

    /** Resource Test Start **/
    static void TestLoadFile0();
    static void TestLoadFile1();
    static void TestLoadFile2();
    static void TestLoadFile3();
    static void TestLoadTris0();
    /** Resource Test End **/


    /** Resource Test Start **/
    static void TestComponent0();
    static void TestComponent1();
    static void TestComponent2();
    static void TestComponent3();
    static void TestComponent4();
    /** Resource Test End **/
};