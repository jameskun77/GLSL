cmake_minimum_required(VERSION 2.8)

project(LearnOpengl)

configure_file(
	"${PROJECT_SOURCE_DIR}/configuration/root_directory.h.in"
	"${PROJECT_SOURCE_DIR}/includes/root_directory.h"
	)
	
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(SOURCE_DIR src)

set(STEP1_DRAWRECT_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step1_drawRect/main.cpp)
	
set(STEP1_DRAWCUBE_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step1_drawCube/main.cpp)

set(STEP1_CAMERACLASS_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step1_cameraClass/main.cpp)
	
set(STEP2_LIGHTDIFFUSE_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step2_lightDiffuse/main.cpp)

set(STEP2_LIGHTTEXTURE_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step2_lightTexture/main.cpp)

message(STATUS "Current directory: " ${CMAKE_SOURCE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/includes)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(DrawRect ${STEP1_DRAWRECT_SOURCE_LIST})

add_executable(DrawCube ${STEP1_DRAWCUBE_SOURCE_LIST})

add_executable(CameraClass ${STEP1_CAMERACLASS_SOURCE_LIST})

add_executable(LightPhongModel ${STEP2_LIGHTDIFFUSE_SOURCE_LIST})

add_executable(LightTexture ${STEP2_LIGHTTEXTURE_SOURCE_LIST})

target_link_libraries(DrawRect glfw3.lib)

target_link_libraries(DrawCube glfw3.lib)

target_link_libraries(CameraClass glfw3.lib)

target_link_libraries(LightPhongModel glfw3.lib)

target_link_libraries(LightTexture glfw3.lib)