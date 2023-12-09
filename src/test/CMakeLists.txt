add_subdirectory(unit_test)
add_subdirectory(test_math)
add_subdirectory(test_geometry)
add_subdirectory(test_component)
add_subdirectory(test_resource)

file(COPY ${PROJECT_SOURCE_DIR}/model/teapot_bezier0.tris DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${PROJECT_SOURCE_DIR}/model/teapot_bezier0.norm DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${PROJECT_SOURCE_DIR}/model/teapot_bezier2.norm DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${PROJECT_SOURCE_DIR}/model/monkey.obj DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${PROJECT_SOURCE_DIR}/model/arm.obj DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${PROJECT_SOURCE_DIR}/textures/default.png DESTINATION ${EXECUTABLE_OUTPUT_PATH})

set(CE_TEST_SOURCES
    ${CE_TEST_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp
    PARENT_SCOPE
)
