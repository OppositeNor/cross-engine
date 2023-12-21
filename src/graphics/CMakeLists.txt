add_subdirectory(shader)
set(CE_SOURCES
    ${CE_SOURCES}
    ${PROJECT_SOURCE_DIR}/include/ce/graphics/window.h
    ${PROJECT_SOURCE_DIR}/include/ce/graphics/graphics.h
    ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics.cpp
    PARENT_SCOPE)