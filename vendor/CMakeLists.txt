add_subdirectory(glfw)

add_subdirectory(glad/cmake)
glad_add_library(glad STATIC
    REPRODUCIBLE
    API gl:core=4.6
    LANGUAGE c
    EXTENSIONS GL_ARB_direct_state_access
)

add_subdirectory(glm)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb
)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE glfw glad glm)
target_include_directories(
    vendor INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/stb
)