cmake_minimum_required(VERSION 3.5)

project(OpenGL_lab03 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCES
    main.cpp
    loadObj.cpp
)

set(HEADERS
    loadObj.hpp
)

add_executable(OpenGL_lab03 ${SOURCES} ${HEADERS})


INCLUDE_DIRECTORIES(${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS} )

target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)