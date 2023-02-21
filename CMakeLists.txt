cmake_minimum_required(VERSION 3.12)
project(rayz)

option(BUILD_STATIC_LIBS "Build Static Library" ON)
option(BUILD_SHARED_LIBS "Build Shared Library" OFF)

add_subdirectory(libs)

add_compile_definitions(GLEW_STATIC)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)

include_directories(
    include
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ${SRC_FILES}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${GLFW_LIBRARIES} 
    ${GLEW_LIBRARIES} 
    ${OPENGL_LIBRARIES}
    jug
)
