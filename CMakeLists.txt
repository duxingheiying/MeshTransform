cmake_minimum_required(VERSION 3.18)
project(MeshTransform LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    #add_compile_options(/W4 /permissive-)
else()
    #add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(MESH_DIR ${SRC_DIR}/mesh)

set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/obj_file.cpp
    ${MESH_DIR}/mesh.cpp
    ${MESH_DIR}/transform.cpp
)

set(HEADERS
    ${SRC_DIR}/obj_file.h
    ${MESH_DIR}/mesh.h
    ${MESH_DIR}/transform.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

#target_include_directories(${PROJECT_NAME} PRIVATE ${MESH_DIR})

if (UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

# force use utf-8
if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif(MSVC)

message(STATUS "---------------------------------------")
message(STATUS "Project:        ${PROJECT_NAME}")
message(STATUS "C++ Standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "Output Binary:  ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}")
message(STATUS "---------------------------------------")
