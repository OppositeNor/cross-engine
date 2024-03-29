cmake_minimum_required(VERSION 3.15)

project(Cross LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/exe/${CMAKE_SYSTEM_NAME})
file (MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

if (MSVC)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set (EXE_PATH "${EXECUTABLE_OUTPUT_PATH}/Debug")
    else()
        set (EXE_PATH "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}")
    endif()
else()
    set (EXE_PATH "${EXECUTABLE_OUTPUT_PATH}")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
option(WIN32_STARTUP "WIN32_STARTUP" OFF)

if (WIN32_STARTUP)
    add_definitions(-DWIN32_MAIN)
endif()

add_subdirectory(src/engine)
add_subdirectory(src/user)
add_subdirectory(src/test)

add_subdirectory(shaders)
add_subdirectory(textures)
add_subdirectory(test_resource)

include_directories(
    ${PROJECT_SOURCE_DIR}/dependencies/glfw/windows/64/include
    ${PROJECT_SOURCE_DIR}/dependencies/glad/include
    ${PROJECT_SOURCE_DIR}/dependencies/stb)

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

    
add_executable(Application
    ${CE_USER_SOURCES}
    ${CE_SOURCES}
)
    
add_executable(Test
    ${CE_TEST_SOURCES}
    ${CE_SOURCES}
)

add_custom_command(TARGET Application POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:glfw3dll> $<TARGET_FILE_DIR:Application>
)

add_custom_command(TARGET Application POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:glfw3dll> $<TARGET_FILE_DIR:Test>
)

target_link_libraries(Application PUBLIC
    ${libs})
    
target_link_libraries(Test PUBLIC
    ${libs})
