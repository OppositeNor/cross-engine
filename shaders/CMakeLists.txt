
file (MAKE_DIRECTORY ${EXE_PATH}/shaders)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/vertex.glsl DESTINATION ${EXE_PATH}/shaders)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fragment.glsl DESTINATION ${EXE_PATH}/shaders)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/skybox_vertex.glsl DESTINATION ${EXE_PATH}/shaders)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/skybox_fragment.glsl DESTINATION ${EXE_PATH}/shaders)