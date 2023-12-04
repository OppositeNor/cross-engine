set(CE_SOURCES
    ${CE_SOURCES}
    ${PROJECT_SOURCE_DIR}/include/ce/geometry/a_geometry.h
    ${PROJECT_SOURCE_DIR}/include/ce/geometry/vertex.h
    ${PROJECT_SOURCE_DIR}/include/ce/geometry/triangle.h
    ${PROJECT_SOURCE_DIR}/include/ce/geometry/polygon.h

    ${CMAKE_CURRENT_SOURCE_DIR}/a_geometry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/triangle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/polygon.cpp
    PARENT_SCOPE)