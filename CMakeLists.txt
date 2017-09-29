cmake_minimum_required(VERSION 2.8)

project(LearnOpengl)

#set(VertexShaderPath "\"resources/shader/vertexShader\"")
#set(FragmentShaderPath "\"resources/shader/fragmentShader\"")

#configure_file(
#	"${PROJECT_SOURCE_DIR}/configuration/shaderPathConfig.h.in"
#	"${PROJECT_SOURCE_DIR}/includes/shaderPathConfig.h"
#	)
	
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(SOURCE_DIR src)

set(SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/main.cpp)

message(STATUS "Current directory: " ${CMAKE_SOURCE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/includes)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(DrawRectangle ${SOURCE_LIST})

target_link_libraries(DrawRectangle glfw3.lib)