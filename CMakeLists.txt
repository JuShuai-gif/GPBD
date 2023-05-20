cmake_minimum_required(VERSION 3.1)

project(PBD)

set(PROJECT_PATH ${PROJECT_SOURCE_DIR})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-idirafter${PROJECT_SOURCE_DIR})
else()
	include_directories(${PROJECT_SOURCE_DIR})
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMKAE_MODULE_PATH})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (NOT WIN32)
        message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

if (DEFINED PBD_EXTERNALINSTALLDIR)
	set(ExternalInstallDir "${PBD_EXTERNALINSTALLDIR}" CACHE INTERNAL "External install directory")	
else()
	set(ExternalInstallDir "${PROJECT_BINARY_DIR}" CACHE INTERNAL "External install directory")	
endif()	


# Eigen3配置

#增加一个子项目
add_subdirectory(src)


