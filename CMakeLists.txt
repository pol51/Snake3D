CMake_Minimum_Required(VERSION 2.6)

Project(Snake3D)
Set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

Include_Directories(${SDL_INCLUDE_DIR} src)

Link_Libraries (glut GL GLU)

File(GLOB_RECURSE source_files src/*)

Add_Executable(Snake3D ${source_files})
