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
	
set(STEP2_MUTIPLELIGHT_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step2_multipleLight/main.cpp)
	
set(LOADMESH_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/load_mesh/main.cpp)
	
set(DEPTHSTENCIL_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step3_depth_stencil/main.cpp)
	
set(BLEND_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step3_blend/main.cpp)
	
set(RENDERTEXTURE_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step3_renderTexture/main.cpp)
	
set(CUBETEXTURE_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step3_cubeTexture/main.cpp)
	
set(ENVIROMENTMAP_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step3_eviromentMap/main.cpp)
	
set(BLINNPHONG_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step4_blinn_phong/main.cpp)
	
set(SHADOWMAP_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step4_shadow_map/main.cpp)

set(NORMALMAP_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step4_normal_map/main.cpp)
	
set(HDR_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step4_hdr/main.cpp)
	
set(BLOOM_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step4_bloom/main.cpp)
	
set(DEFERREDRENDER_SOURCE_LIST ${SOURCE_DIR}/glad.cpp
	${SOURCE_DIR}/step4_deferred_render/main.cpp)
	
message(STATUS "Current directory: " ${CMAKE_SOURCE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/includes)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(DrawRect ${STEP1_DRAWRECT_SOURCE_LIST})

add_executable(DrawCube ${STEP1_DRAWCUBE_SOURCE_LIST})

add_executable(CameraClass ${STEP1_CAMERACLASS_SOURCE_LIST})

add_executable(LightPhongModel ${STEP2_LIGHTDIFFUSE_SOURCE_LIST})

add_executable(LightTexture ${STEP2_LIGHTTEXTURE_SOURCE_LIST})

add_executable(MultipleLight ${STEP2_MUTIPLELIGHT_SOURCE_LIST})

add_executable(LoadMesh ${LOADMESH_SOURCE_LIST})

add_executable(DepthStencilTest ${DEPTHSTENCIL_SOURCE_LIST})

add_executable(BlendTest ${BLEND_SOURCE_LIST})

add_executable(RenderTexture ${RENDERTEXTURE_SOURCE_LIST})

add_executable(CubeTexture ${CUBETEXTURE_SOURCE_LIST})

add_executable(EnviromentMap ${ENVIROMENTMAP_SOURCE_LIST})

add_executable(BlinnPhong ${BLINNPHONG_SOURCE_LIST})

add_executable(ShadowMap ${SHADOWMAP_SOURCE_LIST})

add_executable(NormalMap ${NORMALMAP_SOURCE_LIST})

add_executable(HDR ${HDR_SOURCE_LIST})

add_executable(Bloom ${BLOOM_SOURCE_LIST})

add_executable(DeferredRender ${DEFERREDRENDER_SOURCE_LIST})

target_link_libraries(DrawRect glfw3.lib)

target_link_libraries(DrawCube glfw3.lib)

target_link_libraries(CameraClass glfw3.lib)

target_link_libraries(LightPhongModel glfw3.lib)

target_link_libraries(LightTexture glfw3.lib)

target_link_libraries(MultipleLight glfw3.lib)

target_link_libraries(LoadMesh glfw3.lib)
target_link_libraries(LoadMesh assimp.lib)

target_link_libraries(DepthStencilTest glfw3.lib)

target_link_libraries(BlendTest glfw3.lib)

target_link_libraries(RenderTexture glfw3.lib)

target_link_libraries(CubeTexture glfw3.lib)

target_link_libraries(EnviromentMap glfw3.lib)

target_link_libraries(BlinnPhong glfw3.lib)

target_link_libraries(ShadowMap glfw3.lib)

target_link_libraries(NormalMap glfw3.lib)

target_link_libraries(HDR glfw3.lib)

target_link_libraries(Bloom glfw3.lib)

target_link_libraries(DeferredRender glfw3.lib)
target_link_libraries(DeferredRender assimp.lib)