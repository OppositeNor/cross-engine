
set(CE_SOURCES
    ${CE_SOURCES}
    ${PROJECT_SOURCE_DIR}/include/ce/component/component.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/visual_mesh.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/dynamic_mesh.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/camera.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/light.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/point_light.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/parallel_light.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/skybox.h
    ${PROJECT_SOURCE_DIR}/include/ce/component/component3D.h

    ${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/visual_mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/light.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/point_light.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parallel_light.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/skybox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/component3D.cpp
    PARENT_SCOPE)