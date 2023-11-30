cmake_minimum_required(VERSION 3.15)

project(CrossroadEngine C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/exe/${CMAKE_SYSTEM_NAME})
file (MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

include_directories(${PROJECT_SOURCE_DIR}/include)


if (WIN32_STARTUP)
    add_definitions(-DWIN32_MAIN)
endif()

add_subdirectory(src/test)
add_subdirectory(src/math)
add_subdirectory(src/user)
add_subdirectory(src/game)
add_subdirectory(src/component)
add_subdirectory(src/geometry)
add_subdirectory(src/graphics)
add_subdirectory(src/resource)
add_subdirectory(src/managers)
add_subdirectory(src/event)

add_subdirectory(shaders)

include_directories(
    ${PROJECT_SOURCE_DIR}/dependencies/glfw/windows/64/include
    ${PROJECT_SOURCE_DIR}/dependencies/glad/include)

add_library(glfw3dll SHARED IMPORTED)
set_property(TARGET glfw3dll PROPERTY IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/dependencies/glfw/windows/64/lib-vc2022/glfw3dll.lib)
set_property(TARGET glfw3dll PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/dependencies/glfw/windows/64/lib-vc2022/glfw3.dll)
set(libs glfw3dll)



set (CE_SOURCES
    ${CE_SOURCES}
    ${PROJECT_SOURCE_DIR}/dependencies/glad/src/glad.c
)

if (WIN32_STARTUP)
    set (CE_USER_SOURCES
        WIN32
        ${CE_USER_SOURCES}
    )
endif()

add_executable(CrossroadEngineUnitTest
    ${CE_SOURCES}
    ${CE_TEST_SOURCES})
    
add_executable(CrossroadEngineApplication
    ${CE_USER_SOURCES}
    ${CE_SOURCES}
)

add_custom_command(TARGET CrossroadEngineApplication POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:glfw3dll> $<TARGET_FILE_DIR:CrossroadEngineApplication>
)

target_link_libraries(CrossroadEngineUnitTest PUBLIC
    ${libs})

target_link_libraries(CrossroadEngineApplication PUBLIC
    ${libs})
