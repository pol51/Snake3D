cmake_minimum_required(VERSION 3.10)

project(Snake3D)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${SDL_INCLUDE_DIR} src)

file(GLOB_RECURSE source_files src/*)

add_executable(${PROJECT_NAME} ${source_files})
target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL OpenGL::GLU GLUT::GLUT)
