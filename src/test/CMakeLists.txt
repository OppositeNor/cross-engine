add_subdirectory(unit_test)
add_subdirectory(test_math)

set(CE_TEST_SOURCES
    ${CE_TEST_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp
    PARENT_SCOPE
)
