cmake_minimum_required(VERSION 3.1)
project(glut_01)

# Project sources
add_executable(${PROJECT_NAME} main.cpp)

# Use OpenGL and GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(glut_01 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

# Use C++11 version of the standard
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)

# Place the output binary at the root of the build folder
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
