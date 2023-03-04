cmake_minimum_required(VERSION 3.12)
project(rayz)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pg")

# message(${CMAKE_CXX_FLAGS_DEBUG})

add_subdirectory(libs)

add_compile_definitions(GLEW_STATIC)

find_package(TBB REQUIRED)
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

add_executable(
    ${PROJECT_NAME}
    main.cpp
    src/objects/plane.cpp
    src/objects/sphere.cpp
    src/objects/triangle.cpp
    src/textures/checkerTexture.cpp
    src/textures/imageTexture.cpp
    src/textures/solidColorTexture.cpp
    src/materials/lambertian.cpp
    src/materials/diffuseLight.cpp
    src/materials/metal.cpp
    src/materials/dielectric.cpp
    src/boundingBox.cpp
    src/camera.cpp
    src/hittable.cpp
    src/material.cpp
    src/renderer.cpp
    src/scene.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    ${GLFW_LIBRARIES} 
    ${GLEW_LIBRARIES} 
    ${OPENGL_LIBRARIES}
    jug
    TBB::tbb
)

add_custom_target(
    copy_textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_CURRENT_LIST_DIR}/textures 
    ${CMAKE_CURRENT_BINARY_DIR}/textures
)

add_custom_target(
    copy_jug_assests
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_CURRENT_LIST_DIR}/libs/jug/assests 
    ${CMAKE_CURRENT_BINARY_DIR}/assests
)

add_dependencies(
    ${PROJECT_NAME}
    copy_textures
    copy_jug_assests
)
