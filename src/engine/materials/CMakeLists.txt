
set(CE_SOURCES
    ${CE_SOURCES}
    ${PROJECT_SOURCE_DIR}/include/ce/materials/material.h
    ${PROJECT_SOURCE_DIR}/include/ce/materials/pbr_material.h
    ${CMAKE_CURRENT_SOURCE_DIR}/material.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pbr_material.cpp
    PARENT_SCOPE)