#/////////////////////////////////////////////////////////////////////
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

#/////////////////////////////////////////////////////////////////////

project( "ColoredSquare" )
add_subdirectory( ColoredSquare )

set( DEPENDENCIES_DIR ${CMAKE_CURRENT_LIST_DIR}/Dependencies )

#/////////////////////////////////////////////////////////////////////
#// GLFW subdirectory

set( GLFW_SRC_DIR ${DEPENDENCIES_DIR}/glfw-3.4 )

# GLFW отключение ненужных модулей при компиляции
set( GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE )
set( GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE )
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )

add_subdirectory( ${GLFW_SRC_DIR} )

#/////////////////////////////////////////////////////////////////////
#// DearImGui + GLAD static library

set( IMGUI_SRC_DIR ${DEPENDENCIES_DIR}/imgui-1.9 )
set( IMGUI_PUBLIC_HEADERS ${IMGUI_SRC_DIR}/imgui.h )
set( GLAD_SRC_DIR ${DEPENDENCIES_DIR}/glad )

set( IMGUI_SOURCES
    ${IMGUI_SRC_DIR}/imgui.cpp
    ${IMGUI_SRC_DIR}/imgui_draw.cpp
    ${IMGUI_SRC_DIR}/imgui_demo.cpp
    ${IMGUI_SRC_DIR}/imgui_widgets.cpp
    ${IMGUI_SRC_DIR}/imgui_tables.cpp
    ${IMGUI_SRC_DIR}/imgui_impl_opengl3.cpp
    ${IMGUI_SRC_DIR}/imgui_impl_glfw.cpp
    ${GLAD_SRC_DIR}/glad.c
)

add_library( imgui STATIC ${IMGUI_SOURCES} )
target_include_directories( imgui PRIVATE ${DEPENDENCIES_DIR} ${GLFW_SRC_DIR}/include ${IMGUI_SRC_DIR} ${GLAD_SRC_DIR} )

#/////////////////////////////////////////////////////////////////////
#// CPR FetchContent

include( FetchContent )
FetchContent_Declare( cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG "1.11.1" )

FetchContent_MakeAvailable( cpr )

#/////////////////////////////////////////////////////////////////////
