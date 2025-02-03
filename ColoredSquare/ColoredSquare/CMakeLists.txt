#/////////////////////////////////////////////////////////////////////

cmake_path( GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR )
set( DEPENDENCIES_DIR ${PARENT_DIR}/Dependencies )

set( GLFW_SRC_DIR ${DEPENDENCIES_DIR}/glfw-3.4 )
set( GLAD_SRC_DIR ${DEPENDENCIES_DIR}/glad )
set( IMGUI_SRC_DIR ${DEPENDENCIES_DIR}/imgui-1.9 )
set( NLOHMANN_HEADER_DIR ${DEPENDENCIES_DIR}/nlohmann )

add_executable ( ${PROJECT_NAME} "ColoredSquare.cpp" "ColoredSquare.h" "main.cpp"  "DefineConstants.h"   )

target_compile_features( ${PROJECT_NAME} PRIVATE cxx_std_17 )

target_link_libraries( ${PROJECT_NAME} PRIVATE glfw )
target_link_libraries( ${PROJECT_NAME} PRIVATE imgui )
target_link_libraries( ${PROJECT_NAME} PRIVATE cpr::cpr )

target_include_directories( ${PROJECT_NAME} PRIVATE
	${DEPENDENCIES_DIR}
	${GLFW_SRC_DIR}/include
	${GLAD_SRC_DIR}
	${IMGUI_SRC_DIR}
	${NLOHMANN_HEADER_DIR}
)

if ( CMAKE_VERSION VERSION_GREATER 3.12 )
  set_property( TARGET ColoredSquare PROPERTY CXX_STANDARD 20 )
endif()

#/////////////////////////////////////////////////////////////////////