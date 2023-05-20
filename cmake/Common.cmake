# 设置可执行文件的输出目录，也是源码目录下的bin目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin" CACHE INTERNAL "Where to place executables and dlls")
# 调试模式下可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin")
# 发布模式下可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin" CACHE INTERNAL "Where to place executables and dlls in release mode")
# 发布带调试信息模式下的可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin")
# 发布最小体积模式下可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/bin")
# 库文件的输出目录
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
# 调试模式下可执行文件和库文件的后缀 _d
set(CMAKE_DEBUG_POSTFIX "_d")
# 发布带调试信息模式下可执行文件和库文件后缀 _rd
set(CMAKE_RELWITHDEBINFO_POSTFIX "_rd")
# 发布最小体积模式可执行文件和库文件后缀 _ms
set(CMAKE_MINSIZEREL_POSTFIX "_ms")
# CMake 语言提供的一个函数，它允许在 CMake 项目中有条件地创建选项
include(CMakeDependentOption)

# 开关USE_PYTHON_BINDINGS，用于生成python绑定代码
cmake_dependent_option(USE_PYTHON_BINDINGS "Generate Python Bindings using PyBind11" ON "PYTHON_EXECUTABLE" OFF)
if (USE_PYTHON_BINDINGS AND UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    message(STATUS "Adding -fPIC option when generating Python bindings using GCC")
endif ()

# 标记CI_BUILD选项为高级选项
option(CI_BUILD "Build on CI System" OFF)
mark_as_advanced(CI_BUILD)

# 在非Windows系统下，不存在CMakeCache.txt文件，则将构建类型设置为Release。
if (NOT WIN32)
	if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
	  if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
	  endif()
	endif()
endif()

# 开关USE_OpenMP：用于启用OpenMP多线程并行计算库，并根据配置使用相应的编译器标志。
option(USE_OpenMP "Use OpenMP" ON)
if(USE_OpenMP)
	FIND_PACKAGE(OpenMP)
	if(OPENMP_FOUND)
		if (CMAKE_VERSION VERSION_GREATER "3.8")
			link_libraries(OpenMP::OpenMP_CXX)
		else()
			SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
			SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
		endif()
	endif()
endif()

# 在MSVC编译器下，设置一些编译器和链接器标志
if (MSVC)
    set(CMAKE_USE_RELATIVE_PATHS "1")
    # Set compiler flags
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP /bigobj")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /MP /Ox /Ob2 /Oi /Ot /D NDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /Zi /MP /Ox /Ob2 /Oi /Ot /D NDEBUG /bigobj") 
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
endif (MSVC)

# 在UNIX或MINGW系统下，设置一些编译器标志，以便根据不同的CPU架构和构建模式来进行优化
if (UNIX OR MINGW)
    set(CMAKE_USE_RELATIVE_PATHS "1")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
    # Set compiler flags for "release" Use generic 64 bit instructions when building on CI
	if (CI_BUILD)
		set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=x86-64")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -DNDEBUG -march=x86-64")
	elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
		set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -mcpu=native")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -DNDEBUG -mcpu=native")
	else()
		set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -DNDEBUG -march=native")
	endif ()
endif (UNIX OR MINGW)
if(MINGW)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1 -Wa,-mbig-obj")
endif(MINGW)

if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
	add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# 设置语言标准
set (CMAKE_CXX_STANDARD 11)

# 
if (MSVC)
add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

# 如果使用双精度浮点数，则添加预处理器宏定义
OPTION(USE_DOUBLE_PRECISION "Use double precision"	ON)
if (USE_DOUBLE_PRECISION)
	add_definitions( -DUSE_DOUBLE)	
endif (USE_DOUBLE_PRECISION)
