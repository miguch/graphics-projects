# Miguel Chan - 3/5/2019
cmake_minimum_required(VERSION 3.10)
project(IMGUI)
set(CMAKE_CXX_STANDARD 14)

set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(IMGUI_HEADER_DIR ${IMGUI_INCLUDE_DIR}/imgui)

#for glad
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

FILE(GLOB IMGUI_SOURCE ${IMGUI_SOURCE_DIR}/*.cpp ${IMGUI_SOURCE_DIR}/*.c)
FILE(GLOB IMGUI_HEADER ${IMGUI_HEADER_DIR}/*.h ${IMGUI_HEADER_DIR}/*.hpp)
add_library(imgui ${IMGUI_SOURCE} ${IMGUI_HEADER})
target_include_directories(imgui PRIVATE ${IMGUI_HEADER_DIR})
target_include_directories(imgui INTERFACE ${IMGUI_INCLUDE_DIR})
target_link_libraries(imgui glad)
target_link_libraries(imgui glfw)
