cmake_minimum_required(VERSION 3.1)
project(run)

# Project sources
add_executable(${PROJECT_NAME} main.cpp Camera.cpp Mesh.cpp ShaderClass.cpp ShaderProgram.cpp)

# Use OpenGL, GLEW and GLUT
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

# GLM for math
add_subdirectory(./lib/glm)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} GLEW::GLEW glm::glm)
add_definitions(${GLM_DEFINITIONS})

# Use C++11 version of the standard
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)

# Place the output binary at the root of the build folder
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
