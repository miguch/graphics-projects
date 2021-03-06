# Miguel Chan - 2/28/2019
cmake_minimum_required(VERSION 3.10)
project(Camera)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

FILE(GLOB_RECURSE SOURCE ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.c)
FILE(GLOB_RECURSE HEADER ${SOURCE_DIR}/*.h ${SOURCE_DIR}/*.hpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/imgui)
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/GLM)
add_subdirectory(dependencies/GLFW)
include_directories(dependencies/GLFW/include)
FILE(GLOB DEPENDENCIES_SOURCE dependencies/src/*.c dependencies/src/*.cpp)
FILE(GLOB ALL_SOURCES ${SOURCE} ${HEADER} ${DEPENDENCIES_SOURCE})

add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

if (APPLE)
    set(os macos)
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif(APPLE)

if (UNIX AND NOT APPLE)
    set(os linux)
endif(UNIX AND NOT APPLE)

if (WIN32)
    set(os windows)
    if (MSVC)
        add_compile_definitions(NDEBUG)
    endif(MSVC)
endif(WIN32)

set(EXECUTABLE_NAME ${PROJECT_NAME}-${os})

add_executable(${EXECUTABLE_NAME} ${ALL_SOURCES})

# copy resources and shaders after build
add_custom_target(copy-resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/src/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)


#FIND_PACKAGE(OpenGL REQUIRED)
#if(OpenGL_FOUND)
#    link_directories(${OpenGL_LIBRARY_DIRS})
#    include_directories(${OpenGL_INCLUDE_DIR})
#    target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
#endif(OpenGL_FOUND)

target_link_libraries(${EXECUTABLE_NAME} glfw)
target_link_libraries(${EXECUTABLE_NAME} imgui)
target_link_libraries(${EXECUTABLE_NAME} glad)
target_link_libraries(${EXECUTABLE_NAME} glm)
add_dependencies(${EXECUTABLE_NAME} copy-resources)

#FIND_PACKAGE(GLUT REQUIRED)
#if(GLUT_FOUND)
#    link_directories(${GLUT_LIBRARY_DIR})
#    include_directories(${GLUT_INCLUDE_DIR})
#    target_link_libraries(${EXECUTABLE_NAME} ${GLUT_LIBRARIES})
#endif(GLUT_FOUND)

#find_package(GLEW REQUIRED)
#if (GLEW_FOUND)
#    include_directories(${GLEW_INCLUDE_DIRS})
#    target_link_libraries(${EXECUTABLE_NAME} ${GLEW_LIBRARIES})
#endif()
