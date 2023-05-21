include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_local_changes(GIT_LOCAL_CHANGES)

if (${GIT_LOCAL_CHANGES} STREQUAL "DIRTY")
	set(COMPILER_MESSAGE "#pragma WARNING(Local changes not committed.)")
endif()

file(STRINGS "../../version.txt" PBD_VERSION)

#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Version.h @ONLY)

file(GLOB ALL_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/utils/*.h"
)

add_library(utils
    ${ALL_SOURCES}

    CMakeLists.txt
)

############################################################
# GenericParameters
############################################################
include_directories(${GenericParameters_INCLUDE_DIR})
if(TARGET Ext_GenericParameters)
	add_dependencies(utils Ext_GenericParameters)
endif()

#find_package( Eigen3 REQUIRED )
#include_directories( ${EIGEN3_INCLUDE_DIR} )

target_include_directories(utils
    PUBLIC
        ${PROJECT_SOURCE_DIR}/extern/eigen
        ${PROJECT_SOURCE_DIR}/Common
        ${PROJECT_SOURCE_DIR}/include/utils
        ${PROJECT_SOURCE_DIR}/extern
)

message("Eigen dir is ${PROJECT_SOURCE_DIR}")

#添加GenericParameters

install(TARGETS utils
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

