set(CE_SOURCES
    ${CE_SOURCES}
    ${PROJECT_SOURCE_DIR}/include/ce/event/event.h
    ${PROJECT_SOURCE_DIR}/include/ce/event/window_event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window_event.cpp
    
    ${PROJECT_SOURCE_DIR}/include/ce/event/i_event_listener.h
    ${CMAKE_CURRENT_SOURCE_DIR}/i_event_listener.cpp
    PARENT_SCOPE)