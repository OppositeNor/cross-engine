
set(CE_SOURCES
    ${CE_SOURCES}
    ${PROJECT_SOURCE_DIR}/include/ce/managers/input_manager.h
    ${PROJECT_SOURCE_DIR}/include/ce/managers/event_manager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/input_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event_manager.cpp
    PARENT_SCOPE)