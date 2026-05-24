cmake_minimum_required(VERSION 3.20)
project(BuanGDPHYSX LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 3.4 REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)
target_link_libraries(glad PRIVATE OpenGL::GL)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/particle.cpp
    src/vector.cpp
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/3D $<TARGET_FILE_DIR:${PROJECT_NAME}>/3D
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    glad
    glm::glm
)