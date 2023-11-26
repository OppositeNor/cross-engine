set(CE_SOURCES
    ${CE_SOURCES}
    ${PROJECT_SOURCE_DIR}/include/ce/component/component.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/visual_mesh.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/dynamic_mesh.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/camera.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/light.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/point_light.h

    ${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/visual_mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/light.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/point_light.cpp
    PARENT_SCOPE)